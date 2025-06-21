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

char * AMFEncode::encodeNamedString(char *output) {
    int len = 4;
    char *key = "name";
    output = encodeInt16(output, len); //key的长度
    memcpy(output, key, len);
    output += len;
    return encodeString(output);
}

char * AMFEncode::encodeNamedBoolean(char *output) {
    int len = 4;
    char *key = "name";
    output = encodeInt16(output, len); //key的长度
    output += len;
    return encodeBoolean(output);
}

char * AMFEncode::encodeNamedNumber(char *output) {
    int len = 4;
    char *key = "name";
    output = encodeInt16(output, len); //key的长度
    output += len;
    return encodeNumber(output);
}

char *AMFEncode::encodeInt16(char *output, short nVal) {
    output[1] = nVal & 0xff;
    output[0] = nVal >> 8;
    return nullptr;
}

char *AMFEncode::encodeInt24(char *output, short nVal) {
    output[1] = nVal & 0xff;
    output[0] = nVal >> 8;
    return nullptr;
}

char *AMFEncode::encodeInt32(char *output, short nVal) {
    output[1] = nVal & 0xff;
    output[0] = nVal >> 8;
    return nullptr;
}

int AMFEncode::encodeInt32LE(char *output, short nVal) {
    output[1] = nVal & 0xff;
    output[0] = nVal >> 8;
    return 0;
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

char * AMFEncode::encodeBoolean(char *output) {
    int bVal = 0;
    *output++ = 1;
    *output++ = bVal ? 0x01 : 0x00;
    return output;
}

char * AMFEncode::encodeNumber(char *output) {
    return output;
}

static const int packetSize[] = {12, 8, 4, 1};

void AMFEncode::sendPacket() {
    // Chunk header由Basic Header和Message header组成
    // Basic header总共有三种类型，长度分别是1、2、3（其实是cs id有三种类型）
    // Message header总共有四种类型，长度分别是0、3、7、11 （其实是fmt有四种类型）
    // packetSize四个值的意思是fmt四种类型下Message header的长度加上fmt需要的一个字节
    int m_headerType = 1; // m_headerType就是fmt的意思，fmt类型决定Message header的长度
    int nSize = packetSize[m_headerType]; // Message header + fmt
    int hSize = nSize; //hSize是整个Chunk header的长度
    char *m_body;
    char *header;
    if (m_body) {
        header = m_body - nSize;
    }
    
    int cSize = 0; // cSize是cs id的长度，cs id <= 63不需要额外字节，fmt那个字节剩余的六位正好能放下
    int m_nChannel = 500;
    int t = 0xff9825; //时间戳
    
    if (m_nChannel > 319) {
        // cs id > 319 需要2个字节
        cSize = 2;
    } else if (m_nChannel > 63) {
        // 63 < cs id <= 319，需要1个字节
        cSize = 1;
    }
    
    if (cSize > 0) {
        hSize += cSize; // 再加上cs id的长度
        header -= cSize;
    }
    
    // 判断是否需要Extended Timestamp
    // nSize > 1其实就是Message header长度>0，也就是说是有时间戳的类型(fmt=3没有时间戳)，如果时间戳都没有，何来Extended Timestamp？
    // t > 0xffffff是说只有时间戳大于0xffffff导致存不下才需要Extended Timestamp
    if (nSize > 1 && t >= 0xffffff) {
        hSize += 4; //Extended Timestamp是四个字节
        header -= 4;
    }
    
    char c = m_headerType << 6; //c是存储fmt的字节，最高两位存储fmt，剩余六位的值需要根据cs id大小做对应处理
    switch (cSize) {
        case 0:
            // cs id <= 63，这个时候剩余六位存储cs id的值
            c |= m_nChannel;
            break;
        case 1:
            // 63 < cs id <= 319，这个时候剩余六位赋值为0，因为默认就是0，所以啥都不用干
            break;
        case 2:
            // cs id > 319，这个时候剩余六位赋值为1（不是所有位为1）
            c |= 1;
            break;
        default:
            break;
    }
    
    char *hptr = header;
    *hptr++ = c; // 把fmt存起来
    if (cSize) {
        int tmp = m_nChannel - 64; // 为什么要-64？因为这样可以让存储cs id的字节尽量存下更大的数
        *hptr++ = tmp & 0xff; //cs id 第一个字节赋值
        if (cSize == 2) {
            *hptr++ = tmp >> 8; // cs id 第二个字节赋值，>> 8表示把去掉一个字节剩余的位赋值给第二个字节
            // 举例：tmp = 00000000 00000000 10100100 01100010 cs id 第一个字节 = 01100010 第二个字节 = 10100100
        }
    }
    
//    {11, 7, 3, 0};
    
    // 存储timestamp 3个字节
    if (nSize > 1) {
        hptr = encodeInt24(hptr, t > 0xffffff ? 0xffffff : t);
    }
    
    // 继续存储message length 3个字节和 message type id 1个字节
    if (nSize > 4) {
        int m_nBodySize = 0;
        int m_packetType = 0;
        hptr = encodeInt24(hptr, m_nBodySize);
        *hptr = m_packetType; // message type id
    }
    
    // 继续存储message stream id （rtmp_pack.m_nInfoField2 = _rtmp->m_stream_id;） 4个字节
    if (nSize > 8) {
        int m_nInfoField2 = 0;
        hptr += encodeInt32LE(hptr, m_nInfoField2);
    }
    
    // 如果有需要，存储Extended Timestamp四个字节
    if (nSize > 1 && t >= 0xffffff) {
        hptr = encodeInt32(hptr, t);
    }
    
    int m_nBodySize = 100;
    int bodySize = m_nBodySize;
    char *buffer = m_body;
    int nChunkSize = 20;
    
    // 只有第一个分块发送了Message Header和（或）Extended Timestamp
    while (bodySize + hSize) {
        if (bodySize < nChunkSize) {
            nChunkSize = bodySize;
        }
        
        writeN(header, hSize + nChunkSize);
        bodySize -= nChunkSize; // 发送一次，总数就减少
        buffer += nChunkSize; // 发送一次，指针就往后挪
        hSize = 0; // 发送一次，重新计算Chunk header的长度
        
        // 准备下一次发送的Chunk header，后续的分块只发送Basic Header，没有Message Header和Extended Timestamp
        if (bodySize > 0) {
            header = buffer - 1; //先退一个字节
            hSize = 1;
            if (cSize) {
                hSize += cSize;
                header -= cSize; // 根据cs id再退cSize个字节
            }
            *header = (0xc0 | c);
            if (cSize) {
                int tmp = m_nChannel - 64;
                header[1] = tmp & 0xff;
                if (cSize == 2) {
                    header[2] = tmp >> 8;
                }
            }
        }
    }
}

void AMFEncode::writeN(const char *buffer, int n) {
    
}
