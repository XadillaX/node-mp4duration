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
#include <v8.h>
#include <node_buffer.h>
#include "parse.h"
#include <stdio.h>
#include <string.h>
using namespace v8;

Handle<Value> ParseViaBuffer(const Arguments& args)
{
    HandleScope scope;

    if(args.Length() < 1)
    {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments.")));
        return scope.Close(Undefined());
    }

    Local<Value> arg = args[0];
    if(!node::Buffer::HasInstance(arg))
    {
        ThrowException(Exception::TypeError(String::New("Bad argument!")));
        return scope.Close(Undefined());
    }

    size_t size = node::Buffer::Length(arg->ToObject());
    char* pbuf = node::Buffer::Data(arg->ToObject());
    char* pbuf_end = pbuf + size;

    // check is it mp4
    if(pbuf_end - pbuf < 7)
    {
        ThrowException(Exception::TypeError(String::New("File is too small.")));
        return scope.Close(Undefined());
    }

    char sign[] = { 0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70 };
    for(int i = 0; i < 8; i++)
    {
        if(i == 3) continue;
        if(sign[i] != *(pbuf + i))
        {
            ThrowException(Exception::TypeError(String::New("Broken MP4 file.")));
            return scope.Close(Undefined());
        }
    }

    double file_duration = 0.0f;
    try
    {
        file_duration = ParseDuration(pbuf, pbuf_end);
    }
    catch(...)
    {
        ThrowException(Exception::TypeError(String::New("Broken file or this format is not supported.")));
        return scope.Close(Undefined());
    }

    return scope.Close(Number::New(file_duration));
}

Handle<Value> ParseViaFile(const Arguments& args)
{
    HandleScope scope;

    if(args.Length() < 1)
    {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments.")));
        return scope.Close(Undefined());
    }

    Local<Value> pre_filename = args[0];
    if(!pre_filename->IsString()) 
    {
        ThrowException(Exception::TypeError(String::New("Filename should be a string.")));
        return scope.Close(Undefined());
    }

    char* filename = new char[args[0]->ToString()->Length() + 1];
    Local<String>::Cast(args[0])->WriteAscii(filename);

    FILE* fp = fopen(filename, "rb");
    if(NULL == fp)
    {
        ThrowException(Exception::TypeError(String::New("Can't open this file.")));
        return scope.Close(Undefined());
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
        ThrowException(Exception::TypeError(String::New("File is too small.")));
        return scope.Close(Undefined());
    }

    char sign[] = { 0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70 };
    for(int i = 0; i < 8; i++)
    {
        if(i == 3) continue;
        if(sign[i] != *(pbuf + i))
        {
            delete []filebuf;
            ThrowException(Exception::TypeError(String::New("Broken MP4 file.")));
            return scope.Close(Undefined());
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
        ThrowException(Exception::TypeError(String::New("Broken file or this format is not supported.")));
        return scope.Close(Undefined());
    }

    delete []filebuf;

    return scope.Close(Number::New(file_duration));
}

void Init(Handle<Object> exports)
{
    exports->Set(String::NewSymbol("parseByFilename"),
            FunctionTemplate::New(ParseViaFile)->GetFunction());
    exports->Set(String::NewSymbol("parseByBuffer"),
            FunctionTemplate::New(ParseViaBuffer)->GetFunction());
}

NODE_MODULE(mp4duration, Init);

