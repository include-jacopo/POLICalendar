//
// Created by Jacopo on 15/09/21.
//

#ifndef POLICALENDAR_WEBCLIENT_H
#define POLICALENDAR_WEBCLIENT_H

#include <neon/ne_props.h> /* ne_simple_propfind, ne_prop_result_set */
#include <neon/ne_uri.h> /* uri */
#include <neon/ne_auth.h> /* ne_set_server_auth, ne_ssl_trust_default_ca */
#include <neon/ne_basic.h> /* ne_session, ne_put, ne_get, ne_delete, ne_mkcol */
#include <vector>
#include <list>
#include <string>

using namespace std;

class WebClient {
public:
    WebClient();
    ~WebClient();
    void setClient(const string url, const string user, const string pass, int port);
    void setHttpAndUrl (string str);
    int tryLogin();
    string propfindCtag(string uri);
    void propfindUri();
    void setUri(string strCalendar, string strTask);
    string getUriCalendar();
    string getUriTask();
    string report_calendar(const string uri);
    string report_task(const string uri);
    string reportEtagCalendar();
    string reportEtagTask();
    bool put_event(const string uri, const string evento_xml);
    string getUrl();
    int getPort();
    void setCtagCalendar(string ctag);
    void setCtagTask(string ctag);
    string getCtagCalendar();
    string getCtagTask();
    bool deleteCalendar(const string uid);
    bool deleteTask(const string uid);
    string multiGetCalendar(list<string> l);
    string multiGetTask(list<string> l);


private:
    ne_session *sess;
    string type_of_connection;
    string base64_auth;
    string url;
    string subpath;
    string uri_calendar;
    string uri_task;
    int port;
    string ctag_calendar;
    string ctag_task;
    string buf_userpw;
};


#endif //POLICALENDAR_WEBCLIENT_H
