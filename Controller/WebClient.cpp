//
// Created by Jacopo on 15/09/21.
//

#include "WebClient.h"
#include <neon/ne_session.h>
#include <neon/ne_request.h>
#include <neon/ne_auth.h>
#include <cstring>
#include <iostream>
#include <string>
#include <list>
#include <neon/ne_utils.h>
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

string WebClient::getTag(){
    return this->tag;
};

string WebClient::getTagCaldav(){
    return this->tag_caldav;
};

string WebClient::getTagCalserver(){
    return this->tag_calserver;
};

void WebClient::setHttpAndUrl (string str){
    if (str.starts_with("https")) {
        this->type_of_connection = "https";
    } else if (str.starts_with("http")) {
        this->type_of_connection = "http";
    } else {
        this->type_of_connection = "no_protocol";
        return;
    }

    string path = str.substr(type_of_connection.size() + 3);  //rimuovo http o https dall'url
    auto iSubpath = path.find_first_of('/');

    if (iSubpath == -1) iSubpath = path.length();
    this->url = path.substr(0, iSubpath);   // Host URL
    this->subpath = path.substr(iSubpath);      // Subpath URL

    if (this->subpath.ends_with('/'))
        this->subpath = this->subpath.substr(0, this->subpath.size() - 1);

    if(this->port == 0){ //Se l'utente non ha specificato la porta applico quelle standard
        if(this->type_of_connection == "https"){
            this->port = 443;
        } else {
            this->port = 80;
        }
    }
}

int my_auth(void *userdata, const char *realm, int attempts, char *username, char *password) {
    string format = "%" + to_string(NE_ABUFSIZ) + "s%" + to_string(NE_ABUFSIZ) + "s";
    sscanf((char*)userdata, format.c_str(), username, password);
    return attempts;
}

void hook_pre_send(ne_request *req, void *userdata, ne_buffer *header) {
    /**
     * Content-Type breaks sabre/dav localhost server.
     * At the moment no further headers are necessary.
     * User-agent and required auth headers are set by setClient.
     */

    std::string headersToAdd[] = {"Content-Type: application/xml charset=utf-8\n"};
    for (const auto& h: headersToAdd) {
        ne_buffer_zappend(header, h.c_str());
    }
}

bool WebClient::setClient(const string url, const string user, const string pass, int port) {
    this->port = port;

    setHttpAndUrl(url); //Rimuove "http" o "https" dall'url, salva sia l'url che il protocollo usato

    if(type_of_connection == "no_protocol"){ //se la setHttpAndUrl si accorge che manca il protocollo
        return false;
    }

    ne_sock_init();
    sess = ne_session_create(type_of_connection.c_str(), this->url.c_str(), this->port);

    if(type_of_connection == "https"){
        ne_ssl_trust_default_ca(sess);
    }

    /** TODO REMOVE DEBUG **/
    auto f = fopen("response.txt", "w+");
    ne_debug_init(f, NE_DBG_HTTP|NE_DBG_HTTPBODY);

    // Set up authentication
    buf_userpw = string(user+'\n'+pass);
    ne_set_server_auth(sess, my_auth, (void*)buf_userpw.c_str());
    // Add User-Agent
    ne_set_useragent(sess, "PoliCalendar/0.1");
    return true;
}

