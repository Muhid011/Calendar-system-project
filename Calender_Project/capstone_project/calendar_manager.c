/* calendar_manager.c
   Shows the menu and handles all user input. */

#include "calendar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_line(void)
{
    printf("  ==============================\n");
}

void show_menu(void)
{
    printf("\n");
    show_line();
    printf("   BY HAMZA MUHIDIN\n");
    show_line();
    printf("  1. Add a new event\n");
    printf("  2. View all events\n");
    printf("  3. View upcoming events\n");
    printf("  4. Search by date\n");
    printf("  5. Search by type\n");
    printf("  6. View reminders\n");
    printf("  7. Check for conflicts\n");
    printf("  8. Edit an event\n");
    printf("  9. Delete an event\n");
    printf("  0. Save and quit\n");
    show_line();
    printf("  Choice: ");
}

/* Ask to fill in all the information for a new event */
Event build_event(int id)
{
    Event e;
    memset(&e, 0, sizeof(Event));
    e.id = id;

    printf("\n");
    get_text("Title", e.title, MAX_TITLE);
    get_text("Description (or press Enter to skip)", e.description, MAX_DESC);
    get_text("Location    (or press Enter to skip)", e.location, MAX_LOC);
    get_text("Start time  (Date and time)", e.startTime, MAX_DATE);

    printf("  All day? (1=yes 0=no): ");
    scanf("%d", &e.isAllDay);
    while (getchar() != '\n');

    if (!e.isAllDay)
        get_text("End time  (date and time)", e.endTime, MAX_DATE);
    else
        strncpy(e.endTime, e.startTime, MAX_DATE - 1);

    printf("  Type: 1)Meeting 2)Appt 3)Reminder 4)Personal 5)Holiday\n");
    e.type = (EventType)get_number("  Pick 1-5");

    e.reminderMinutes = get_number("Reminder minutes before (0=none)");

    printf("  Does it repeat? (1=yes 0=no): ");
    scanf("%d", &e.isRecurring);
    while (getchar() != '\n');

    if (e.isRecurring)
    {
        e.repeatEveryDays = get_number("Repeat every how many days?");
        get_text("Stop repeating on this date", e.repeatUntil, MAX_DATE);
    }

    return e;
}

/* Let the user change information on an existing event.
   Just press Enter to keep the old value. */
void edit_event(Event *e)
{
    char buf[MAX_DESC];

    printf("  Press Enter to keep the current value.\n\n");

    printf("  Title [%s]: ", e->title);
    fgets(buf, MAX_TITLE, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(e->title, buf, MAX_TITLE - 1);

    printf("  Description [%s]: ", e->description);
    fgets(buf, MAX_DESC, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(e->description, buf, MAX_DESC - 1);

    printf("  Location [%s]: ", e->location);
    fgets(buf, MAX_LOC, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(e->location, buf, MAX_LOC - 1);

    printf("  Start time [%s]: ", e->startTime);
    fgets(buf, MAX_DATE, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(e->startTime, buf, MAX_DATE - 1);

    if (!e->isAllDay)
    {
        printf("  End time [%s]: ", e->endTime);
        fgets(buf, MAX_DATE, stdin);
        buf[strcspn(buf, "\n")] = '\0';
        if (strlen(buf) > 0) strncpy(e->endTime, buf, MAX_DATE - 1);
    }

    printf("  Reminder minutes [%d]: ", e->reminderMinutes);
    fgets(buf, MAX_TITLE, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) e->reminderMinutes = atoi(buf);
}

int get_number(const char *label)
{
    int n = 0;
    printf("  %s: ", label);
    scanf("%d", &n);
    while (getchar() != '\n');
    return n;
}

void get_text(const char *label, char *dest, int max)
{
    printf("  %s: ", label);
    fgets(dest, max, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}
