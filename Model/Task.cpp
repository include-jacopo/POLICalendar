//
// Created by michele on 9/25/21.
//

#include "Task.h"
#include <iostream>
#include <iomanip>

Task::Task() : priority(1), completed(false), flagDate(false) {};

Task::Task(string uid, string name, string description, string location, string etag, int priority, bool completed,chrono::time_point<std::chrono::system_clock> dateS):
            uidS(uid), name(name), description(description), location(location), etag(etag), priority(priority),
            completed(completed), flagDate(false), dateS(dateS) {};
Task::Task(string uid, string name, string description, string location, string etag, int priority, bool completed,
           chrono::time_point<std::chrono::system_clock> date, chrono::time_point<std::chrono::system_clock> dateS):
        uidS(uid), name(name), description(description), location(location), etag(etag), priority(priority), completed(completed),
        date(date), flagDate(true), dateS(dateS){};

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

const string &Task::getLocation() const {
    return location;
}

void Task::setLocation(const string &location) {
    Task::location = location;
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
}

bool Task::isCompleted() const {
    return completed;
}

void Task::setCompleted(bool completed) {
    Task::completed = completed;
};

const std::chrono::time_point<std::chrono::system_clock> &Task::getDateS() const{
    return dateS;

};
void Task::setDateS(const chrono::time_point<std::chrono::system_clock> &date){
    Task::dateS = date;
}

void Task::printTask() {
    auto tt = std::chrono::system_clock::to_time_t( Task::getDate());
    auto ttS = std::chrono::system_clock::to_time_t( Task::getDateS());
    auto lt = std::localtime(&tt);
    auto ltS = std::localtime(&ttS);
    std::cout << "TASK:"<<endl;

    std::cout << "UID: " << uidS <<endl<< "NAME: " << name<< endl<< "DESCRIPTION: " << description <<endl << "LOCATION: " << location<<endl<<"ETAG: "<<etag<<endl;

    if(Task::isFlagDate()) {
        auto ttS2 = std::chrono::system_clock::to_time_t( Task::getDate());
        auto ttS2D = std::chrono::system_clock::to_time_t( Task::getDateS());
        std::cout << "time due Time: "<< std::put_time(std::localtime(&ttS2), "%b %d %Y %H:%M:%S" ) <<endl;
        std::cout << "time dtstamp Time: "<< std::put_time(std::localtime(&ttS2D), "%b %d %Y %H:%M:%S" ) <<endl;
        //std::cout << "DUE TIME: " << std::put_time(std::localtime(&ttS2), "%d/%m/%Y %H:%M:%S") << endl;
        //std::cout<<"DSTAMP TIME: "<<std::put_time(ltS, "%d/%m/%Y %H:%M:%S") <<endl;
    }
    else{
        auto ttS2D = std::chrono::system_clock::to_time_t( Task::getDateS());
        std::cout << "time dtstamp Time: "<< std::put_time(std::localtime(&ttS2D), "%b %d %Y %H:%M:%S" ) <<endl;
    }

}

const string &Task::getEtag() const {
    return etag;
}

void Task::setEtag(const string &etag) {
    Task::etag = etag;
}
