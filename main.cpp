#include <QApplication>
#include <QBoxLayout>
#include "Calendar.h"
#include "MainWindow.h"
#include <libical/ical.h>
#include <neon/ne_utils.h>
#include <iostream>

int main(int argc, char *argv[]) {
    icalcomponent *event; //prova di utilizzo
    icalproperty *desc;
    icalvalue *val;
    vent = icalcomponent_new(ICAL_VEVENT_COMPONENT);
    desc = icalproperty_new(ICAL_DESCRIPTION_PROPERTY);
    val = icalvalue_new_string("AUAUAU");
    icalproperty_set_value(desc, val);
    icalcomponent_add_property(event, desc);
    std::cout << icalcomponent_get_description(event);
    icalvalue_free(val);
    icalproperty_fr
    eee(desc);
    icalcomponent_free(event);

    QApplication a(argc, argv);
    MainWindow mainW = MainWindow();
    mainW.show();
    return QApplication::exec();
}