int WebClient::tryLogin() {
    //Provo ad ottenere il link del calendario dell' utente per testare il login
    string response;
    string propfind_link_user = "<d:propfind xmlns:d=\"DAV:\">\n"
                                "  <d:prop>\n"
                                "     <d:current-user-principal />\n"
                                "  </d:prop>\n"
                                "</d:propfind>";

    string url_prop = subpath + "/";

    ne_request *req = ne_request_create(sess, "PROPFIND", (url_prop).c_str());
    ne_add_request_header(req, "Depth", "0");

    ne_set_request_body_buffer(req, propfind_link_user.c_str(), propfind_link_user.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    ne_request_destroy(req);

    //Lettura del tag ricevuto, che può essere maiuscolo o minuscolo
    //per esempio <D:multistatus> oppure <d:multistatus>
    if(!response.empty()){
        size_t end = response.find("=\"DAV:\"");
        size_t start = response.find("xmlns");
        this->tag = response.substr(start+6, end-(start+6)); //salvo solo il tag
    }

    string link_user = readLinkUser(response, getTag()); //ottenuto l'xml, pesco solo l'url che mi interessa, altrimenti è vuoto

    if(link_user.empty()){
        return 1; //Non ho avuto la risposta che mi aspettavo, il login non è avvenuto
    }

    switch (result) {
        case NE_OK:
            return 0; //Il login è andato a buon fine
        case NE_CONNECT:
            return 2; //Errore di connessione con il server
        case NE_TIMEOUT:
            return 3; //Connessione timeout
        default:
            return 4; //Errore generico con il server
    }
}

void WebClient::setUri(string strCalendar, string strTask) {
    this->uri_calendar = strCalendar;
    this->uri_task = strTask;
}

string WebClient::getUriCalendar(){
    return this->uri_calendar;
}

string WebClient::getUriTask(){
    return this->uri_task;
}

void WebClient::propfindUri(){
    //Per prima cosa necessito dell'url specifico del nostro utente
    string response;
    string propfind_link_user = "<d:propfind xmlns:d=\"DAV:\">\n"
                                "  <d:prop>\n"
                                "     <d:current-user-principal />\n"
                                "  </d:prop>\n"
                                "</d:propfind>";

    string url_prop = subpath + "/";

    ne_request *req = ne_request_create(sess, "PROPFIND", (url_prop).c_str());
    ne_add_request_header(req, "Depth", "0");

    ne_set_request_body_buffer(req, propfind_link_user.c_str(), propfind_link_user.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    ne_request_dispatch(req);
    ne_request_destroy(req);

    string link_user = readLinkUser(response, getTag()); //ottenuto l'xml, pesco solo l'url che mi interessa

    //Effettuo quindi una nuova propfind verso tale url per ricevere il link alla sua collezione di calendari
    string propfind_calendar_collection = "<d:propfind xmlns:d=\"DAV:\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\n"
                                          "  <d:prop>\n"
                                          "     <c:calendar-home-set />\n"
                                          "  </d:prop>\n"
                                          "</d:propfind>";

    url_prop = link_user; //modifico l'url della richiesta verso l'utente specifico

    req = ne_request_create(sess, "PROPFIND", (url_prop).c_str());
    ne_add_request_header(req, "Depth", "0");

    string response1;
    ne_set_request_body_buffer(req, propfind_calendar_collection.c_str(), propfind_calendar_collection.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response1);

    ne_request_dispatch(req);
    ne_request_destroy(req);

    //Lettura del tag caldav
    //per esempio <cal:calendar-home-set> oppure <B:calendar-home-set>
    if(!response1.empty()){
        size_t end = response1.find("=\"urn:ietf:params:xml:ns:caldav\"");
        int i = 0; bool find = 1;
        while(find){
            i++;
            if(response1[end-i] == ':') {
                find = 0;
            }
        }
        this->tag_caldav = response1.substr(end-i+1, i-1); //salvo solo il tag
    }

    string calendar_collection = readCalendarCollection(response1, getTag(), getTagCaldav()); //ottenuto l'xml, pesco solo l'url che mi interessa

    //Effettuo l'ultima propfind verso la sua collezione di calendari per ricevere l'uri sia del calendario che del to-do
    string propfind_calendar_user = "<d:propfind xmlns:d=\"DAV:\" xmlns:cs=\"http://calendarserver.org/ns/\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\n"
                                    "  <d:prop>\n"
                                    "     <d:resourcetype />\n"
                                    "     <d:displayname />\n"
                                    "     <cs:getctag />\n"
                                    "     <c:supported-calendar-component-set />\n"
                                    "  </d:prop>\n"
                                    "</d:propfind>";

    url_prop = calendar_collection; //modifico l'url della richiesta il calendario dell'utente

    req = ne_request_create(sess, "PROPFIND", (url_prop).c_str());
    ne_add_request_header(req, "Depth", "1");

    string response2;
    ne_set_request_body_buffer(req, propfind_calendar_user.c_str(), propfind_calendar_user.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response2);

    ne_request_dispatch(req);
    ne_request_destroy(req);

    string uri_calendar = readUriCalendar(response2, getTag(), getTagCaldav()); //ottenuto l'xml, pesco solo l'uri del calendario
    string uri_task = readUriTask(response2, getTag(), getTagCaldav()); //pesco anche l'uri dei to-do

    setUri(uri_calendar, uri_task); //salvo l'uri del calendario nella struttura dati
}

void WebClient::setCtagCalendar(string ctag){
    this->ctag_calendar= ctag;
}

void WebClient::setCtagTask(string ctag){
    this->ctag_task= ctag;
}

string WebClient::getCtagCalendar() {
    return this->ctag_calendar;
}

string WebClient::getCtagTask() {
    return this->ctag_task;
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
    ne_add_request_header(req, "Depth", "0");

    ne_set_request_body_buffer(req, propfind.c_str(), propfind.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    int status = ne_get_status(req)->code;
    ne_request_destroy(req);

    //Lettura del tag calendarserver
    //per esempio <xmlns:C="http://calendarserver.org/ns/"> oppure <xmlns:cs="http://calendarserver.org/ns/">
    if(!response.empty() && getTagCalserver().empty()){
        size_t end = response.find("\"http://calendarserver.org/ns/\"");
        int i = 0; bool find = 1;
        while(find){
            i++;
            if(response[end-i] == ':') {
                find = 0;
            }
        }
        this->tag_calserver = response.substr(end-i+1, i-2); //salvo solo il tag
    }

    switch (result) {
        case NE_OK:
            return response;
        case NE_CONNECT:
            throw invalid_argument("ne_connect error");
        case NE_TIMEOUT:
            throw invalid_argument("ne_timeout error");
        case NE_AUTH:
            throw invalid_argument("ne_auth error");
        default:
            throw invalid_argument("ne_generic error");
    }
}

string WebClient::report_calendar() {
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

    ne_request *req = ne_request_create(sess, "REPORT", getUriCalendar().c_str());
    ne_add_request_header(req, "Depth", "1");

    ne_set_request_body_buffer(req, report.c_str(), report.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);


    int result = ne_request_dispatch(req);
    ne_request_destroy(req);

    switch (result) {
        case NE_OK:
            return move(response);
        case NE_CONNECT:
            throw invalid_argument("ne_connect error");
        case NE_TIMEOUT:
            throw invalid_argument("ne_timeout error");
        case NE_AUTH:
            throw invalid_argument("ne_auth error");
        default:
            throw invalid_argument("ne_generic error");
    }
}

string WebClient::report_task() {
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

    ne_request *req = ne_request_create(sess, "REPORT", getUriTask().c_str());
    ne_add_request_header(req, "Depth", "1");

    ne_set_request_body_buffer(req, report.c_str(), report.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    int status = ne_get_status(req)->code;
    ne_request_destroy(req);

    switch (result) {
        case NE_OK:
            return move(response);
        case NE_CONNECT:
            throw invalid_argument("ne_connect error");
        case NE_TIMEOUT:
            throw invalid_argument("ne_timeout error");
        case NE_AUTH:
            throw invalid_argument("ne_auth error");
        default:
            throw invalid_argument("ne_generic error");
    }
}

bool WebClient::put_event(string uri, string evento_xml) {
    string response;

    cout << uri+".ics" << endl;

    ne_request *req = ne_request_create(sess, "PUT", (uri+".ics").c_str());

    ne_set_request_body_buffer(req, evento_xml.c_str(), evento_xml.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    ne_request_dispatch(req);
    int status = ne_get_status(req)->code;
    ne_request_destroy(req);

    if(status != 201) {
        cout << "\nERROR IN THE PUT METHOD" << response << endl; //da cancellare
        return false;
    }
    return true;
}

bool WebClient::deleteCalendar(const string uid) {
    string response;

    ne_request *req = ne_request_create(sess, "DELETE", (getUriCalendar()+uid+".ics").c_str());

    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    ne_request_destroy(req);

    switch (result) {
        case NE_OK:
            return true;
        case NE_CONNECT:
            throw invalid_argument("ne_connect error");
        case NE_TIMEOUT:
            throw invalid_argument("ne_timeout error");
        case NE_AUTH:
            throw invalid_argument("ne_auth error");
        default:
            throw invalid_argument("ne_generic error");
    }
}

bool WebClient::deleteTask(const string uid) {
    string response;

    ne_request *req = ne_request_create(sess, "DELETE", (getUriTask()+uid+".ics").c_str());

    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    ne_request_destroy(req);

    switch (result) {
        case NE_OK:
            return true;
        case NE_CONNECT:
            throw invalid_argument("ne_connect error");
        case NE_TIMEOUT:
            throw invalid_argument("ne_timeout error");
        case NE_AUTH:
            throw invalid_argument("ne_auth error");
        default:
            throw invalid_argument("ne_generic error");
    }
}

string WebClient::reportEtagCalendar() {
    string response;
    string propfind = "<c:calendar-query xmlns:d=\"DAV:\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\n"
                      "    <d:prop>\n"
                      "        <d:getetag />\n"
                      "    </d:prop>\n"
                      "    <c:filter>\n"
                      "        <c:comp-filter name=\"VCALENDAR\">\n"
                      "            <c:comp-filter name=\"VEVENT\" />\n"
                      "        </c:comp-filter>\n"
                      "    </c:filter>\n"
                      "</c:calendar-query>";

    ne_request *req = ne_request_create(sess, "REPORT", (getUriCalendar()).c_str());
    ne_add_request_header(req, "Depth", "1");

    ne_set_request_body_buffer(req, propfind.c_str(), propfind.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    int status = ne_get_status(req)->code;
    ne_request_destroy(req);

    switch (result) {
        case NE_OK:
            return response;
        case NE_CONNECT:
            throw invalid_argument("ne_connect error");
        case NE_TIMEOUT:
            throw invalid_argument("ne_timeout error");
        case NE_AUTH:
            throw invalid_argument("ne_auth error");
        default:
            throw invalid_argument("ne_generic error");
    }
}

string WebClient::reportEtagTask() {
    string response;
    string propfind = "<c:calendar-query xmlns:d=\"DAV:\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\n"
                      "    <d:prop>\n"
                      "        <d:getetag />\n"
                      "    </d:prop>\n"
                      "    <c:filter>\n"
                      "        <c:comp-filter name=\"VCALENDAR\">\n"
                      "            <c:comp-filter name=\"VTODO\" />\n"
                      "        </c:comp-filter>\n"
                      "    </c:filter>\n"
                      "</c:calendar-query>";

    ne_request *req = ne_request_create(sess, "REPORT", (getUriTask()).c_str());
    ne_add_request_header(req, "Depth", "1");

    ne_set_request_body_buffer(req, propfind.c_str(), propfind.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    int status = ne_get_status(req)->code;
    ne_request_destroy(req);

    switch (result) {
        case NE_OK:
            return response;
        case NE_CONNECT:
            throw invalid_argument("ne_connect error");
        case NE_TIMEOUT:
            throw invalid_argument("ne_timeout error");
        case NE_AUTH:
            throw invalid_argument("ne_auth error");
        default:
            throw invalid_argument("ne_generic error");
    }
}

string WebClient::multiGetCalendar(list<string> new_event) {
    string response;
    string payload_iniziale = "<c:calendar-multiget xmlns:d=\"DAV:\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\n"
                              "    <d:prop>\n"
                              "        <d:getetag />\n"
                              "        <c:calendar-data />\n"
                              "    </d:prop>\n";
    string payload_finale =   "</c:calendar-multiget>";

    string event_to_get;
    for(auto i: new_event){
        event_to_get.append("<d:href>"+getUriCalendar()+i+".ics</d:href>\n");
    }
    string report = payload_iniziale + event_to_get + payload_finale;

    ne_request *req = ne_request_create(sess, "REPORT", getUriCalendar().c_str());
    ne_add_request_header(req, "Depth", "1");

    ne_set_request_body_buffer(req, report.c_str(), report.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    ne_request_destroy(req);

    switch (result) {
        case NE_OK:
            return move(response);
        case NE_CONNECT:
            throw invalid_argument("ne_connect error");
        case NE_TIMEOUT:
            throw invalid_argument("ne_timeout error");
        case NE_AUTH:
            throw invalid_argument("ne_auth error");
        default:
            throw invalid_argument("ne_generic error");
    }
}

string WebClient::multiGetTask(list<string> new_task) {
    string response;
    string payload_iniziale = "<c:calendar-multiget xmlns:d=\"DAV:\" xmlns:c=\"urn:ietf:params:xml:ns:caldav\">\n"
                              "    <d:prop>\n"
                              "        <d:getetag />\n"
                              "        <c:calendar-data />\n"
                              "    </d:prop>\n";
    string payload_finale =   "</c:calendar-multiget>";

    string task_to_get;
    for(auto i: new_task){
        task_to_get.append("<d:href>"+getUriTask()+i+".ics</d:href>\n");
    }
    string report = payload_iniziale + task_to_get + payload_finale;

    ne_request *req = ne_request_create(sess, "REPORT", getUriTask().c_str());
    ne_add_request_header(req, "Depth", "1");

    ne_set_request_body_buffer(req, report.c_str(), report.size());
    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    ne_request_destroy(req);

    switch (result) {
        case NE_OK:
            return move(response);
        case NE_CONNECT:
            throw invalid_argument("ne_connect error");
        case NE_TIMEOUT:
            throw invalid_argument("ne_timeout error");
        case NE_AUTH:
            throw invalid_argument("ne_auth error");
        default:
            throw invalid_argument("ne_generic error");
    }
}
