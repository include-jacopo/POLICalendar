//
// Created by Jacopo on 15/09/21.
//

#include "session.h"
#include <neon/ne_session.h>
#include <neon/ne_request.h>
#include <iostream>
using namespace std;

int new_session() {
    ne_session *sess;
    sess = ne_session_create("http", "www.google.com", 80);

    ne_request *req = ne_request_create(sess, "GET", "");
    //returns a pointer to a request object

    if (ne_request_dispatch(req)) {
        cout << "Request failed" << ne_get_error(sess);
    } else {
        cout << "ok";
    }

    return 1;
}
