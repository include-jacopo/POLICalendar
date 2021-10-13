//
// Created by michele on 9/25/21.
//

#ifndef POLICALENDAR_TASK_H
#define POLICALENDAR_TASK_H

#include <string>
#include <chrono>

using namespace std;

class Task {
private:
    string uidS, name, description, location, etag;
    int priority;
    chrono::time_point<std::chrono::system_clock> dueDate, dateCompleted, dateS;  /*dateS è il campo dtstamp */
    bool completed;
    /* Flag che quando settato a true segnala la presenza della data opzionale */
    bool flagDate;


    std::string gen_random(std::string s, int len);
    std::string createUid();

public:
    /* constructors */
    Task();
    Task(string uid, string name, string description, string location, string etag, int priority, bool completed, bool flagDate,
         chrono::time_point<std::chrono::system_clock> date, chrono::time_point<std::chrono::system_clock> dateS, chrono::time_point<std::chrono::system_clock> dateCompl);

    /* getter & setter */
    [[nodiscard]] const string &getUid() const;
    void setUidS(const string &uidS);
    [[nodiscard]] const string &getName() const;
    void setName(const string &name);
    [[nodiscard]] const string &getDescription() const;
    void setDescription(const string &description);
    [[nodiscard]] const string &getLocation() const;
    void setLocation(const string &location);
    [[nodiscard]] int getPriority() const;
    void setPriority(int priority);
    [[nodiscard]] const string &getEtag() const;
    void setEtag(const string &etag);
    [[nodiscard]] const chrono::time_point<std::chrono::system_clock> &getDueDate() const;
    void setDueDate(const chrono::time_point<std::chrono::system_clock> &date);
    [[nodiscard]] const chrono::time_point<std::chrono::system_clock> &getDateCompleted() const;
    void setDateCompleted(const chrono::time_point<std::chrono::system_clock> &dateCompleted);
    [[nodiscard]] const std::chrono::time_point<std::chrono::system_clock> &getDateS() const;
    void setDateS(const chrono::time_point<std::chrono::system_clock> &date);
    [[nodiscard]] bool isFlagDate() const;
    void setFlagDate(bool flagDate);
    [[nodiscard]] bool isCompleted() const;
    void setCompleted(bool completed);
    void printTask();
};


#endif //POLICALENDAR_TASK_H
