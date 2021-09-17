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
#include <neon/ne_props.h>
#include <iostream>

#include <string>
#include <unistd.h> /* close */
#include <fcntl.h> /* open */
#include <cstring> /* strncpy */
using namespace std;

int httpResponseReader(void *userdata, const char *buf, size_t len)
{
    string *str = (string *)userdata;
    str->append(buf, len);
    return 0;
}

static const ne_propname fetchProps[] = {
        { "DAV:", "resourcetype" },
        { "DAV:", "getlastmodified" },
        { "DAV:", "getcontenttype"},
        { NULL }
};

WebClient::WebClient(const string url, const string user, const string pass, const unsigned port){
    ne_sock_init();
    sess = ne_session_create("https", url.c_str(), port);
    ne_ssl_trust_default_ca(sess);
    this->login_info.push_back(user);
    this->login_info.push_back(pass);
    ne_set_server_auth(sess, WebClient::setLogin, &login_info);
}

WebClient::~WebClient(){
    ne_forget_auth(sess);
    ne_session_destroy(sess);
    ne_sock_exit();
}

int WebClient::setLogin(void *userdata, const char *realm, int attempts, char *username, char *password) {
    vector<string> *login = (vector<string>*) userdata;
    strncpy(username, login->at(0).c_str(), NE_ABUFSIZ);
    strncpy(password, login->at(1).c_str(), NE_ABUFSIZ);
    return attempts;
}

void WebClient::getProps(void *userdata, const ne_uri *uri, const ne_prop_result_set *set){

    std::vector<WebPath> *paths = (std::vector<WebPath> *) userdata;

    ne_propname props[] = {
            { "DAV:", "resourcetype" },
            { "DAV:", "getlastmodified" },
            { "DAV:", "getcontenttype"}
    };

    std::string ressourceType = ne_propset_value(set, &props[0]) ? ne_propset_value(set, &props[0]) : std::string();
    std::string lastModified  =  ne_propset_value(set, &props[1]) ?  ne_propset_value(set, &props[1]) : std::string();
    std::string contentType   = ne_propset_value(set, &props[2]) ?  ne_propset_value(set, &props[2]) : std::string();


    WebPath path(uri->host,
                    uri->path,
                    ressourceType,
                    lastModified,
                    contentType);

    // Push information to userdata
    paths->push_back(path);

}

std::vector<WebPath> WebClient::tree(std::string uri){
    std::vector<WebPath> *props = new std::vector<WebPath>;
    const int depth = NE_DEPTH_INFINITE; /* NE_DEPTH_ZERO, NE_DEPTH_ONE, NE_DEPTH_INFINITE */
    int res = ne_simple_propfind(sess, uri.c_str(), depth, fetchProps, WebClient::getProps, props);

    if (!props->empty()) {
        props->erase(props->begin());
    }
    delete props;

    if(res!=NE_OK){
        messageError = ne_get_error(sess);
        return *props;
    }

    return *props;
}

std::vector<WebPath> WebClient::ls(std::string uri){
    const int depth = NE_DEPTH_ONE; /* NE_DEPTH_ZERO, NE_DEPTH_ONE, NE_DEPTH_INFINITE */
    std::vector<WebPath> * props = new std::vector<WebPath>;
    int res = ne_simple_propfind(sess, uri.c_str(), depth, fetchProps, WebClient::getProps, props);

    if (!props->empty()) {
        props->erase(props->begin());
    }
    delete props;

    if(res!=NE_OK){
        messageError = ne_get_error(sess);
        return *props;
    }
    props->erase(props->begin());
    return *props;


}

bool WebClient::get(std::string uri, std::string localDestination){
    int fd = open(localDestination.c_str(), O_WRONLY | O_CREAT);
    int res = ne_get(sess, uri.c_str(), fd);
    if(res!=NE_OK){
        messageError = ne_get_error(sess);
        return false;
    }
    close(fd);
    return true;

}

int WebClient::do_propfind(std::string uri) {
    string response;

    ne_set_useragent(sess, "MyAgent/1.0");
    ne_request *req = ne_request_create(sess, "GET", uri.c_str());

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