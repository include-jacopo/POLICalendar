//
// Created by Jacopo on 15/09/21.
//

#include "session.h"
#include <neon/ne_session.h>
#include <neon/ne_request.h>
#include <neon/ne_207.h>
#include <neon/ne_utils.h>
#include <neon/ne_uri.h>
#include <iostream>
using namespace std;

int cbReader(void *userdata, const char *buf, size_t len){
    //__PRINTFUNC__;

    ne_xml_parser *parser = (ne_xml_parser *)userdata;
    ne_xml_parse(parser, buf, len);

    return 0;
}

int httpResponseReader(void *userdata, const char *buf, size_t len)
{
    string *str = (string *)userdata;
    str->append(buf, len);
    return 0;
}

int new_session() {
    ne_session *sess;
    sess = ne_session_create("http", "www.google.com", 80);
    string response;

    ne_request *req = ne_request_create(sess, "GET", "/");
    //returns a pointer to a request object

    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    int status = ne_get_status(req)->code;

    ne_request_destroy(req);

    string errorMessage = ne_get_error(sess);
    ne_session_destroy(sess);

    printf("result %d, status %d\n", result, status);
    cout << response << "\n";

    switch (result) {
        case NE_OK:
            break;
        case NE_CONNECT:
            throw invalid_argument("ne_connect error");
        case NE_TIMEOUT:
            throw invalid_argument("ne_timeout error");
        case NE_AUTH:
            throw invalid_argument("ne_auth error");
        default:
            throw invalid_argument("ne_generic error");
    }

    return 1;
}
