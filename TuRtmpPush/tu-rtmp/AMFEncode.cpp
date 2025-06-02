//
//  AMFEncode.cpp
//  TuRtmpPush
//
//  Created by zhantu wu on 2025/6/2.
//

#include "AMFEncode.hpp"
#include <string.h>

AMFEncode::AMFEncode() {
    
}

char * AMFEncode::encodeNameString(char *output) {
    int len = 4;
    char *key = "name";
    output = encodeInt16(output, len); //key的长度
    memcpy(output, key, len);
    output += len;
    return nullptr;
}

char * AMFEncode::encodeString(char *output) {
    int len = 2;
    char *val = "tu";
    if (len < 65536) {
        *output++ = 1; // string
        output = encodeInt16(output, len);
    } else {
        *output++ = 2; // long string
        output = encodeInt32(output, len);
    }
    memcpy(output, val, len);
    output += len;
    return output;
}

char *AMFEncode::encodeInt16(char *output, short nVal) {
    output[1] = nVal & 0xff;
    output[0] = nVal >> 8;
    return nullptr;
}

char *AMFEncode::encodeInt32(char *output, short nVal) {
    output[1] = nVal & 0xff;
    output[0] = nVal >> 8;
    return nullptr;
}
