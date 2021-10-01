//
// Created by Riccardo Mengoli on 14/09/2021 17:50.
//

#include <QDir>
#include <QFile>
#include "CalendarEvents.h"

CalendarEvents::CalendarEvents(QDate date, ICalendarGUIEventsHandler *handler, QWidget *parent) : QFrame(parent) {
    CalendarEvents::date = date;
    this->handler = handler;
}

void CalendarEvents::setDate(const QDate &date) {
    CalendarEvents::date = date;
}

void CalendarEvents::addEvent(const Event &event) {
    auto calEv = new CalendarEvent(event, handler, this);
    calEv->show();
    setGeometryEvent(calEv, CalendarEvents::AddElement);
    events.push_back(calEv);
}

void CalendarEvents::removeEvent(const Event &event) {
    auto it = std::find_if(events.constBegin(), events.constEnd(), [&event](CalendarEvent *ce) {
        return ce->getEventUid() == QString::fromStdString(event.getUid());
    });
    if (it != events.constEnd()) {
        qsizetype i = it - events.constBegin();

        // Update geometry levels
        setGeometryEvent(events[i], CalendarEvents::RemoveElement);

        events[i]->deleteLater();
        events.remove(i);
    }
}

void CalendarEvents::clearEvents() {
    for (auto calEv: events) {
        calEv->deleteLater();
    }
    events.clear();
}


void CalendarEvents::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QColor::fromRgb(205, 205, 205));
    for (int i = 0; i < 24 + 1; ++i) {
        if (i != 24) {
            painter.drawLine(5, (int) (this->height() / 24.0 * i), this->width() - 5,
                             (int) (this->height() / 24.0 * i));
        } else {
            painter.drawLine(5, (int) (this->height() / 24.0 * i - 1), this->width() - 5,
                             (int) (this->height() / 24.0 * i - 1));
        }
    }
}

void CalendarEvents::resizeEvent(QResizeEvent *event) {
    for (auto calendarEvent: events) {
        setGeometryEvent(calendarEvent, CalendarEvents::Resize);
    }
}

QRect CalendarEvents::calculateCoords(CalendarEvent *e) {
    auto dtStart = std::max(e->getDateTimeStart(), date.startOfDay());
    auto dtEnd = std::min(e->getDateTimeEnd(), date.endOfDay());
    int sm, dur;
    sm = (int) (date.startOfDay().secsTo(dtStart) / 60);
    dur = (int) (dtStart.secsTo(dtEnd) / 60);

    auto tl = QPoint(e->geometry().x(), (int) (this->height() / 24.0 * sm / 60));
    auto br = QPoint(this->width() - 5, (int) (this->height() / 24.0 * (sm + dur) / 60));
    return QRect(tl, br);
}

void orderLayers(const QMultiMap<int, CalendarEvent *> &events) {
    for (auto it = events.constBegin(); it != events.constEnd(); it++) {
        // Raise to the top
        it.value()->raise();
    }
}

void CalendarEvents::setGeometryEvent(CalendarEvent *e, GeometryEventType type) {
    QRect geometry;
    QFile fileS;
    QMultiMap<int, CalendarEvent *> zIndexMap;

    // Handle overlapping events
    switch (type) {
        case Resize:
            e->setGeometry(calculateCoords(e));
            break;
        case AddElement:
            geometry = calculateCoords(e);
            geometry.setX(5);
            e->setProperty("zIndex", 0);
            for (auto ev: events) {
                int zIndex;
                bool IContain, WeIntercept;
                IContain = geometry.contains(ev->geometry());
                WeIntercept = ev->geometry().intersects(geometry);

                if (IContain) {
                    // Modify the other one
                    auto otherGeom = ev->geometry();
                    int maxX = std::min(otherGeom.x() + 30, this->width() - 40);
                    otherGeom.setX(maxX);
                    ev->setGeometry(otherGeom);

                    // Update zIndex for colors
                    zIndex = ev->property("zIndex").toInt();
                    ev->setProperty("zIndex", zIndex + 1);

                } else if (WeIntercept) {
                    // Modify myself
                    int maxX = std::min(geometry.x() + 30, this->width() - 40);
                    geometry.setX(maxX);

                    // Update my zIndex for colors
                    zIndex = e->property("zIndex").toInt();
                    e->setProperty("zIndex", zIndex + 1);
                }
            }

            // Reorder zIndex layers
            zIndexMap.insert(e->property("zIndex").toInt(), e);
            for (auto ev: events) {
                zIndexMap.insert(ev->property("zIndex").toInt(), ev);
            }
            orderLayers(zIndexMap);

            // Reapply stylesheet
            fileS.setFileName(":/Calendar/EventColors.qss");
            fileS.open(QFile::ReadOnly);
            setStyleSheet(fileS.readAll());

            e->setGeometry(geometry);
            break;
        case RemoveElement:
            geometry = e->geometry();
            for (auto ev: events) {
                if (ev == e) continue;
                if (geometry.contains(ev->geometry())) {
                    // Modify the other one
                    auto otherGeom = ev->geometry();
                    int minX = std::max(otherGeom.x() - 30, 5);
                    otherGeom.setX(minX);
                    ev->setGeometry(otherGeom);

                    // Update zIndex for colors
                    auto zIndex = ev->property("zIndex").toInt();
                    ev->setProperty("zIndex", zIndex - 1);
                }
            }

            // Reapply stylesheet
            fileS.setFileName(":/Calendar/EventColors.qss");
            fileS.open(QFile::ReadOnly);
            setStyleSheet(fileS.readAll());
            break;
    }
}