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

    /**
    * General methods.
    */
    WebClient();
    ~WebClient();
    bool setClient(const string& url, const string user, const string pass, int port);
    void setHttpAndUrl (const string& str);
    int tryLogin();
    string propfindCtag(const string& uri);
    void propfindUri();
    void setUri(string strCalendar, string strTask);
    string getUrl();
    int getPort();
    string getTag();
    string getTagCaldav();
    string getTagCalserver();

    /**
    * Events methods.
    */
    string getUriCalendar();
    string report_calendar();
    string reportEtagCalendar();
    bool put_event(const string& uri, const string evento_xml);
    void setCtagCalendar(string ctag);
    string getCtagCalendar();
    bool deleteCalendar(const string& uid);
    string multiGetCalendar(const list<string>& new_event);

    /**
    * Tasks methods.
    */
    string getUriTask();
    string report_task();
    string reportEtagTask();
    void setCtagTask(string ctag);
    string getCtagTask();
    bool deleteTask(const string& uid);
    string multiGetTask(const list<string>& new_task);

private:
    ne_session *sess;
    string type_of_connection;
    string tag;
    string tag_caldav;
    string tag_calserver;
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
