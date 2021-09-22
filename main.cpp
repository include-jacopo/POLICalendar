#include <QApplication>
#include "View/MainWindow.h"
#include "Controller/Controller.h"

int main(int argc, char *argv[]) {
    // Controller
    Controller controller = Controller();
    // GUI
    QApplication a(argc, argv);
    MainWindow mainW = MainWindow();

    controller.setView(&mainW);
    mainW.setController(&controller);

    mainW.show();
    return QApplication::exec();
}
