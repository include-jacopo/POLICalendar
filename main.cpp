#include <QApplication>
#include <QBoxLayout>
#include "Calendar.h"
#include "MainWindow.h"
#include <libical/ical.h>
#include <neon/ne_utils.h>
#include <iostream>
#include "backend/session.h"
#include "backend/WebClient.h"

int main(int argc, char *argv[]) {
    const std::string host("dav.fruux.com");
    const std::string user("b3297398995");
    const std::string pass("dap2zg5z54tu");
    const std::string uri("/calendars/a3298160768/51759490-6b14-4c41-88ae-1a94106fe0b6/");
    const unsigned port = 443; //443

    //const std::string uri("/principals/uid/a3298160768/");

    WebClient cal(host, user, pass, port);

    //cal.ls("/Users/jacopo/Desktop");

    cal.do_propfind(uri);


    /*
    icalcomponent *event; //prova di utilizzo
    icalproperty *desc;
    icalvalue *val;
    event = icalcomponent_new(ICAL_VEVENT_COMPONENT);
    desc = icalproperty_new(ICAL_DESCRIPTION_PROPERTY);
    val = icalvalue_new_string("AUAUAU");
    icalproperty_set_value(desc, val);
    icalcomponent_add_property(event, desc);
    std::cout << icalcomponent_get_description(event);
    icalvalue_free(val);
    icalproperty_free(desc);
    icalcomponent_free(event);

    QApplication a(argc, argv);
    MainWindow mainW = MainWindow();
    mainW.show();
    return QApplication::exec();
    */
    return 0;
}
