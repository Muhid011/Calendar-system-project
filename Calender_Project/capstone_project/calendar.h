/* calendar.h
   Holds all the structs and function names used by the program. */

#ifndef CALENDAR_H
#define CALENDAR_H

#include <stdbool.h>

/* How long each text field can be */
#define MAX_TITLE  100
#define MAX_DESC   300
#define MAX_LOC    100
#define MAX_DATE    20
#define DATA_FILE  "events.txt"

/* The five kinds of events */
typedef enum
{
    MEETING     = 1,
    APPOINTMENT = 2,
    REMINDER    = 3,
    PERSONAL    = 4,
    HOLIDAY     = 5
} EventType;

/* One calendar event */
typedef struct
{
    int       id;
    char      title[MAX_TITLE];
    char      description[MAX_DESC];
    char      location[MAX_LOC];
    char      startTime[MAX_DATE];
    char      endTime[MAX_DATE];
    int       isAllDay;
    EventType type;
    int       reminderMinutes;
    int       isRecurring;
    int       repeatEveryDays;
    char      repeatUntil[MAX_DATE];
} Event;

/* One node in the linked list */
typedef struct EventNode
{
    Event             data;
    struct EventNode *next;
} EventNode;

/* The list itself */
typedef struct
{
    EventNode *head;
    int        nextId;
} EventStore;

/* Functions in calendar_functions.c */
void   init_store(EventStore *store);
void   free_store(EventStore *store);
void   add_event(EventStore *store, Event e);
bool   delete_event(EventStore *store, int id);
Event *find_by_id(EventStore *store, int id);
void   list_all(const EventStore *store);
void   list_by_type(const EventStore *store, EventType type);
void   list_by_date(const EventStore *store, const char *date);
void   list_upcoming(const EventStore *store);
void   list_reminders(const EventStore *store);
void   find_conflicts(const EventStore *store);
void   save_to_file(const EventStore *store);
void   load_from_file(EventStore *store);
void   print_event(const Event *e);
int    next_id(const EventStore *store);
int    count_events(const EventStore *store);
const char *type_name(EventType t);

/* Functions in calendar_manager.c */
void  show_menu(void);
void  show_line(void);
Event build_event(int id);
void  edit_event(Event *e);
int   get_number(const char *label);
void  get_text(const char *label, char *dest, int max);

#endif
