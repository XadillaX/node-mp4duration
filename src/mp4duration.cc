/*
 * =====================================================================================
 *
 *       Filename:  mp4duration.cc
 *
 *    Description:  MP4 duration parser - node.js c++ addon
 *
 *        Version:  1.0
 *        Created:  2014/10/24 16时14分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  XadillaX (http://xcoder.in/), admin@xcoder.in
 *   Organization:  Touhou Gensokyo
 *
 * =====================================================================================
 */
#include <node.h>
#include <node_buffer.h>
#include "parse.h"
#include <stdio.h>
#include <string.h>
#include <nan.h>
using namespace v8;
using namespace node;

NAN_METHOD(ParseViaBuffer)
{
    NanScope();

    if(args.Length() < 1)
    {
        NanThrowError("Wrong number of arguments.");
    }

    Local<Value> arg = args[0];
    if(!node::Buffer::HasInstance(arg))
    {
        NanThrowError("Bad argument!");
    }

    size_t size = Buffer::Length(arg->ToObject());
    char* pbuf = Buffer::Data(arg->ToObject());
    char* pbuf_end = pbuf + size;

    // check is it mp4
    if(pbuf_end - pbuf < 7)
    {
        NanThrowError("File is too small.");
    }

    char sign[] = { 0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70 };
    for(int i = 0; i < 8; i++)
    {
        if(i == 3) continue;
        if(sign[i] != *(pbuf + i))
        {
            NanThrowError("Broken MP4 file.");
        }
    }

    double file_duration = 0.0f;
    try
    {
        file_duration = ParseDuration(pbuf, pbuf_end);
    }
    catch(...)
    {
        NanThrowError("Broken file or this format is not supported.");
    }

    NanReturnValue(NanNew<Number>(file_duration));
}

NAN_METHOD(ParseViaFile)
{
    NanScope();

    if(args.Length() < 1)
    {
        NanThrowError("Wrong number of arguments.");
    }

    Local<Value> pre_filename = args[0];
    if(!pre_filename->IsString()) 
    {
        NanThrowError("Filename should be a string.");
    }

    char* filename = new char[args[0]->ToString()->Length() + 1];
    Local<String>::Cast(args[0])->WriteAscii(filename);

    FILE* fp = fopen(filename, "rb");
    if(NULL == fp)
    {
        NanThrowError("Can't open this file.");
    }

    unsigned int filesize;
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* filebuf = new char[filesize];
    fread(filebuf, filesize, 1, fp);
    fclose(fp);

    const char* pbuf = filebuf;
    const char* pbuf_end = filebuf + filesize;

    // check is it mp4
    if(pbuf_end - pbuf < 7)
    {
        delete []filebuf;
        NanThrowError("File is too small.");
    }

    char sign[] = { 0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70 };
    for(int i = 0; i < 8; i++)
    {
        if(i == 3) continue;
        if(sign[i] != *(pbuf + i))
        {
            delete []filebuf;
            NanThrowError("Broken MP4 file.");
        }
    }

    double file_duration = 0.0f;
    try
    {
        file_duration = ParseDuration(pbuf, pbuf_end);
    }
    catch(...)
    {
        delete []filebuf;
        NanThrowError("Broken file or this format is not supported.");
    }

    delete []filebuf;

    NanReturnValue(NanNew<Number>(file_duration));
}

void InitAll(Handle<Object> exports)
{
    exports->Set(NanNew<String>("parseByFilename"),
            NanNew<FunctionTemplate>(ParseViaFile)->GetFunction());
    exports->Set(NanNew<String>("parseByBuffer"),
            NanNew<FunctionTemplate>(ParseViaBuffer)->GetFunction());
}

NODE_MODULE(mp4duration, InitAll);

