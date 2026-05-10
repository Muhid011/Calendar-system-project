/* main.c
   Starts the program, shows the menu, and calls
   the right function based on what the user chooses.
   Events are saved to events.txt every time something changes. */

#include "calendar.h"
#include <stdio.h>

int main(void)
{
    EventStore store;
    init_store(&store);

    printf("\n");
    show_line();
    printf("  Calendar Management System\n");
    show_line();

    load_from_file(&store);

    int choice = -1;

    while (choice != 0)
    {
        show_menu();
        scanf("%d", &choice);
        while (getchar() != '\n');

        if (choice == 1)
        {
            Event e = build_event(next_id(&store));
            add_event(&store, e);
            printf("\n  Added: %s (ID %d)\n", e.title, e.id);
            save_to_file(&store);
        }
        else if (choice == 2)
        {
            printf("\n  All events (%d):\n", count_events(&store));
            list_all(&store);
        }
        else if (choice == 3)
        {
            printf("\n  Upcoming events:\n");
            list_upcoming(&store);
        }
        else if (choice == 4)
        {
            char date[MAX_DATE];
            get_text("Date to search ", date, MAX_DATE);
            list_by_date(&store, date);
        }
        else if (choice == 5)
        {
            printf("  1)Meeting 2)Appt 3)Reminder 4)Personal 5)Holiday\n");
            int t = get_number("Pick a type");
            list_by_type(&store, (EventType)t);
        }
        else if (choice == 6)
        {
            printf("\n  Events with reminders:\n");
            list_reminders(&store);
        }
        else if (choice == 7)
        {
            printf("\n  Checking for conflicts...\n");
            find_conflicts(&store);
        }
        else if (choice == 8)
        {
            int id = get_number("ID of event to edit");
            Event *e = find_by_id(&store, id);
            if (e == NULL)
                printf("  Not found.\n");
            else
            {
                edit_event(e);
                save_to_file(&store);
                printf("  Saved.\n");
            }
        }
        else if (choice == 9)
        {
            int id = get_number("ID of event to delete");
            Event *e = find_by_id(&store, id);
            if (e == NULL)
                printf("  Not found.\n");
            else
            {
                printf("  Deleted: %s\n", e->title);
                delete_event(&store, id);
                save_to_file(&store);
            }
        }
        else if (choice == 0)
        {
            save_to_file(&store);
            printf("\n  Goodbye!\n\n");
        }
        else
        {
            printf("  Please enter a number from 0 to 9.\n");
        }
    }

    free_store(&store);
    return 0;
}
