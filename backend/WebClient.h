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


class WebClient {
public:
    WebClient(const std::string url, const std::string user, const std::string pass);
    ~WebClient();
    int do_propfind();

private:
    ne_session *sess;
    static int setLogin(void *userdata, const char *realm, int attempts, char *usernmae, char *password);
    std::vector<string> login_info;

};


#endif //POLICALENDAR_WEBCLIENT_H
