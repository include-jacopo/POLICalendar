#ifndef POLICALENDAR_CONTROLLER_H
#define POLICALENDAR_CONTROLLER_H

#include "../Model/Event.h"
#include "../Model/Task.h"
#include "IController.h"
#include "WebClient.h"
#include <map>
#include <optional>

using namespace std;

class Controller : public IController {

public:
    static Controller* getInstance();
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;

    /**
    * General methods.
    */
    int createSession (string url, string usr, string pw, int port) override;
    bool sync() override;

    /**
    * Events methods.
    */
    bool downloadEvents();
    const map<string, Event>& getEvents() override;
    bool updateEvents();
    bool addEvent(Event ev) override;
    bool editEvent(Event ev) override;
    bool deleteEvent(string uid) override;
    bool updateCtagCalendar();
    void displayEvents();
    optional<Event> findEvent(const string& uid);

    /**
    * Tasks methods.
    */
    bool downloadTask();
    const map<std::string, Task> &getTasks() override;
    bool addTask(Task task) override;
    bool editTask(Task task) override;
    bool deleteTask(string uid) override;
    bool updateCtagTask();
    bool updateTasks();
    void displayTasks();

private:
    map<string, Event> Events;
    map<string, Task> Tasks;
    WebClient wc;
    static Controller *instance;
    Controller(); // Private constructor for singleton pattern
    void insertLocalEvent(const Event& ev);
    void insertLocalTask(const Task& t);
};

#endif //POLICALENDAR_CONTROLLER_H
