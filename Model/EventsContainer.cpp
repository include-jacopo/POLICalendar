//
// Created by michele on 9/22/21.
//

#include "EventsContainer.h"
#include <map>
#include <string>
#include <iostream>

using namespace std;

EventsContainer::EventsContainer(){};

void EventsContainer::addEvent(Event ev) {
    Events.insert({ev.getUid(), ev});
}
optional<Event> EventsContainer::deleteEvent(string uid){

    map<string,Event>::iterator it = Events.find(uid);
    Event removed;

    if(it!= Events.end()){
        /* ho trovato l'evento da rimuovere */
        removed = it->second;
        Events.erase(it);
        return removed;
    }


    /*
     * vecchia implementazione con il vector
    vector<Event>::iterator it;
    Event removed;
    it = remove_if(Events.begin(), Events.end(),[uid, &removed](Event ev){
                                if(ev.getUid()==uid) {
                                    removed = ev;
                                    return 1;
                                }
                                else
                                    return 0;} );
    if( it!= Events.end())
        return removed;
    */
}

optional<Event> EventsContainer::findEvent(string uid){

    map<string,Event>::iterator it = Events.find(uid);
    Event found;

    if(it!= Events.end()){
        /* ho trovato l'evento e lo ritorno */
        found = it->second;
        return found;
    }

    /*
     *  vecchia implementazione con vector

    vector<Event>::iterator it;
    Event found;
    it = find(Events.begin(), Events.end(),[uid, &found](Event ev){
        if(ev.getUid()==uid) {
            found = ev;
            return 1;
        }
        else
            return 0;} );
    if( it!= Events.end())
        return removed;
    */
}
void EventsContainer::displayEvents(){
    cout<<"**EVENTI ATTUALMENTE PRESENTI NEL CONTENITORE**"<<endl;
    for(auto i : Events){
        i.second.printEvent();
    }
    cout<<"***********************************************"<<endl;
}
