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
    string uidS, name, description, location;
    int priority;
    /** Datetime is always UTC Time */
    chrono::time_point<std::chrono::system_clock> date;
    bool completed;
    /** Flag che quando settato a true segnala la presenza della data opzionale */
    bool flagDate;
public:
    Task();
    Task(string uid, string name, string description, string location, int priority, bool completed); /* costruttore per i task senza data */
    Task(string uid, string name, string description, string location, int priority, bool completed,
         chrono::time_point<std::chrono::system_clock> date);

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
    /** Gets the Task due datetime
     * @return Due datetime UTC time
     */
    const chrono::time_point<std::chrono::system_clock> &getDate() const;
    /** Sets the Task due datetime
     * @param date Due datetime UTC time
     */
    void setDate(const chrono::time_point<std::chrono::system_clock> &date);
    bool isFlagDate() const;
    void setFlagDate(bool flagDate);
    bool isCompleted() const;
    void setCompleted(bool completed);
};


#endif //POLICALENDAR_TASK_H
