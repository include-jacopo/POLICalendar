//
// Created by Jacopo on 15/09/21.
//

#include "session.h"
#include <neon/ne_session.h>
#include <neon/ne_request.h>
#include <iostream>
#include <neon/ne_207.h>
using namespace std;

int cbReader(void *userdata, const char *buf, size_t len){
    //__PRINTFUNC__;

    ne_xml_parser *parser = (ne_xml_parser *)userdata;
    ne_xml_parse(parser, buf, len);

    return 0;
}

int new_session() {
    ne_session *sess;
    sess = ne_session_create("http", "www.google.com", 80);

    ne_request *req = ne_request_create(sess, "GET", "");
    //returns a pointer to a request object

    if (ne_request_dispatch(req)) {
        cout << "Request failed" << ne_get_error(sess);
    } else {
        cout << "ok";
    }

    return 1;
}
