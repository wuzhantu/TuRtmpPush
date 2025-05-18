//
//  handshake.cpp
//  TuRtmpPush
//
//  Created by zhantu wu on 2025/5/18.
//

#include "handshake.hpp"

handshake::handshake(int socketVal) {
    this->socketVal = socketVal;
}

void handshake::threehandshake() {
    int RTMP_SIG_SIZE= 1536;
    int i;
    uint32_t uptime, suptime;
    int bMatch;
    char type;
    char clientbuf[RTMP_SIG_SIZE + 1], *clientsig = clientbuf + 1;
    char serversig[RTMP_SIG_SIZE];

    // c0 s0
    clientbuf[0] = 0x03; /* not encrypted */
    
    static int clk_tck = 100;
    struct tms t;
    if (!clk_tck) clk_tck = sysconf(_SC_CLK_TCK);
    uint32_t currenttime = times(&t) * 1000 / clk_tck;
    
    uptime = htonl(currenttime);
    memcpy(clientsig, &uptime, 4);

    memset(&clientsig[4], 0, 4);
    
    for (i = 8; i < RTMP_SIG_SIZE; i++)
        clientsig[i] = (char)(rand() % 256);
    
    // 这里是一次性发送了C0和C1？
    int rc = send(socketVal, clientbuf, RTMP_SIG_SIZE + 1, 0); //如果你的端口不是1935，发送0x03不会被当做rtmp握手的
    
    int nBytes = 16384;
    char serverbuf[nBytes + 1];
    memset(serverbuf, 0, nBytes + 1);
    while (1) {
        int result = recv(socketVal, serverbuf, nBytes, 0);
        if (result == 3073) { // 有时分两次收到 1537 + 1536
            break;
        }
    }
    
    // 读取S0
    memcpy(&type, serverbuf, 1);
    int val = type;
    
    // 读取S1
    memcpy(serversig, serverbuf + 1, RTMP_SIG_SIZE);
    
    // 发送C2
    rc = send(socketVal, serversig, RTMP_SIG_SIZE, 0);
    
    // 读取S2
    memcpy(serversig, serverbuf + 1 + RTMP_SIG_SIZE, RTMP_SIG_SIZE);
    
    bMatch = (memcmp(serversig, clientsig, RTMP_SIG_SIZE) == 0);
    if (!bMatch) {
        cout << "client signature does not match!" << endl;
    } else {
        cout << "S2 == C1" << endl;
    }
}
