#ifndef DIALOGTASK_H
#define DIALOGTASK_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QDateTime>
#include "../../Model/Task.h"
#include "../../Controller/Controller.h"

namespace Ui {
    class DialogTask;
}

class DialogTask : public QDialog {
Q_OBJECT

public:
    explicit DialogTask(QWidget *parent = nullptr);
    explicit DialogTask(const Task &event, QWidget *parent = nullptr);
    ~DialogTask() override;

    Task getTask();

public slots:
    void accept() override;
    void deleteTask();

signals:
    void taskCreated(const Task& task);
    void taskEdited(const Task& task);
    void taskDeleted(const Task& task);

private:
    Ui::DialogTask *ui;

    Task task;
    Controller *controller;
    enum DialogMode {New, Edit};
    DialogMode mode;

    QString getName();
    QString getLocation();
    QString getDescription();
    bool getDueDateEnabled();
    QDateTime getDueDate();
    int getPriority();
    bool getCompleted();
    void setName(const QString& taskName);
    void setLocation(const QString& location);
    void setDescription(const QString& description);
    Q_SLOT void setDueDateEnabled(int val);
    void setDueDate(const QDateTime& dueDate);
    void setPriority(int priority);
    void setCompleted(bool completed);
};

#endif // DIALOGTASK_H
