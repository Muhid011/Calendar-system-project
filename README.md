# Calendar Management System

### What the Program Does
The point of this project is to create a program that manages calendar events like meetings, appointments, and reminders. This program helps you stay organized by letting you:
* **Add** new events
* **View** all your scheduled plans
* **Edit or Delete** events if your plans change
* **Search** by specific dates or event types
* **Conflict Check** to make sure you aren't double-booked
* **Set Reminders** for important tasks

---

### My Solution
To build this, I used a **Linked List of Structs** to store event information in the computer's memory. Each event (struct) keeps track of an ID, title, description, location, start/end times, and reminders.

**The Main Menu Options Include:**
1. Add a new event
2. View all events
3. View upcoming events
4. Search by date
5. Search by type
6. View reminders
7. Check for conflicts
8. Edit an event
9. Delete an event

I also added **File Handling** to make the data permanent. The program saves everything to `events.txt` using `fprintf` and loads it back up using `fgets` every time you start it. This means your data is never lost, unless you delete it!

---

### File Structure
* **main.c**: The "brain" that starts the program and runs the menu.
* **calendar_functions.c**: The "muscles" that handle the linked list and conflict detection.
* **calendar_manager.c**: The "face" that handles user input and the display.
* **calendar.h**: The "rulebook" containing all struct definitions.
* **Makefile**: Used to compile the program quickly.
* **test_events.txt**: Sample data to help you test the program right away.

---

### How to Run
To get the program running on your machine, use these commands:

1. **Compile the program:**
   ```bash
   gcc main.c calendar_functions.c calendar_manager.c -o calendar
   ```
2. **Run it:**
   ```bash
   ./calendar
   ```

---

### Pros and Cons

**Pros:**
* **User Friendly:** Easy to use, simple menu system.
* **Flexibility:** You can add, edit, or delete events whenever you want.
* **Auto Checks:** Automatically checks for scheduling conflicts.
* **Organized:** Filters events by date or type.
* **Data Never Lost:** Data stays saved even after you close the program.

**Cons:**
* **Formatting:** If you type a date wrong, the program might have some trouble reading it.
* **Performance Check:** The conflict check can get slower if you have a huge number of events.
* **Storage:** The whole file is rewritten every time a change is made.
* **Single User:** Everyone who runs the program shares the same calendar.
