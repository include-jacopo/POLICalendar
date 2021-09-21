#ifndef DIALOGEDIT_H
#define DIALOGEDIT_H

#include <QDialog>
#include "../../Model/Event.h"

namespace Ui {
class DialogEdit;
}

class DialogEdit : public QDialog {
    Q_OBJECT

public:
    explicit DialogEdit(QWidget *parent = nullptr);
    DialogEdit(const Event& event, QWidget* parent = nullptr);
    ~DialogEdit();

private:
    Ui::DialogEdit *ui;

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