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
    void eventCreated(const Event& evCreated);
    void eventEdited(const Event& evUpdated);
    void eventDeleted(const Event& evDeleted);

private:
    Ui::DialogEdit *ui;

    Event event;
    Controller *controller;
    enum DialogMode {New, Edit};
    DialogMode mode;

    QString getName();
    QString getLocation();
    QString getDescription();
    QDateTime getDateStart();
    QDateTime getDateEnd();
    void setName(const QString& eventName);
    void setLocation(const QString& location);
    void setDescription(const QString& description);
    void setDateStart(const QDateTime& dateStart);
    void setDateEnd(const QDateTime& dateEnd);
};

#endif // DIALOGEDIT_H
