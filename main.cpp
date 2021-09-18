#include <QApplication>
#include <QBoxLayout>
#include "View/Calendar/Calendar.h"
#include "View/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow mainW = MainWindow();
    mainW.show();
    return QApplication::exec();
}
