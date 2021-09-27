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
#include "XMLReader.h"
using namespace std;

int httpResponseReader(void *userdata, const char *buf, size_t len)
{
    string *str = (string *)userdata;
    str->append(buf, len);
    return 0;
}

WebClient::WebClient() {
}

WebClient::~WebClient(){
    ne_session_destroy(sess);
    ne_sock_exit();
}

string WebClient::getUrl(){
    return this->url;
};
int WebClient::getPort(){
    return this->port;
};

void WebClient::setClient(const string url, const string user, const string pass, int port) {
    this->port = port;
    this->url = url;
    base64_auth = Base64::Encode(user + ":" + pass); //base 64 encoding di username e password
    ne_sock_init();
    sess = ne_session_create("https", url.c_str(), port);
    ne_ssl_trust_default_ca(sess);
}

void WebClient::setUri(string strCalendar, string strTodo) {
    this->uri_calendar = strCalendar;
    this->uri_todo = strTodo;
}

string WebClient::getUriCalendar(){
    return this->uri_calendar;
}

string WebClient::getUriTodo(){
    return this->uri_todo;
}

void WebClient::propfindUri(){
    //Per prima cosa necessito dell'url specifico del nostro utente
    string response;
    string propfind_link_user = "<d:propfind xmlns:d=\"DAV:\">\n"
                                "  <d:prop>\n"
                                "     <d:current-user-principal />\n"
                                "  </d:prop>\n"
                                "</d:propfind>";

    string url_prop = "https://" + getUrl() + "/";

    ne_request *req = ne_request_create(sess, "PROPFIND", (url_prop).c_str());
    ne_add_request_header(req, "Authorization", ("Basic "+base64_auth).c_str());

    ne_set_request_body_buffer(req, propfind_link_user.c_str(), propfind_link_user.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    ne_request_dispatch(req);
    ne_request_destroy(req);
    string link_user = readLinkUser(response); //ottenuto l'xml, pesco solo l'url che mi interessa

    //Effettuo quindi una nuova propfind verso tale url per ricevere il link alla sua collezione di calendari
    string propfind_calendar_collection = "<d:propfind xmlns:d=\"DAV:\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\n"
                                          "  <d:prop>\n"
                                          "     <c:calendar-home-set />\n"
                                          "  </d:prop>\n"
                                          "</d:propfind>";

    url_prop = "https://" + getUrl() + link_user; //modifico l'url della richiesta verso l'utente specifico

    req = ne_request_create(sess, "PROPFIND", (url_prop).c_str());
    ne_add_request_header(req, "Authorization", ("Basic "+base64_auth).c_str());

    string response1;
    ne_set_request_body_buffer(req, propfind_calendar_collection.c_str(), propfind_calendar_collection.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response1);

    ne_request_dispatch(req);
    ne_request_destroy(req);

    string calendar_collection = readCalendarCollection(response1); //ottenuto l'xml, pesco solo l'url che mi interessa

    //Effettuo l'ultima propfind verso la sua collezione di calendari per ricevere l'uri sia del calendario che del to-do
    string propfind_calendar_user = "<d:propfind xmlns:d=\"DAV:\" xmlns:cs=\"http://calendarserver.org/ns/\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\n"
                                    "  <d:prop>\n"
                                    "     <d:resourcetype />\n"
                                    "     <d:displayname />\n"
                                    "     <cs:getctag />\n"
                                    "     <c:supported-calendar-component-set />\n"
                                    "  </d:prop>\n"
                                    "</d:propfind>";

    url_prop = "https://" + getUrl() + calendar_collection; //modifico l'url della richiesta il calendario dell'utente

    req = ne_request_create(sess, "PROPFIND", (url_prop).c_str());
    ne_add_request_header(req, "Authorization", ("Basic "+base64_auth).c_str());
    ne_add_request_header(req, "Depth", "1");

    string response2;
    ne_set_request_body_buffer(req, propfind_calendar_user.c_str(), propfind_calendar_user.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response2);

    ne_request_dispatch(req);
    ne_request_destroy(req);

    string uri_calendar = readUriCalendar(response2); //ottenuto l'xml, pesco solo l'uri del calendario
    string uri_todo = readUriTodo(response2); //pesco anche l'uri dei to-do

    setUri(uri_calendar, uri_todo); //salvo l'uri del calendario nella struttura dati
}

void WebClient::setCtag(string ctag) {
    this->ctag = ctag;
}

string WebClient::getCtag() {
    return this->ctag;
}

string WebClient::propfindCtag(string uri) {
    string response;
    string propfind = "<d:propfind xmlns:d=\"DAV:\" xmlns:cs=\"http://calendarserver.org/ns/\">"
                      "  <d:prop>\n"
                      "     <d:displayname />\n"
                      "     <cs:getctag />\n"
                      "  </d:prop>\n"
                      "</d:propfind>";

    ne_request *req = ne_request_create(sess, "PROPFIND", (uri).c_str());
    ne_add_request_header(req, "Authorization", ("Basic "+base64_auth).c_str());

    ne_set_request_body_buffer(req, propfind.c_str(), propfind.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    int status = ne_get_status(req)->code;

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

int WebClient::put_event(string uri, string evento_xml) {
    string response;

    ne_request *req = ne_request_create(sess, "PUT", (uri+".ics").c_str());
    ne_add_request_header(req, "Authorization", ("Basic "+base64_auth).c_str());

    ne_set_request_body_buffer(req, evento_xml.c_str(), evento_xml.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    ne_request_dispatch(req);
    int status = ne_get_status(req)->code;

    if(status != 201) {
        cout << "\nERROR IN THE PUT METHOD\n" << response; //da cancellare
        ne_request_destroy(req);
        return 1;
    }
    ne_request_destroy(req);
    return 0;
}