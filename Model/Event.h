//
// Created by Riccardo Mengoli on 18/09/2021 17:22.
//

#ifndef POLICALENDAR_EVENT_H
#define POLICALENDAR_EVENT_H

#include <string>
#include <chrono>

class Event {
private:
    std::string uidS, nameS, descr, loc, url;
    std::chrono::time_point<std::chrono::system_clock> creationT;
    std::chrono::time_point<std::chrono::system_clock> startT;
    std::chrono::time_point<std::chrono::system_clock> endT;

    std::string gen_random(std::string s, int len);


public:
    Event();
    Event(const std::string &uid, const std::string &name, const std::string &description, const std::string &location, const std::string &url,
          const std::chrono::time_point<std::chrono::system_clock> &creationTime,
          const std::chrono::time_point<std::chrono::system_clock> &startTime,
          const std::chrono::time_point<std::chrono::system_clock> &endTime);

    const std::string &getUid() const;
    void setUid(const std::string &uid);
    const std::string &getName() const;
    void setName(const std::string &name);
    const std::string &getDescription() const;
    void setDescription(const std::string &description);
    const std::string &getLocation() const;
    void setLocation(const std::string &location);
    const std::string &getUrl() const;
    void setUrl(const std::string &url);
    const std::chrono::time_point<std::chrono::system_clock> &getCreationTime() const;
    void setCreationTime(const std::chrono::time_point<std::chrono::system_clock> &creationTime);
    const std::chrono::time_point<std::chrono::system_clock> &getStartTime() const;
    void setStartTime(const std::chrono::time_point<std::chrono::system_clock> &startTime);
    const std::chrono::time_point<std::chrono::system_clock> &getEndTime() const;
    void setEndTime(const std::chrono::time_point<std::chrono::system_clock> &endTime);
    void printEvent();
};

#endif //POLICALENDAR_EVENT_H
