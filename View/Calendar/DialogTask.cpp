#include <QMessageBox>
#include "DialogTask.h"
#include "ui_DialogTask.h"

DialogTask::DialogTask(QWidget *parent) : QDialog(parent), ui(new Ui::DialogTask) {
    ui->setupUi(this);
    mode = DialogMode::New;
    // Hide delete button if new task dialog
    ui->deleteButton->hide();

    controller = Controller::getInstance();
    setDueDate(QDateTime::currentDateTime());
}

DialogTask::DialogTask(const Task &task, QWidget *parent) : DialogTask(parent) {
    mode = DialogMode::Edit;
    this->task = task;
    setName(QString::fromStdString(task.getName()));
    if (task.isFlagDate()) {
        ui->enableDueDate->setChecked(true);
        setDueDate(QDateTime::fromSecsSinceEpoch(
                std::chrono::duration_cast<std::chrono::seconds>(task.getDueDate().time_since_epoch()).count()));
    }
    setLocation(QString::fromStdString(task.getLocation()));
    setDescription(QString::fromStdString(task.getDescription()));
    setPriority(task.getPriority());
    setCompleted(task.isCompleted());
    // Show delete button
    ui->deleteButton->show();
}

DialogTask::~DialogTask() {
    delete ui;
}

QString DialogTask::getName() {
    return ui->fieldTaskName->text();
}

QString DialogTask::getLocation() {
    return ui->fieldLocation->text();
}

QString DialogTask::getDescription() {
    return ui->fieldDescription->toPlainText();
}

bool DialogTask::getDueDateEnabled() {
    return ui->enableDueDate->isChecked();
}

QDateTime DialogTask::getDueDate() {
    return ui->fieldDueDate->dateTime();
}

int DialogTask::getPriority() {
    int sliderVal = ui->prioritySlider->value();
    if (sliderVal == 1) {
        return 9; // Low priority
    } else if (sliderVal == 2) {
        return 5; // Middle priority
    } else {
        return 1; // High priority
    }
}

bool DialogTask::getCompleted() {
    return ui->taskCompleted->isChecked();
}

void DialogTask::setName(const QString &taskName) {
    ui->fieldTaskName->setText(taskName);
}

void DialogTask::setLocation(const QString &location) {
    ui->fieldLocation->setText(location);
}

void DialogTask::setDescription(const QString &description) {
    ui->fieldDescription->setText(description);
}

void DialogTask::setDueDateEnabled(int val) {
    ui->fieldDueDate->setEnabled((Qt::CheckState) val);
}

void DialogTask::setDueDate(const QDateTime &dueDate) {
    ui->fieldDueDate->setDateTime(dueDate);
}

void DialogTask::setPriority(int priority) {
    int sliderVal;
    if (priority == 0 || priority > 5) {
        sliderVal = 1; // Low priority
    } else if (priority == 5) {
        sliderVal = 2; // Middle priority
    } else {
        sliderVal = 3; // High priority
    }
    ui->prioritySlider->setValue(sliderVal);
}

void DialogTask::setCompleted(bool completed) {
    ui->taskCompleted->setChecked(completed);
}

void DialogTask::accept() {
    if (getName().isEmpty()) {
        QMessageBox::warning(this, "Errore", QString("Inserire il nome del task"));
    } else {
        bool statusOk;
        if (mode == DialogMode::New) {
            // Create a new task and send it to the controller, then update UI
            statusOk = controller->addTask(getTask());
            if (statusOk) emit taskCreated(getTask());
        } else if (mode == DialogMode::Edit) {
            // Send the updated task to the controller and update the UI
            statusOk = controller->editTask(getTask());
            if (statusOk) emit taskEdited(getTask());
        }
    }
}

void DialogTask::deleteTask() {
    bool statusOk = controller->deleteTask(task.getUid());
    if (statusOk) emit taskDeleted(getTask());
    done(2);
}

Task DialogTask::getTask() {
    task.setName(getName().toStdString());
    task.setLocation(getLocation().toStdString());
    task.setDescription(getDescription().toStdString());
    task.setFlagDate(getDueDateEnabled());
    task.setDueDate(std::chrono::time_point<std::chrono::system_clock>(
            std::chrono::seconds(getDueDate().toUTC().toSecsSinceEpoch())));
    task.setPriority(getPriority());
    // Add completion date if old task was not completed, and now it is
    if (!task.isCompleted() && getCompleted()) {
        task.setDateCompleted(std::chrono::time_point<std::chrono::system_clock>(
                std::chrono::seconds(QDateTime::currentDateTimeUtc().toSecsSinceEpoch())));
    }
    task.setCompleted(getCompleted());
    return task;
}