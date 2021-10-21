#ifndef POLICALENDAR_ICONTROLLER_H
#define POLICALENDAR_ICONTROLLER_H

using namespace std;

class IController {
public:
    /* **** GENERAL **** */

    /**
     * Connect to the CalDav server.
     * @param url CalDav server URL
     * @param usr Username
     * @param pw Password
     * @param port Server port
     * @return 0 -> Login successful
     * @return 1 -> Login failed
     * @return 2 -> Connection failed
     * @return 3 -> Connection timeout
     * @return 4 -> Generic server error
     * @return 5 -> Can't download events
     * @return 6 -> Can't download tasks
     */
    virtual int createSession (std::string url, std::string usr, std::string pw, int port) = 0;

    /**
     * Sync the local database with remote data.
     * @return true if request completed, false if error
     */
    virtual bool sync() = 0;

    /* **** EVENTS **** */

    /**
     * Get the container reference to read all the events.
     * @return Const reference to a map -> key = uid, value = Event
     */
    virtual const std::map<std::string, Event>& getEvents() = 0;

    /**
     * Add an event to the calendar.
     * @return true if event added, false if fail
     */
    virtual bool addEvent(Event ev) = 0;

    /**
     * Edit an event present in the calendar.
     * @return true if event edited, false if fail
     */
    virtual bool editEvent(Event ev) = 0;

    /**
     * Delete an event.
     * @return true if event deleted, false if fail
     */
    virtual bool deleteEvent(string uid) = 0;

    /* **** TASKS **** */

    /**
     * Get the container reference to read all the task.
     * @return Const reference to a map -> key = uid, value = Task
     */
    virtual const std::map<std::string, Task>& getTasks() = 0;

    /**
     * Add a task to the calendar.
     * @return true if task added, false if fail
     */
    virtual bool addTask(Task task) = 0;

    /**
     * Edit a task present in the calendar.
     * @return true if task edited, false if fail
     */
    virtual bool editTask(Task task) = 0;

    /**
     * Delete a task.
     * @return true if task deleted, false if fail
     */
    virtual bool deleteTask(string uid) = 0;
};

#endif //POLICALENDAR_ICONTROLLER_H
