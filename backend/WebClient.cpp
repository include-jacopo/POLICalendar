//
// Created by Jacopo on 15/09/21.
//

#include "WebClient.h"
#include <neon/ne_session.h>
#include <neon/ne_request.h>
#include <neon/ne_207.h>
#include <neon/ne_utils.h>
#include <neon/ne_uri.h>
#include <neon/ne_props.h>
#include <iostream>
#include <string>
#include "Base64.h"


using namespace std;

int httpResponseReader(void *userdata, const char *buf, size_t len)
{
    string *str = (string *)userdata;
    str->append(buf, len);
    return 0;
}

WebClient::WebClient(const string url, const string user, const string pass, const unsigned port){
    base64_auth = Base64::Encode(user+":"+pass); //base 64 encoding di username e password
    ne_sock_init();
    sess = ne_session_create("https", url.c_str(), port);
    ne_ssl_trust_default_ca(sess);
}

WebClient::~WebClient(){
    ne_session_destroy(sess);
    ne_sock_exit();
}

string WebClient::do_propfind(std::string uri) {
    string response;
    string report = "<c:calendar-query xmlns:d=\"DAV:\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\n"
                    "    <d:prop>\n"
                    "        <d:getetag />\n"
                    "        <c:calendar-data />\n"
                    "    </d:prop>\n"
                    "    <c:filter>\n"
                    "        <c:comp-filter name=\"VCALENDAR\" />\n"
                    "    </c:filter>\n"
                    "</c:calendar-query>";

    ne_request *req = ne_request_create(sess, "REPORT", uri.c_str());
    ne_add_request_header(req, "Authorization", ("Basic "+base64_auth).c_str());
    ne_add_request_header(req, "Depth", "1");

    ne_set_request_body_buffer(req, report.c_str(), report.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    int status = ne_get_status(req)->code; //aggiungere un controllo su questi return?

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
    ne_request_destroy(req);
    return response;
}