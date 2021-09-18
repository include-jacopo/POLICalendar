//
// Created by Riccardo Mengoli on 18/09/2021 17:22.
//

#ifndef POLICALENDAR_EVENT_H
#define POLICALENDAR_EVENT_H

#include <string>
#include <chrono>

class Event {
private:
    std::string uid;
    std::string summary, description, location;
    std::chrono::time_point<std::chrono::system_clock> creationTime, startTime, endTime;

public:
    Event();

    Event(const std::string &uid);

    Event(const std::string &uid, const std::string &name, const std::string &description, const std::string &location,
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
    const std::chrono::time_point<std::chrono::system_clock> &getCreationTime() const;
    void setCreationTime(const std::chrono::time_point<std::chrono::system_clock> &creationTime);
    const std::chrono::time_point<std::chrono::system_clock> &getStartTime() const;
    void setStartTime(const std::chrono::time_point<std::chrono::system_clock> &startTime);
    const std::chrono::time_point<std::chrono::system_clock> &getEndTime() const;
    void setEndTime(const std::chrono::time_point<std::chrono::system_clock> &endTime);
};

#endif //POLICALENDAR_EVENT_H
