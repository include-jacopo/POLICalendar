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
    /** Datetimes are always UTC Time */
    chrono::time_point<std::chrono::system_clock> dueDate, dateCompleted, dateS;
    bool completed;
    /** Flag che quando settato a true segnala la presenza della data opzionale */
    bool flagDate;


    std::string gen_random(std::string s, int len);
    std::string createUid();

public:
    Task();
    /*
    Task(string uid, string name, string description, string location, string etag, int priority, bool completed,
         chrono::time_point<std::chrono::system_clock> date, chrono::time_point<std::chrono::system_clock> dateCompl); /* costruttore per i task senza data
    */
    Task(string uid, string name, string description, string location, string etag, int priority, bool completed, bool flagDate,
         chrono::time_point<std::chrono::system_clock> date, chrono::time_point<std::chrono::system_clock> dateS, chrono::time_point<std::chrono::system_clock> dateCompl);

    const string &getUid() const;
    void setUidS(const string &uidS);
    const string &getName() const;
    void setName(const string &name);
    const string &getDescription() const;
    void setDescription(const string &description);
    const string &getLocation() const;
    void setLocation(const string &location);
    int getPriority() const;
    void setPriority(int priority);
    const string &getEtag() const;
    void setEtag(const string &etag);
    /** Gets the Task due datetime
     * @return Due datetime UTC time
     */
    const chrono::time_point<std::chrono::system_clock> &getDueDate() const;
    /** Sets the Task due datetime
     * @param dueDate Due datetime UTC time
     */
    void setDueDate(const chrono::time_point<std::chrono::system_clock> &date);
    const chrono::time_point<std::chrono::system_clock> &getDateCompleted() const;
    void setDateCompleted(const chrono::time_point<std::chrono::system_clock> &dateCompleted);
    const std::chrono::time_point<std::chrono::system_clock> &getDateS() const;
    void setDateS(const chrono::time_point<std::chrono::system_clock> &date);
    bool isFlagDate() const;
    void setFlagDate(bool flagDate);
    bool isCompleted() const;
    void setCompleted(bool completed);

    void printTask();
};


#endif //POLICALENDAR_TASK_H
