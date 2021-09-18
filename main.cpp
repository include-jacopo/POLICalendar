#include <QApplication>
#include <QBoxLayout>
#include "Calendar.h"
#include "MainWindow.h"
#include <libical/ical.h>
#include <neon/ne_utils.h>
#include <iostream>
#include "backend/WebClient.h"
#include <pugixml.hpp>
#include <sstream>
using namespace std;

int main(int argc, char *argv[]) {
    const std::string host("dav.fruux.com");
    const std::string user("b3297398995");
    const std::string pass("dap2zg5z54tu");
    const std::string uri("/calendars/a3298160768/51759490-6b14-4c41-88ae-1a94106fe0b6/"); //solo per calendario
    //const std::string uri("/principals/uid/a3298160768/");
    const unsigned port = 443; //443
    string xml_cal;

    WebClient cal(host, user, pass, port);
    xml_cal = cal.do_propfind(uri);
    cout << xml_cal;

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


    pugi::xml_document doc;
    std::stringstream ss;
    ss<<xml_cal;
    pugi::xml_parse_result result = doc.load(ss, pugi::parse_default|pugi::parse_declaration);
    //if (!doc.load("<node id='123'>text</node><!-- comment -->", pugi::parse_default | pugi::parse_comments)) return -1;

    if(!result){
        std::cout << "Parse error: " << result.description()<< ", character pos= " << result.offset;
    }

   vector<string> v;
    for(auto node: doc.child("d:multistatus").children()){
        /*
        for(auto node2: doc.child("d:multistatus").child("d:response").child("d:propstat").child("d:prop").child("cal:calendar-data")){
            std::cout<<node2.name()<<": "<<node2.text().as_string()<<endl;
        }
          */

        for( auto node2: node.child("d:propstat").child("d:prop").child("cal:calendar-data")) {
            //std::cout << node2.name() << ": " << node2.text().as_string() << endl;
            cout<<node2.text().as_string()<<endl;
            v.push_back(node2.text().as_string());

        }
        }


    return 0;
}
