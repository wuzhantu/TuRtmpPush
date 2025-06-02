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
    char *encodeNameString(char *output);
    char *encodeString(char *output);
    char *encodeInt16(char *output, short nVal);
    char *encodeInt32(char *output, short nVal);
};

#endif /* AMFEncode_hpp */
