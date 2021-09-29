//
// Created by Riccardo Mengoli on 29/09/2021 12:47.
//

#ifndef POLICALENDAR_ICALENDARGUITASKHANDLER_H
#define POLICALENDAR_ICALENDARGUITASKHANDLER_H

class ICalendarGUITaskHandler {
public:
    virtual void createTaskDialog() = 0;
    virtual void editTaskDialog(const Task &task) = 0;

protected:
    virtual void addTask(const Task &task) = 0;
    virtual void editTask(const Task &task) = 0;
    virtual void removeTask(const Task &task) = 0;
};

#endif //POLICALENDAR_ICALENDARGUITASKHANDLER_H
