//
// Created by michele on 9/25/21.
//

#include "Task.h"
#include <iostream>
#include <iomanip>

/**
 * Constructor of Task without parameters.
 */
Task::Task() : priority(1), completed(false), flagDate(false) {
    uidS = createUid();
};

/**
 * Constructor of Task.
 */
Task::Task(string uid, string name, string description, string location, string etag, int priority, bool completed, bool flagDate,
           chrono::time_point<std::chrono::system_clock> date, chrono::time_point<std::chrono::system_clock> dateS, chrono::time_point<std::chrono::system_clock> dateCompl):
           uidS(move(uid)), name(move(name)), description(move(description)), location(move(location)), etag(move(etag)), priority(priority), completed(completed),
           dueDate(date), flagDate(flagDate), dateS(dateS){
    if(completed)
        setDateCompleted(dateCompl);
    if(flagDate)
        setDueDate(date);
};

/**
 * Creates a random uid.
 * @return the uid string.
 */
std::string Task::createUid() {
    std::string s1("aaaaaaaa");
    std::string s2("aaaa");
    std::string s3("aaaa");
    std::string s4("aaaa");
    std::string s5("aaaaaaaaaaaa");
    /* UID: 5 campi alfanumerici separati da 4 divisori */

    //Calcolo 5 stringhe casuali
    std::string ss1 = gen_random(s1, 8);
    std::string ss2 = gen_random(s2, 4);
    std::string ss3 = gen_random(s3, 4);
    std::string ss4 = gen_random(s4, 4);
    std::string ss5 = gen_random(s5, 12);

    //Le unisco */
    std::string s = ss1 + "-" + ss2 + "-" + ss3 + "-" + ss4 + "-" + ss5;
    return s;
}

/**
 * Compute a random char from a dictionary and save it in a string.
 * @return the random string.
 */
std::string Task::gen_random(std::string s, int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        int randNum = rand() % (62 - 1);
        s[i] = alphanum[randNum];
    }

    return s;
}

/**
     * Gets the Task uidS
     * @return the string with uidS
     */
const string &Task::getUid() const {
    return uidS;
}

/**
     * Sets the Task uidS
     * @return void
     */
void Task::setUidS(const string &uidS) {
    Task::uidS = uidS;
}

/**
     * Gets the Task name
     * @return the string with name
     */
const string &Task::getName() const {
    return name;
}

/**
     * Sets the Task name
     * @return void
     */
void Task::setName(const string &name) {
    Task::name = name;
}

/**
     * Gets the Task description
     * @return the string with description
     */
const string &Task::getDescription() const {
    return description;
}

/**
     * Sets the Task description
     * @return void
     */
void Task::setDescription(const string &description) {
    Task::description = description;
}

/**
     * Gets the Task priority
     * @return the int with priority value
     */
int Task::getPriority() const {
    return priority;
}

/**
     * Sets the Task priority
     * @return void
     */
void Task::setPriority(int priority) {
    Task::priority = priority;
}

/**
     * Gets the Task location
     * @return the string with location
     */
const string &Task::getLocation() const {
    return location;
}

/**
     *Sets the Task location
     * @return void
     */
void Task::setLocation(const string &location) {
    Task::location = location;
}

/** Gets the Task dueDate
     * @return the  dueDate datetime UTC time
     */
const chrono::time_point<std::chrono::system_clock> &Task::getDueDate() const {
    return dueDate;
}
/** Sets the Task dueDate
     * @return void
     */
void Task::setDueDate(const chrono::time_point<std::chrono::system_clock> &date) {
    Task::dueDate = date;
}

/** Gets the Task flagDate
     * @return the bool value of flagDate
     */
bool Task::isFlagDate() const {
    return flagDate;
}

/**
     * Sets the Task field flagDate
     * @return void
     */
void Task::setFlagDate(bool flagDate) {
    Task::flagDate = flagDate;
}

/**
     * Gets the Task completed
     * @return the bool value of cinoketed
     */
bool Task::isCompleted() const {
    return completed;
}

/**
     *Sets the Task field completed
     * @return void
     */
void Task::setCompleted(bool completed) {
    Task::completed = completed;
};

/**
     * Gets the Task dateCompleted
     * @return the  datesCompleted datetime UTC time
     */
const chrono::time_point<std::chrono::system_clock> &Task::getDateCompleted() const {
    return dateCompleted;
}

/**
     * Sets the Task dateCompleted
     * @return void
     */
void Task::setDateCompleted(const chrono::time_point<std::chrono::system_clock> &dateCompleted) {
    Task::dateCompleted = dateCompleted;
}

/**
     * Gets the Task date
     * @return the  datesS datetime UTC time
     */
const std::chrono::time_point<std::chrono::system_clock> &Task::getDateS() const{
    return dateS;

};

/**
     * Sets the Task dateS
     * @return void
     */
void Task::setDateS(const chrono::time_point<std::chrono::system_clock> &date){
    Task::dateS = date;
}

/**
 * prints on terminal the fields of a task object.
 * @return void.
 */
void Task::printTask() {
    auto tt = std::chrono::system_clock::to_time_t(Task::getDueDate());
    auto ttS = std::chrono::system_clock::to_time_t( Task::getDateS());
    std::cout << "TASK:"<<endl;

    std::cout << "UID: " << uidS <<endl<< "NAME: " << name<< endl<< "DESCRIPTION: " << description <<endl << "LOCATION: " << location<<endl<<"ETAG: "<<etag<<endl;

    if(Task::isFlagDate()) {
        auto ttS2 = std::chrono::system_clock::to_time_t(Task::getDueDate());
        auto ttS2D = std::chrono::system_clock::to_time_t( Task::getDateS());
        std::cout << "time due Time: "<< std::put_time(std::localtime(&ttS2), "%b %d %Y %H:%M:%S" ) <<endl;
        std::cout << "time dtstamp Time: "<< std::put_time(std::localtime(&ttS2D), "%b %d %Y %H:%M:%S" ) <<endl;
    }
    else{
        auto ttS2D = std::chrono::system_clock::to_time_t( Task::getDateS());
        std::cout << "time dtstamp Time: "<< std::put_time(std::localtime(&ttS2D), "%b %d %Y %H:%M:%S" ) <<endl;
    }
    if(isCompleted()){
        cout<<"flag completed: "<<Task::isCompleted()<<endl;
        auto ttScompl = std::chrono::system_clock::to_time_t(Task::getDateCompleted());

        std::cout << "time due Time: "<< std::put_time(std::localtime(&ttScompl), "%b %d %Y %H:%M:%S" ) <<endl;
    }
}

/**
     * Gets the Task ETag
     * @return the ETag string
     */
const string &Task::getEtag() const {
    return etag;
}

/**
     * Sets the Task description
     * @return void
     */
void Task::setEtag(const string &etag) {
    Task::etag = etag;
}
