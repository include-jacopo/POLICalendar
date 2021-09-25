//
// Created by michele on 9/25/21.
//

#include "Task.h"

Task::Task(string uid, string name, string description, int priority, chrono::time_point<std::chrono::system_clock> date):
            uidS(uid), name(name), description(description), priority(priority), date(date),flagDate(true){};

Task::Task(string uid, string name, string description, int priority):
            uidS(uid), name(name), description(description), priority(priority),flagDate(false){};

const string &Task::getUid() const {
    return uidS;
}

void Task::setUidS(const string &uidS) {
    Task::uidS = uidS;
}

const string &Task::getName() const {
    return name;
}

void Task::setName(const string &name) {
    Task::name = name;
}

const string &Task::getDescription() const {
    return description;
}

void Task::setDescription(const string &description) {
    Task::description = description;
}

int Task::getPriority() const {
    return priority;
}

void Task::setPriority(int priority) {
    Task::priority = priority;
}

const chrono::time_point<std::chrono::system_clock> &Task::getDate() const {
    return date;
}

void Task::setDate(const chrono::time_point<std::chrono::system_clock> &date) {
    Task::date = date;
}

bool Task::isFlagDate() const {
    return flagDate;
}

void Task::setFlagDate(bool flagDate) {
    Task::flagDate = flagDate;
};
