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
    void createSession (string url, string usr, string pw, int port);
    static Controller* getInstance();
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;
    bool downloadEvents();
    const map<string, Event>& getEvents() override;
    bool updateEvents() override;
    bool addEvent(Event ev) override;
    optional<Event> findEvent(string uid) override;
    bool deleteEvent(string uid) override;
    void displayEvents();

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
