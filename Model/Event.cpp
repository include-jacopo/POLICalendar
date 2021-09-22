//
// Created by Riccardo Mengoli on 18/09/2021 17:49.
//

#include "Event.h"

#include <utility>
#include <iostream>

Event::Event() {

}

Event::Event(const std::string &uid, const std::string &name, const std::string &description,
             const std::string &location, const std::string &url, const std::chrono::time_point<std::chrono::system_clock> &creationTime,
             const std::chrono::time_point<std::chrono::system_clock> &startTime,
             const std::chrono::time_point<std::chrono::system_clock> &endTime):
        uidS(uid),nameS(name),descr(description),loc(location),url(url),
        creationT(creationTime), startT(startTime), endT(endTime){

}

const std::string &Event::getUid() const {
    return uidS;
}

void Event::setUid(const std::string &uid) {
    this->uidS = uid;

}

const std::string &Event::getName() const {
    return nameS;
}

void Event::setName(const std::string &name) {
    this->nameS = name;
}

const std::string &Event::getDescription() const {
    return  descr;
}

void Event::setDescription(const std::string &description) {
    this->descr = description;
}

const std::string &Event::getLocation() const {
    return loc;
}

void Event::setLocation(const std::string &location) {
    this->loc = location;
}

const std::string &Event::getUrl() const {
    return url;
}
void Event::setUrl(const std::string &url) {
    this->url = url;
}

const std::chrono::time_point<std::chrono::system_clock> &Event::getCreationTime() const {
    return creationT;
}

void Event::setCreationTime(const std::chrono::time_point<std::chrono::system_clock> &creationTime) {
    this->creationT = creationTime;
}

const std::chrono::time_point<std::chrono::system_clock> &Event::getStartTime() const {
    return startT;
}

void Event::setStartTime(const std::chrono::time_point<std::chrono::system_clock> &startTime) {
    this->startT = startTime;
}

const std::chrono::time_point<std::chrono::system_clock> &Event::getEndTime() const {
    return endT;
}

void Event::setEndTime(const std::chrono::time_point<std::chrono::system_clock> &endTime) {
    this->endT = endTime;
}

void Event::printEvent() {
    std::cout<<"I campi dell'evento sono i seguenti (stampa solo alcuni)"<<std::endl;
    std::cout<<"UID: "<<uidS<<" NAME: "<<nameS<<" DESCRIPTION: "<<descr<<" location: "<<loc<<" URL: "<<url<<std::endl;
}

