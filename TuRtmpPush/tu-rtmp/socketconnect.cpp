//
//  socketconnect.cpp
//  TuRtmpPush
//
//  Created by zhantu wu on 2025/5/18.
//

#include "socketconnect.hpp"

socketconnect::socketconnect() {
    
}

void socketconnect::connectServer() {
    struct addrinfo hints = {0}, *ai;
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_DEFAULT;
    
    unsigned short port = 1935;
    const char *hostname = "192.168.0.101";

    char portstr[10];
    snprintf(portstr, sizeof(portstr), "%d", port);
    int addrret = getaddrinfo(hostname, portstr, &hints, &ai);
    if (addrret != 0) {
        cout << "Problem accessing the DNS" << endl;
    }
    
    socketVal = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (ai->ai_family == AF_INET6) {
        struct sockaddr_in6 *in6 = (struct sockaddr_in6 *)ai->ai_addr;
        in6->sin6_port = htons(port);
    }
    
    if (socketVal != -1) {
        if (connect(socketVal, ai->ai_addr, ai->ai_addrlen) < 0) {
            cout << "Failed to connect socket" << endl;
        } else {
            cout << "Success to connect socket" << endl;
        }
    }
}
