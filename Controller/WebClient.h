//
// Created by Jacopo on 15/09/21.
//
#include <neon/ne_props.h> /* ne_simple_propfind, ne_prop_result_set */
#include <neon/ne_uri.h> /* uri */
#include <neon/ne_auth.h> /* ne_set_server_auth, ne_ssl_trust_default_ca */
#include <neon/ne_basic.h> /* ne_session, ne_put, ne_get, ne_delete, ne_mkcol */
#include <vector>

using namespace std;

#ifndef POLICALENDAR_WEBCLIENT_H
#define POLICALENDAR_WEBCLIENT_H

#include <string>
#include <vector>
using namespace std;

class WebClient {
public:
    WebClient();
    ~WebClient();
    void setClient(const string url, const string user, const string pass, int port);
    string propfind_calendar(string uri);
    void setUri();
    string getUriCalendar();
    string getUriTodo();
    string report_calendar(const string uri);
    string report_todo(const string uri);
    int put_event(const string uri, const string evento);
    string getUrl();
    int getPort();
    void setCtag(string ctag);
    string getCtag();

private:
    ne_session *sess;
    string base64_auth;
    string url;
    string uri_calendar;
    string uri_todo;
    int port;
    string ctag;
};


#endif //POLICALENDAR_WEBCLIENT_H
