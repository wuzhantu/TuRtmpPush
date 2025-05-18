//
//  socketconnect.hpp
//  TuRtmpPush
//
//  Created by zhantu wu on 2025/5/18.
//

#ifndef socketconnect_hpp
#define socketconnect_hpp

#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <unistd.h>
#include <iostream>

using namespace std;

class socketconnect {
public:
    int socketVal;
    
public:
    socketconnect();
    void connectServer();
};

#endif /* socketconnect_hpp */
