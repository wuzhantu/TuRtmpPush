//
//  AMFEncode.hpp
//  TuRtmpPush
//
//  Created by zhantu wu on 2025/6/2.
//

#ifndef AMFEncode_hpp
#define AMFEncode_hpp

#include <stdio.h>

class AMFEncode {
    
    
public:
    AMFEncode();
    char *encodeNamedString(char *output);
    char *encodeNamedBoolean(char *output);
    char *encodeNamedNumber(char *output);
    char *encodeInt16(char *output, short nVal);
    char *encodeInt24(char *output, short nVal);
    char *encodeInt32(char *output, short nVal);
    int encodeInt32LE(char *output, short nVal);
    char *encodeString(char *output);
    char *encodeBoolean(char *output);
    char *encodeNumber(char *output);
    void sendPacket();
};

#endif /* AMFEncode_hpp */
