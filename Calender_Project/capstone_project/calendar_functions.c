/* calendar_functions.c
   file save/load, and conflict checking. */

#include "calendar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Set up an empty list */
void init_store(EventStore *store)
{
    store->head   = NULL;
    store->nextId = 1;
}

/* Free all nodes when the program ends */
void free_store(EventStore *store)
{
    EventNode *cur = store->head;
    while (cur != NULL)
    {
        EventNode *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    store->head = NULL;
}

/* Add a new event to the end of the list */
void add_event(EventStore *store, Event e)
{
    EventNode *node = (EventNode *)malloc(sizeof(EventNode));
    if (node == NULL)
    {
        printf("Error: out of memory.\n");
        return;
    }
    node->data = e;
    node->next = NULL;

    if (store->head == NULL)
    {
        store->head = node;
    }
    else
    {
        EventNode *cur = store->head;
        while (cur->next != NULL)
            cur = cur->next;
        cur->next = node;
    }

    if (e.id >= store->nextId)
        store->nextId = e.id + 1;
}

/* Remove an event by ID, free its memory */
bool delete_event(EventStore *store, int id)
{
    if (store->head == NULL)
        return false;

    if (store->head->data.id == id)
    {
        EventNode *tmp = store->head;
        store->head = store->head->next;
        free(tmp);
        return true;
    }

    EventNode *prev = store->head;
    while (prev->next != NULL && prev->next->data.id != id)
        prev = prev->next;

    if (prev->next == NULL)
        return false;

    EventNode *tmp = prev->next;
    prev->next = tmp->next;
    free(tmp);
    return true;
}

/* Find an event by ID, return a pointer to it */
Event *find_by_id(EventStore *store, int id)
{
    EventNode *cur = store->head;
    while (cur != NULL)
    {
        if (cur->data.id == id)
            return &cur->data;
        cur = cur->next;
    }
    return NULL;
}

/* Print every event */
void list_all(const EventStore *store)
{
    if (store->head == NULL)
    {
        printf("  No events yet.\n");
        return;
    }
    EventNode *cur = store->head;
    while (cur != NULL)
    {
        print_event(&cur->data);
        cur = cur->next;
    }
}

/* Print events that match the given type */
void list_by_type(const EventStore *store, EventType type)
{
    int found = 0;
    EventNode *cur = store->head;
    while (cur != NULL)
    {
        if (cur->data.type == type)
        {
            print_event(&cur->data);
            found = 1;
        }
        cur = cur->next;
    }
    if (!found)
        printf("  No events of that type.\n");
}

/* Print events that start on a given date */
void list_by_date(const EventStore *store, const char *date)
{
    int found = 0;
    int len = (int)strlen(date);
    EventNode *cur = store->head;
    while (cur != NULL)
    {
        if (strncmp(cur->data.startTime, date, len) == 0)
        {
            print_event(&cur->data);
            found = 1;
        }
        cur = cur->next;
    }
    if (!found)
        printf("  No events on that date.\n");
}

/* Print events that have not happened yet */
void list_upcoming(const EventStore *store)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char today[MAX_DATE];
    strftime(today, sizeof(today), "%Y-%m-%d", t);

    int found = 0;
    EventNode *cur = store->head;
    while (cur != NULL)
    {
        if (strcmp(cur->data.startTime, today) >= 0)
        {
            print_event(&cur->data);
            found = 1;
        }
        cur = cur->next;
    }
    if (!found)
        printf("  No upcoming events.\n");
}

/* Print events that have a reminder set */
void list_reminders(const EventStore *store)
{
    int found = 0;
    EventNode *cur = store->head;
    while (cur != NULL)
    {
        if (cur->data.reminderMinutes > 0)
        {
            printf("\n  Reminder: %d min before\n", cur->data.reminderMinutes);
            print_event(&cur->data);
            found = 1;
        }
        cur = cur->next;
    }
    if (!found)
        printf("  No events have reminders.\n");
}

/* Check every pair of events for time overlaps
   Two events conflict if A starts before B ends
   AND A ends after B starts. */
