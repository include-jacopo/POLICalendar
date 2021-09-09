#include <QApplication>
#include <QMainWindow>
#include <QBoxLayout>
#include "Calendar.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // Componente main window, diviso in 5 sezioni
    QMainWindow mainW = QMainWindow();

    // Calendar raggruppa le CalendarColumn
    auto calendar = new Calendar();
    //calendar->setLayout(new QHBoxLayout());

    mainW.setCentralWidget(calendar);
    mainW.show();
    return QApplication::exec();
}
