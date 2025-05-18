//
//  handshake.hpp
//  TuRtmpPush
//
//  Created by zhantu wu on 2025/5/18.
//

#ifndef handshake_hpp
#define handshake_hpp

#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <unistd.h>
#include <iostream>

using namespace std;

class handshake {
    int socketVal;
    
public:
    handshake(int socketVal);
    void threehandshake();
};

#endif /* handshake_hpp */
