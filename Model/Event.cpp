//
// Created by Riccardo Mengoli on 18/09/2021 17:49.
//

#include <iostream>
#include <iomanip>
#include "Event.h"

const std::string Event::createUid() {
    std::string s1("aaaaaaaa");
    std::string s2("aaaa");
    std::string s3("aaaa");
    std::string s4("aaaa");
    std::string s5("aaaaaaaaaaaa");
    /* UID: 7fe9b5cf-676f-4f77-8fa6-cc66b61ce4b3  */
    /* 5 campi alfanumerici separati da 4 - */

    /* calcolo 5 stringhe casuali */
    std::string ss1 = gen_random(s1, 8);
    std::string ss2 = gen_random(s2, 4);
    std::string ss3 = gen_random(s3, 4);
    std::string ss4 = gen_random(s4, 4);
    std::string ss5 = gen_random(s5, 12);

    /* le unisco */
    std::string s = ss1 + "-" + ss2 + "-" + ss3 + "-" + ss4 + "-" + ss5;

    return s;
}

Event::Event() {
    this->uidS = createUid();
}

Event::Event(const std::string &uid, const std::string &name, const std::string &description,
             const std::string &location, const std::string &url,
             const std::chrono::time_point<std::chrono::system_clock> &creationTime,
             const std::chrono::time_point<std::chrono::system_clock> &startTime,
             const std::chrono::time_point<std::chrono::system_clock> &endTime) :
             uidS(uid), nameS(name), descr(description), loc(location), url(url),
             creationT(creationTime), startT(startTime), endT(endTime) {
}

const std::string &Event::getUid() const {
    return this->uidS;
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
    return descr;
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

void Event::printEvent() const {

    std::cout << "Evento:\n";
    std::cout << "UID: " << uidS << " NAME: " << nameS << "\nDESCRIPTION: " << descr << "\nLOCATION: " << loc << "\nURL: "
              << url << "\n";
    auto tt = std::chrono::system_clock::to_time_t(startT);
    auto lt = std::localtime(&tt);
    std::cout << "START TIME: " << std::put_time(lt, "%d/%m/%Y %H:%M:%S") << "\n";
    tt = std::chrono::system_clock::to_time_t(endT);
    lt = std::localtime(&tt);
    std::cout << "END TIME: " << std::put_time(lt, "%d/%m/%Y %H:%M:%S") << "\n\n";
}


std::string Event::gen_random(std::string s, int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        int randNum = rand() % (63 - 0 + 1) + 0;
        s[i] = alphanum[randNum];
    }

    return s;
}
