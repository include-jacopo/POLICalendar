//
// Created by michele on 9/22/21.
//

#ifndef POLICALENDAR_CONTROLLER_H
#define POLICALENDAR_CONTROLLER_H

#include "../Model/Event.h"
#include "../View/MainWindow.h"
#include <map>
#include <optional>

using namespace std;

class Controller {
private:
    MainWindow *view;
    map<string, Event> Events;
public:
    Controller();
    void setView(MainWindow *view);

    void addEvent(Event ev);
    optional<Event> findEvent(string uid);
    optional<Event> deleteEvent(string uid);
    void displayEvents();


};


#endif //POLICALENDAR_CONTROLLER_H
