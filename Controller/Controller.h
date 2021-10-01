//
// Created by michele on 9/22/21.
//

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
    bool createSession (string url, string usr, string pw, int port) override;
    static Controller* getInstance();
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;

    /* General methods */
    bool sync() override;

    /* Events methods */
    bool downloadEvents();
    const map<string, Event>& getEvents() override;
    bool updateEvents();
    optional<Event> findEvent(string uid);
    bool addEvent(Event ev) override;
    bool editEvent(Event ev) override;
    bool deleteEvent(string uid) override;
    void displayEvents();
    bool updateCtagCalendar();

    /* Tasks methods */
    bool downloadTask();
    const map<std::string, Task> &getTasks() override;
    bool addTask(Task task) override;
    bool editTask(Task task) override;
    bool deleteTask(string uid) override;
    void displayTasks();
    bool updateCtagTask();
    bool updateTasks();

private:
    map<string, Event> Events;
    map<string, Task> Tasks;
    WebClient wc;

    static Controller *instance;
    Controller(); // Private constructor for singleton pattern
    int insertLocalEvent(Event ev);
    int insertLocalTask(Task t);
};

#endif //POLICALENDAR_CONTROLLER_H
