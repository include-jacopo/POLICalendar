#ifndef DIALOGEVENT_H
#define DIALOGEVENT_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QDate>
#include <QDateTime>
#include "../../Model/Event.h"
#include "../../Controller/Controller.h"

namespace Ui {
    class DialogEvent;
}

class DialogEvent : public QDialog {
    Q_OBJECT

public:
    explicit DialogEvent(QDate date, QWidget *parent = nullptr);
    explicit DialogEvent(const Event& event, QWidget* parent = nullptr);
    ~DialogEvent() override;

    Event getEvent();

public slots:
    void accept() override;
    void deleteEvent();

signals:
    void eventCreated(Event evCreated);
    void eventEdited(Event evUpdated);
    void eventDeleted(Event evDeleted);

private:
    Ui::DialogEvent *ui;

    Event event;
    Controller *controller;
    enum DialogMode {New, Edit};
    DialogMode mode;

    QString getName();
    QString getLocation();
    QString getDescription();
    /** Gets the starting date GUI field value
    * @return Start date as local time
    */
    QDateTime getDateStart();
    /** Gets the ending date GUI field value
    * @return End date as local time
    */
    QDateTime getDateEnd();
    void setName(const QString& eventName);
    void setLocation(const QString& location);
    void setDescription(const QString& description);
    /** Sets the starting date GUI field value
    * @param dateStart Start date as local time
    */
    void setDateStart(const QDateTime& dateStart);
    /** Sets the ending date GUI field value
    * @param dateStart End date as local time
    */
    void setDateEnd(const QDateTime& dateEnd);
};

#endif // DIALOGEVENT_H
