//
// Created by michele on 9/22/21.
//

#ifndef POLICALENDAR_CONTROLLER_H
#define POLICALENDAR_CONTROLLER_H

#include "../Model/Event.h"
#include "../View/IView.h"
#include "IController.h"
#include "WebClient.h"
#include <map>
#include <optional>

using namespace std;

class Controller : public IController {
private:
    IView *view;
    map<string, Event> Events;
    WebClient wc;
public:
    Controller();

    pair<forward_iterator_tag, forward_iterator_tag> getEvents() override;
    bool updateEvents() override;
    bool addEvent(Event ev) override;
    optional<Event> findEvent(string uid) override;
    bool deleteEvent(string uid) override;
    void displayEvents();

    void setView(IView *view);
};


#endif //POLICALENDAR_CONTROLLER_H
