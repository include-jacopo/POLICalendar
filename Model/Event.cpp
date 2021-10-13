//
// Created by Riccardo Mengoli on 18/09/2021 17:49.
//

#include <iostream>
#include <iomanip>
#include "Event.h"
/**
 * Creates a random uid.
 * @return the uid string.
 */
const std::string Event::createUid() {
    std::string s1("aaaaaaaa");
    std::string s2("aaaa");
    std::string s3("aaaa");
    std::string s4("aaaa");
    std::string s5("aaaaaaaaaaaa");
    /* UID: 5 campi alfanumerici separati da 4 */

    /* creo le 5 stringhe casuali */
    std::string ss1 = gen_random(s1, 8);
    std::string ss2 = gen_random(s2, 4);
    std::string ss3 = gen_random(s3, 4);
    std::string ss4 = gen_random(s4, 4);
    std::string ss5 = gen_random(s5, 12);

    /* Le unisco */
    std::string s = ss1 + "-" + ss2 + "-" + ss3 + "-" + ss4 + "-" + ss5;
    return s;
}
/**
 * Creates a new event object, filling only the uidS field.
 * @return the event.
 */
Event::Event() {
    this->uidS = createUid();
}
/**
 * Constructor of event.
 */
Event::Event(const std::string &uid, const std::string &name, const std::string &description,
             const std::string &location, const std::string &url, const std::string &etag,
             const std::chrono::time_point<std::chrono::system_clock> &creationTime,
             const std::chrono::time_point<std::chrono::system_clock> &startTime,
             const std::chrono::time_point<std::chrono::system_clock> &endTime) :
             uidS(uid), nameS(name), descr(description), loc(location), url(url),
             etag(etag), creationT(creationTime), startT(startTime), endT(endTime) {
}

/**
     * Gets the event uid
     * @return the string with uid
     */
const std::string &Event::getUid() const {
    return this->uidS;
}

/**
     * Sets the Event uid
     * @return void
     */
void Event::setUid(const std::string &uid) {
    this->uidS = uid;
}

/**
     * Gets the event name
     * @return the string with name
     */
const std::string &Event::getName() const {
    return this->nameS;
}

/**
     * Sets the Event name
     * @return void
     */
void Event::setName(const std::string &name) {
    this->nameS = name;
}

/**
     * Gets the event description
     * @return the string with description
     */
const std::string &Event::getDescription() const {
    return this->descr;
}

/**
     * Sets the Event description
     * @return void
     */
void Event::setDescription(const std::string &description) {
    this->descr = description;
}

/**
     * Gets the event location
     * @return the string with location
     */
const std::string &Event::getLocation() const {
    return this->loc;
}

/**
     * Sets the Event location.
     * @return void.
     */
void Event::setLocation(const std::string &location) {
    this->loc = location;
}

/**
     * Gets the event url.
     * @return the string with url.
     */
const std::string &Event::getUrl() const {
    return this->url;
}

/**
     * Sets the Event url
     * @return void
     */
void Event::setUrl(const std::string &url) {
    this->url = url;
}

/**
     * Gets the event CreationTime
     * @return the  CreationTime datetime UTC time
     */
const std::chrono::time_point<std::chrono::system_clock> &Event::getCreationTime() const {
    return this->creationT;
}

/**
     * Sets the Event creationTime
     * @return void
     */
void Event::setCreationTime(const std::chrono::time_point<std::chrono::system_clock> &creationTime) {
    this->creationT = creationTime;
}

/**
     * Gets the event StartTime
     * @return the  StartTime datetime UTC time
     */
const std::chrono::time_point<std::chrono::system_clock> &Event::getStartTime() const {
    return this->startT;
}

/**
     * Sets the Event startTime.
     * @return void
     */
void Event::setStartTime(const std::chrono::time_point<std::chrono::system_clock> &startTime) {
    this->startT = startTime;
}

/**
     * Gets the event EndTime
     * @return the  EndTime datetime UTC time
     */
const std::chrono::time_point<std::chrono::system_clock> &Event::getEndTime() const {
    return this->endT;
}

/**
     * Sets the Event endTime
     * @return void
     */
void Event::setEndTime(const std::chrono::time_point<std::chrono::system_clock> &endTime) {
    this->endT = endTime;
}

/**
     * Gets the event ETag
     * @return the string with ETag
     */
const std::string &Event::getEtag() const {
    return this->etag;
}

/**
     * Sets the Event ETag
     * @return void
     */
void Event::setEtag(const std::string &etag) {
    this->etag = etag;
}
/**
 * Prints in the terminal the event.
 * @return void.
 */
void Event::printEvent() const {

    std::cout << "Evento:\n";
    std::cout << "UID: " << uidS << "\nNAME: " << nameS << "\nDESCRIPTION: " << descr << "\nLOCATION: " << loc << "\nURL: "
              << url << "\n";

    std::cout<<"ETAG: "<<etag<<std::endl;
    auto tt = std::chrono::system_clock::to_time_t(startT);
    auto lt = std::localtime(&tt);
    std::cout << "START TIME: " << std::put_time(lt, "%d/%m/%Y %H:%M:%S") << "\n";
    tt = std::chrono::system_clock::to_time_t(endT);
    lt = std::localtime(&tt);
    std::cout << "END TIME: " << std::put_time(lt, "%d/%m/%Y %H:%M:%S") << "\n\n";
}

/**
 * Compute a random char from a dictionary and save it in a string.
 * @return the random string.
 */
std::string Event::gen_random(std::string s, int len) {
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

