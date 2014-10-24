/*
 * =====================================================================================
 *
 *       Filename:  parse.cc
 *
 *    Description:  parse mp4 buffer and calculate out duration
 *
 *        Version:  1.0
 *        Created:  2014/10/24 15时58分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX (http://xcoder.in/), admin@xcoder.in
 *   Organization:  Huaban.com
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>

typedef unsigned char xxuint8;
typedef unsigned int xxuint32;
typedef unsigned long long xxuint64;

xxuint8 BytesToUI08(const char* buf)
{
    return buf[0];
}

xxuint32 BytesToUI32(const char* buf)
{
    return ((((xxuint32)buf[0]) << 24) & 0xff000000)
        | ((((xxuint32)buf[1]) << 16) & 0xff0000)
        | ((((xxuint32)buf[2]) << 8) & 0xff00)
        | ((((xxuint32)buf[3])) & 0xff);
}

xxuint64 BytesToUI64(const char* buf)
{
    return ((((xxuint64)buf[0]) << 56) & 0xff00000000000000)
        | ((((xxuint64)buf[1]) << 48) & 0xff000000000000)
        | ((((xxuint64)buf[2]) << 40) & 0xff0000000000)
        | ((((xxuint64)buf[3]) << 32) & 0xff00000000)
        | ((((xxuint64)buf[4]) << 24) & 0xff000000)
        | ((((xxuint64)buf[5]) << 16) & 0xff0000)
        | ((((xxuint64)buf[6]) << 8) & 0xff00)
        | ((((xxuint64)buf[7])) & 0xff);
}

struct Mp4BoxHeader
{
    xxuint32 size;
    xxuint32 type;

    xxuint64 large_size;
    xxuint8 extend_type[16];

    xxuint32 mp4_header_size;
};

bool BoxHeaderIs(Mp4BoxHeader *p_head, const char *type)
{
    const xxuint32 val = BytesToUI32(type);
    return (val == p_head->type);
}

const char* ReadBoxHeader(Mp4BoxHeader *p_head, const char *buf)
{
    const char *p_buf = buf;
    p_head->size = BytesToUI32(buf); buf += 4;
    p_head->type = BytesToUI32(buf); buf += 4;

    if(p_head->size == 1)
    {
        p_head->large_size = BytesToUI64(buf); buf += 8;
    }

    if(0 == memcmp(&p_head->type, "uuid", 4))
    {
        int i;
        for(i = 0; i != 16; ++i)
        {
            p_head->extend_type[i] = BytesToUI08(buf);
            buf += 1;
        }
    }
    p_head->mp4_header_size = (xxuint32)(buf - p_buf);

    return buf;
}

void ReadMvhdDuration(const char* buf, double* fileDuration)
{
    unsigned int i;
    Mp4BoxHeader header;
    buf = ReadBoxHeader(&header, buf);

    xxuint8 version = BytesToUI08(buf); buf += 1;   // version
    buf += 3;   // flag

    xxuint32 time_scale;
    xxuint64 duration;
    if(1 == version)
    {
        buf += 8;   // creat_time
        buf += 8;   // modif_time
        time_scale = BytesToUI32(buf); buf += 4;   // time_scale
        duration = BytesToUI64(buf); buf += 8;     // duration
    }
    else
    {
        buf += 4;   // creat_time
        buf += 4;   // modif_time
        time_scale = BytesToUI32(buf); buf += 4;   // time_scale
        duration = BytesToUI32(buf); buf += 4;     // duration
    }

    *fileDuration = (double)duration / time_scale;
}

double ParseDuration(const char* pbuf, const char* pbuf_end)
{
    double file_duration = 0;
    while (pbuf < pbuf_end)
    {
        Mp4BoxHeader header;
        memset(&header, 0, sizeof(header));
        ReadBoxHeader(&header, pbuf);
        if(BoxHeaderIs(&header, "moov"))
        {
            const char* p_end = pbuf + header.size;
            pbuf = ReadBoxHeader(&header, pbuf);
            while(pbuf < p_end)
            {
                Mp4BoxHeader header;
                ReadBoxHeader(&header, pbuf);
                if(BoxHeaderIs(&header, "mvhd"))
                {
                    ReadMvhdDuration(pbuf, &file_duration);
                }
                pbuf += header.size;
            }
        }
        else
        {
            if(header.large_size > 0) pbuf += header.large_size;
            else pbuf += header.size;
        }
    }

    return file_duration;
}

