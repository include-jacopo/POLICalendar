#ifndef DIALOGEDIT_H
#define DIALOGEDIT_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QDateTime>
#include "../../Model/Event.h"
#include "../../Controller/Controller.h"

namespace Ui {
class DialogEdit;
}

class DialogEdit : public QDialog {
    Q_OBJECT

public:
    explicit DialogEdit(QWidget *parent = nullptr);
    explicit DialogEdit(const Event& event, QWidget* parent = nullptr);
    ~DialogEdit() override;

    Event getEvent();

public slots:
    void accept() override;
    void deleteEvent();

signals:
    void eventCreated(Event evCreated);
    void eventEdited(Event evUpdated);
    void eventDeleted(Event evDeleted);

private:
    Ui::DialogEdit *ui;

    Event event;
    Controller *controller;
    enum DialogMode {New, Edit};
    DialogMode mode;

    QString getName();
    QString getLocation();
    QString getDescription();
    /** Gets the starting date GUI field value
    * @return Start date as UTC time
    */
    QDateTime getDateStart();
    /** Gets the ending date GUI field value
    * @return End date as UTC time
    */
    QDateTime getDateEnd();
    void setName(const QString& eventName);
    void setLocation(const QString& location);
    void setDescription(const QString& description);
    /** Sets the starting date GUI field value
    * @param dateStart Start date as UTC time
    */
    void setDateStart(const QDateTime& dateStart);
    /** Sets the ending date GUI field value
    * @param dateStart End date as UTC time
    */
    void setDateEnd(const QDateTime& dateEnd);
};

#endif // DIALOGEDIT_H
