#include <QApplication>
#include "View/MainWindow.h"

int main(int argc, char *argv[]) {
    // GUI
    QApplication a(argc, argv);
    MainWindow mainW = MainWindow();
    mainW.show();
    return QApplication::exec();
}
