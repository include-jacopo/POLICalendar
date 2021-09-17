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
#include <neon/ne_props.h> /* ne_prop_result_set, ne_session */

struct WebPath {
    WebPath(string host, string path, string ressourceType, string lastModified,
            string contentType) :
            host(host),
            path(path),
            ressourceType(ressourceType),
            lastModified(lastModified),
            contentType(contentType) {

    }
    string host;
    string path;
    string ressourceType;
    string lastModified;
    string contentType;
};

class WebClient {
public:
    WebClient(const string url, const string user, const string pass, const unsigned port);
    ~WebClient();
    int do_propfind(const string uri);
    bool get(string uri, string localDestination);

    vector<WebPath> ls(string uri);
    vector<WebPath> tree(string uri);

private:
    ne_session *sess;
    vector<string> login_info;
    string messageError;
    string base64_auth;

};



#endif //POLICALENDAR_WEBCLIENT_H
