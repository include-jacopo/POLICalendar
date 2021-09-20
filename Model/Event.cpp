//
// Created by Riccardo Mengoli on 18/09/2021 17:49.
//

#include "Event.h"

Event::Event() = default;

Event::Event(const std::string &uid) : uid(uid) {}

Event::Event(const std::string &uid, const std::string &summary, const std::string &description,
             const std::string &location, const std::chrono::time_point<std::chrono::system_clock> &creationTime,
             const std::chrono::time_point<std::chrono::system_clock> &startTime,
             const std::chrono::time_point<std::chrono::system_clock> &endTime) : uid(uid), summary(summary),
             description(description), location(location), creationTime(creationTime), startTime(startTime), endTime(endTime) {};

const std::string &Event::getUid() const {
    return uid;
}

void Event::setUid(const std::string &uid) {
    Event::uid = uid;
}

const std::string &Event::getName() const {
    return summary;
}

void Event::setName(const std::string &name) {
    Event::summary = name;
}

const std::string &Event::getDescription() const {
    return description;
}

void Event::setDescription(const std::string &description) {
    Event::description = description;
}

const std::string &Event::getLocation() const {
    return location;
}

void Event::setLocation(const std::string &location) {
    Event::location = location;
}

const std::chrono::time_point<std::chrono::system_clock> &Event::getCreationTime() const {
    return creationTime;
}

void Event::setCreationTime(const std::chrono::time_point<std::chrono::system_clock> &creationTime) {
    Event::creationTime = creationTime;
}

const std::chrono::time_point<std::chrono::system_clock> &Event::getStartTime() const {
    return startTime;
}

void Event::setStartTime(const std::chrono::time_point<std::chrono::system_clock> &startTime) {
    Event::startTime = startTime;
}

const std::chrono::time_point<std::chrono::system_clock> &Event::getEndTime() const {
    return endTime;
}

void Event::setEndTime(const std::chrono::time_point<std::chrono::system_clock> &endTime) {
    Event::endTime = endTime;
}

