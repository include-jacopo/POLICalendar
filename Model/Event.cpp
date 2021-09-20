//
// Created by Riccardo Mengoli on 18/09/2021 17:49.
//

#include "Event.h"

#include <utility>

Event::Event() {

}

Event::Event(const std::string &uid, const std::string &summary, const std::string &description,
             const std::string &location, const std::chrono::time_point<std::chrono::system_clock> &creationTime,
             const std::chrono::time_point<std::chrono::system_clock> &startTime,
             const std::chrono::time_point<std::chrono::system_clock> &endTime) {

}

const std::string &Event::getUid() const {

}

void Event::setUid(const std::string &uid) {

}

const std::string &Event::getName() const {

}

void Event::setName(const std::string &name) {

}

const std::string &Event::getDescription() const {

}

void Event::setDescription(const std::string &description) {

}

const std::string &Event::getLocation() const {

}

void Event::setLocation(const std::string &location) {

}

const std::chrono::time_point<std::chrono::system_clock> &Event::getCreationTime() const {

}

void Event::setCreationTime(const std::chrono::time_point<std::chrono::system_clock> &creationTime) {

}

const std::chrono::time_point<std::chrono::system_clock> &Event::getStartTime() const {

}

void Event::setStartTime(const std::chrono::time_point<std::chrono::system_clock> &startTime) {

}

const std::chrono::time_point<std::chrono::system_clock> &Event::getEndTime() const {

}

void Event::setEndTime(const std::chrono::time_point<std::chrono::system_clock> &endTime) {

}

