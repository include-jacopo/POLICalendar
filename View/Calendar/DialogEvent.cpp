#include <QMessageBox>
#include "DialogEvent.h"
#include "ui_DialogEvent.h"

DialogEvent::DialogEvent(QDate date, QWidget *parent) : QDialog(parent), ui(new Ui::DialogEvent) {
    ui->setupUi(this);
    mode = DialogMode::New;
    // Hide delete button if new event dialog
    ui->deleteButton->hide();

    controller = Controller::getInstance();
    setDateStart(QDateTime(date, QTime::currentTime()));
    setDateEnd(QDateTime(date, QTime::currentTime()).addSecs(3600));
}

DialogEvent::DialogEvent(const Event& event, QWidget *parent) : DialogEvent(QDate::currentDate(), parent) {
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

DialogEvent::~DialogEvent() {
    delete ui;
}

QString DialogEvent::getName() {
    return ui->fieldEventName->text();
}

QString DialogEvent::getLocation() {
    return ui->fieldLocation->text();
}
QString DialogEvent::getDescription() {
    return ui->fieldDescription->toPlainText();
}
QDateTime DialogEvent::getDateStart() {
    return ui->fieldDateStart->dateTime();
}
QDateTime DialogEvent::getDateEnd() {
    return ui->fieldDateEnd->dateTime();
}

void DialogEvent::setName(const QString& eventName) {
    ui->fieldEventName->setText(eventName);
}

void DialogEvent::setLocation(const QString& location) {
    ui->fieldLocation->setText(location);
}

void DialogEvent::setDescription(const QString& description) {
    ui->fieldDescription->setText(description);
}

void DialogEvent::setDateStart(const QDateTime& dateStart) {
    ui->fieldDateStart->setDateTime(dateStart);
}

void DialogEvent::setDateEnd(const QDateTime& dateEnd) {
    ui->fieldDateEnd->setDateTime(dateEnd);
}

void DialogEvent::accept() {
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
        bool statusOk;
        if (mode == DialogMode::New) {
            // Create a new event and send it to the controller, then update UI
            statusOk = controller->addEvent(getEvent());
            if (statusOk) emit eventCreated(getEvent());
        } else if (mode == DialogMode::Edit) {
            // Send the updated event to the controller and update the UI
            statusOk = controller->editEvent(getEvent());
            if (statusOk) emit eventEdited(getEvent());
        }
        QDialog::accept();
    }
}

void DialogEvent::deleteEvent() {
    bool statusOk = controller->deleteEvent(event.getUid());
    if (statusOk) emit eventDeleted(getEvent());
    done(2);
}

Event DialogEvent::getEvent() {
    event.setName(getName().toStdString());
    event.setLocation(getLocation().toStdString());
    event.setDescription(getDescription().toStdString());
    event.setStartTime(std::chrono::time_point<std::chrono::system_clock>(
            std::chrono::seconds(getDateStart().toUTC().toSecsSinceEpoch())));
    event.setEndTime(std::chrono::time_point<std::chrono::system_clock>(
            std::chrono::seconds(getDateEnd().toUTC().toSecsSinceEpoch())));
    event.setCreationTime(std::chrono::time_point<std::chrono::system_clock>(
            std::chrono::seconds(QDateTime::currentDateTimeUtc().toSecsSinceEpoch())));
    return event;
}
