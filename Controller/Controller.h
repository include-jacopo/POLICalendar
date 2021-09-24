//
// Created by michele on 9/22/21.
//

#ifndef POLICALENDAR_CONTROLLER_H
#define POLICALENDAR_CONTROLLER_H

#include "../Model/Event.h"
#include "IController.h"
#include "WebClient.h"
#include <map>
#include <optional>

using namespace std;

class Controller : public IController {
private:
    map<string, Event> Events;
    WebClient wc;

    static Controller *instance;
    Controller(); // Private constructor for singleton pattern
    int insertLocalEvent(Event ev);

public:
    static Controller* getInstance();
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;

    const map<string, Event>& getEvents() override;
    bool updateEvents() override;
    bool addEvent(Event ev) override;
    optional<Event> findEvent(string uid) override;
    bool deleteEvent(string uid) override;
    void displayEvents();
};

#endif //POLICALENDAR_CONTROLLER_H
