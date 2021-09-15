//
// Created by Jacopo on 15/09/21.
//

#include "session.h"
#include <neon/ne_session.h>



ne_session *sess;
sess = ne_session_create("http","www.google.com", 80);

void ne_close_connection(	ne_session *session);

void ne_session_destroy(	ne_session *session);
