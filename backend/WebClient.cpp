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
#include <random>
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

string WebClient::report_calendar(string uri) {
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
    return move(response);
}

string WebClient::report_todo(string uri) {
    string response;
    string report = "<c:calendar-query xmlns:d=\"DAV:\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\n"
                    "    <d:prop>\n"
                    "        <d:getetag />\n"
                    "        <c:calendar-data />\n"
                    "    </d:prop>\n"
                    "    <c:filter>\n"
                    "        <c:comp-filter name=\"VCALENDAR\">\n"
                    "            <c:comp-filter name=\"VTODO\" />\n"
                    "        </c:comp-filter>\n"
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
    return move(response);
}

int WebClient::put_event(string uri, string evento) {
    string response;

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<long> random(100000000000000, 199999999999999);
    string random_number = to_string(random(gen));

    ne_request *req = ne_request_create(sess, "PUT", (uri+random_number+".ics").c_str());
    ne_add_request_header(req, "Authorization", ("Basic "+base64_auth).c_str());

    ne_set_request_body_buffer(req, evento.c_str(), evento.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int status = ne_get_status(req)->code;

    cout << response;
    cout << status;

    if(status != 201) {
        cout << "\nERROR IN THE PUT METHOD\n" << response;
        ne_request_destroy(req);
        return 1;
    }
    ne_request_destroy(req);
    return 0;
}