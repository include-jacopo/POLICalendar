#include "DialogEdit.h"
#include "ui_DialogEdit.h"

DialogEdit::DialogEdit(QWidget *parent) : QDialog(parent), ui(new Ui::DialogEdit) {
    ui->setupUi(this);
}

DialogEdit::DialogEdit(const Event& event, QWidget *parent) : DialogEdit(parent) {
    setName(QString::fromStdString(event.getName()));
    setDateStart(QDateTime::fromSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::seconds>(event.getStartTime().time_since_epoch()).count()
            ));
    setDateEnd(QDateTime::fromSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::seconds>(event.getEndTime().time_since_epoch()).count()
    ));
    setLocation(QString::fromStdString(event.getLocation()));
    setDescription(QString::fromStdString(event.getDescription()));
}

DialogEdit::~DialogEdit() {
    delete ui;
}

QString DialogEdit::getName() {
    return ui->fieldEventName->text();
}

QString DialogEdit::getLocation() {
    return ui->fieldLocation->text();
}
QString DialogEdit::getDescription() {
    return ui->fieldDescription->toPlainText();
}
QDateTime DialogEdit::getDateStart() {
    return ui->fieldDateStart->dateTime();
}
QDateTime DialogEdit::getDateEnd() {
    return ui->fieldDateEnd->dateTime();
}

void DialogEdit::setName(const QString& eventName) {
    ui->fieldEventName->setText(eventName);
}

void DialogEdit::setLocation(const QString& location) {
    ui->fieldLocation->setText(location);
}

void DialogEdit::setDescription(const QString& description) {
    ui->fieldDescription->setText(description);
}

void DialogEdit::setDateStart(const QDateTime& dateStart) {
    ui->fieldDateStart->setDateTime(dateStart);
}

void DialogEdit::setDateEnd(const QDateTime& dateEnd) {
    ui->fieldDateEnd->setDateTime(dateEnd);
}

