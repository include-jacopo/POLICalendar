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
    WebPath(std::string host, std::string path, std::string ressourceType, std::string lastModified,
               std::string contentType) :
            host(host),
            path(path),
            ressourceType(ressourceType),
            lastModified(lastModified),
            contentType(contentType) {

    }
    std::string host;
    std::string path;
    std::string ressourceType;
    std::string lastModified;
    std::string contentType;
};

class WebClient {
public:
    WebClient(const std::string url, const std::string user, const std::string pass, const unsigned port);
    ~WebClient();
    int do_propfind(const std::string uri);
    bool get(std::string uri, std::string localDestination);

    std::vector<WebPath> ls(std::string uri);
    std::vector<WebPath> tree(std::string uri);

private:
    ne_session *sess;
    static int setLogin(void *userdata, const char *realm, int attempts, char *username, char *password);
    static void getProps(void *userdata, const ne_uri *uri, const ne_prop_result_set *set);
    std::vector<string> login_info;
    std::string messageError;

};



#endif //POLICALENDAR_WEBCLIENT_H
