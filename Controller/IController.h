//
// Created by Riccardo Mengoli on 22/09/2021 18:28.
//

#ifndef POLICALENDAR_ICONTROLLER_H
#define POLICALENDAR_ICONTROLLER_H

using namespace std;

class IController {
public:
    /*
     * Get the iterators to read all the events.
     * @return std::pair<>(begin_it, end_it)
     */
    virtual pair<forward_iterator_tag, forward_iterator_tag> getEvents() = 0;

    /*
     * Update events.
     * @return true if request completed, false if error
     */
    virtual bool updateEvents() = 0;

    /*
     * Add an event to the calendar.
     * @return true if event added, false if fail
     */
    virtual bool addEvent(Event ev) = 0;

    /*
     * Search for an event based on its unique uid.
     * @return optional<Event>
     */
    virtual optional<Event> findEvent(string uid) = 0;

    /*
     * Delete an event.
     * @return true if event deleted, false if fail
     */
    virtual bool deleteEvent(string uid) = 0;
};

#endif //POLICALENDAR_ICONTROLLER_H
