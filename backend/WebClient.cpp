//
// Created by Jacopo on 15/09/21.
//

#include "WebClient.h"
#include "session.h"
#include <neon/ne_session.h>
#include <neon/ne_request.h>
#include <neon/ne_207.h>
#include <neon/ne_utils.h>
#include <neon/ne_uri.h>
#include <iostream>
using namespace std;

int httpResponseReader(void *userdata, const char *buf, size_t len)
{
    string *str = (string *)userdata;
    str->append(buf, len);
    return 0;
}

int WebClient::setLogin(void *userdata, const char *realm, int attempts, char *username, char *password) {
    vector<string> *login = (vector<string>*) userdata;
    strncpy(username, login->at(0).c_str(), NE_ABUFSIZ);
    strncpy(password, login->at(1).c_str(), NE_ABUFSIZ);
    return attempts;
}

WebClient::WebClient(const string url, const string user, const string pw){
    ne_sock_init();
    sess = ne_session_create("http", url.c_str(), 80);
    this->login_info.push_back(user);
    this->login_info.push_back(pw);
    ne_set_server_auth(sess, WebClient::setLogin, &login_info);
}

WebClient::~WebClient(){
    ne_forget_auth(sess);
    ne_session_destroy(sess);
    ne_sock_exit();
}

int WebClient::do_propfind() {
    string response;

    ne_request *req = ne_request_create(this->sess, "GET", "/principals/uid/a3298160768/");
    //returns a pointer to a request object

    ne_add_response_body_reader(req, ne_accept_always, httpResponseReader, &response);

    int result = ne_request_dispatch(req);
    int status = ne_get_status(req)->code;

    ne_request_destroy(req);

    string errorMessage = ne_get_error(sess);
    ne_session_destroy(sess);

    printf("result %d, status %d\n", result, status);
    cout << response << "\n";

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
    return 1;
}