void find_conflicts(const EventStore *store)
{
    int found = 0;
    EventNode *a = store->head;
    while (a != NULL)
    {
        EventNode *b = a->next;
        while (b != NULL)
        {
            if (!a->data.isAllDay && !b->data.isAllDay)
            {
                int overlap =
                    strcmp(a->data.startTime, b->data.endTime)   < 0 &&
                    strcmp(a->data.endTime,   b->data.startTime) > 0;

                if (overlap)
                {
                    printf("\n  !! Conflict !!\n");
                    printf("  %s  (%s to %s)\n",
                           a->data.title, a->data.startTime, a->data.endTime);
                    printf("  %s  (%s to %s)\n",
                           b->data.title, b->data.startTime, b->data.endTime);
                    found = 1;
                }
            }
            b = b->next;
        }
        a = a->next;
    }
    if (!found)
        printf("  No conflicts found.\n");
}

/* Save all events to events.txt */
void save_to_file(const EventStore *store)
{
    FILE *f = fopen(DATA_FILE, "w");
    if (f == NULL)
    {
        printf("  Error: could not save file.\n");
        return;
    }
    EventNode *cur = store->head;
    while (cur != NULL)
    {
        const Event *e = &cur->data;
        fprintf(f, "%d|%s|%s|%s|%s|%s|%d|%d|%d|%d|%d|%s\n",
                e->id, e->title, e->description, e->location,
                e->startTime, e->endTime, e->isAllDay,
                (int)e->type, e->reminderMinutes,
                e->isRecurring, e->repeatEveryDays, e->repeatUntil);
        cur = cur->next;
    }
    fclose(f);
    printf("  Saved %d event(s).\n", count_events(store));
}

/* Load events from events.txt when the program starts */
void load_from_file(EventStore *store)
{
    FILE *f = fopen(DATA_FILE, "r");
    if (f == NULL)
        return;

    char line[800];
    int loaded = 0;

    while (fgets(line, sizeof(line), f))
    {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue;

        Event e;
        memset(&e, 0, sizeof(Event));

        char allday[4], type[4], remind[8], recur[4], days[8];

        int n = sscanf(line,
            "%d|%99[^|]|%299[^|]|%99[^|]|%19[^|]|%19[^|]"
            "|%3[^|]|%3[^|]|%7[^|]|%3[^|]|%7[^|]|%19[^\n]",
            &e.id, e.title, e.description, e.location,
            e.startTime, e.endTime,
            allday, type, remind, recur, days, e.repeatUntil);

        if (n >= 11)
        {
            e.isAllDay        = atoi(allday);
            e.type            = (EventType)atoi(type);
            e.reminderMinutes = atoi(remind);
            e.isRecurring     = atoi(recur);
            e.repeatEveryDays = atoi(days);
            add_event(store, e);
            loaded++;
        }
    }
    fclose(f);
    if (loaded > 0)
        printf("  Loaded %d event(s) from last time.\n", loaded);
}

/* Helper: print one event */
void print_event(const Event *e)
{
    printf("\n  [%d] %s\n", e->id, e->title);
    printf("  Type : %s\n", type_name(e->type));
    if (e->isAllDay)
        printf("  When : %s (all day)\n", e->startTime);
    else
        printf("  When : %s  to  %s\n", e->startTime, e->endTime);
    if (strlen(e->location) > 0)
        printf("  Where: %s\n", e->location);
    if (strlen(e->description) > 0)
        printf("  Notes: %s\n", e->description);
    if (e->reminderMinutes > 0)
        printf("  Alarm: %d min before\n", e->reminderMinutes);
    if (e->isRecurring)
        printf("  Reps : every %d day(s) until %s\n",
               e->repeatEveryDays, e->repeatUntil);
    printf("  ---\n");
}

int next_id(const EventStore *store)
{
    return store->nextId;
}

int count_events(const EventStore *store)
{
    int n = 0;
    EventNode *cur = store->head;
    while (cur != NULL) { n++; cur = cur->next; }
    return n;
}

const char *type_name(EventType t)
{
    switch (t)
    {
        case MEETING:     return "Meeting";
        case APPOINTMENT: return "Appointment";
        case REMINDER:    return "Reminder";
        case PERSONAL:    return "Personal";
        case HOLIDAY:     return "Holiday";
        default:          return "Other";
    }
}
