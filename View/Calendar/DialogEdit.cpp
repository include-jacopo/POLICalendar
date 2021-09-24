#include <QMessageBox>
#include "DialogEdit.h"
#include "ui_DialogEdit.h"

DialogEdit::DialogEdit(QWidget *parent) : QDialog(parent), ui(new Ui::DialogEdit) {
    ui->setupUi(this);
    mode = DialogMode::New;
    // Hide delete button if new event dialog
    ui->deleteButton->hide();

    controller = Controller::getInstance();
    setDateStart(QDateTime::currentDateTime());
    setDateEnd(QDateTime::currentDateTime());
}

DialogEdit::DialogEdit(const Event& event, QWidget *parent) : DialogEdit(parent) {
    mode = DialogMode::Edit;
    this->event = event;
    setName(QString::fromStdString(event.getName()));
    setDateStart(QDateTime::fromSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::seconds>(event.getStartTime().time_since_epoch()).count()
            ));
    setDateEnd(QDateTime::fromSecsSinceEpoch(
            std::chrono::duration_cast<std::chrono::seconds>(event.getEndTime().time_since_epoch()).count()
    ));
    setLocation(QString::fromStdString(event.getLocation()));
    setDescription(QString::fromStdString(event.getDescription()));
    // Show delete button
    ui->deleteButton->show();
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

void DialogEdit::accept() {
    QList<QString> errors;
    if (getName().isEmpty()) {
        errors.push_back(QString("Inserire il nome dell'evento"));
    }
    if (getDateEnd() < getDateStart()) {
        errors.push_back(QString("Data di fine precedente data di inizio"));
    } else if (getDateEnd() == getDateStart()) {
        errors.push_back(QString("Data di fine coincidente con data di inizio"));
    }

    if (!errors.isEmpty()) {
        QString strErr;
        for (const auto& err : errors) {
            strErr.append(err + "\n");
        }
        QMessageBox::warning(this, "Errore", strErr);
    } else {
        if (mode == DialogMode::New) {
            // Create a new event and send it to the controller
            controller->addEvent(getEvent());
        } else if (mode == DialogMode::Edit) {
            controller->editEvent(getEvent());
        }
        QDialog::accept();
    }
}

void DialogEdit::deleteEvent() {
    controller->deleteEvent(event.getUid());
}

Event DialogEdit::getEvent() {
    event.setName(getName().toStdString());
    event.setLocation(getLocation().toStdString());
    event.setDescription(getDescription().toStdString());
    event.setStartTime(std::chrono::time_point<std::chrono::system_clock>(
            std::chrono::seconds(getDateStart().toSecsSinceEpoch())));
    event.setEndTime(std::chrono::time_point<std::chrono::system_clock>(
            std::chrono::seconds(getDateEnd().toSecsSinceEpoch())));
    return event;
}
