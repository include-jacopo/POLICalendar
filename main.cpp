#include <QApplication>
#include <QBoxLayout>
#include "Calendar.h"
#include "MainWindow.h"
#include <libical/ical.h>

int main(int argc, char *argv[]) {
    icalcomponent *event; //prova di utilizzo

    QApplication a(argc, argv);
    MainWindow mainW = MainWindow();
    mainW.show();
    return QApplication::exec();
}
