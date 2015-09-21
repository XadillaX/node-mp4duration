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
#include <node_object_wrap.h>
#include "parse.h"
#include <stdio.h>
#include <string.h>
#include <nan.h>
using namespace v8;
using namespace node;

NAN_METHOD(ParseViaBuffer)
{
    Nan::HandleScope scope;

    if(info.Length() < 1)
    {
        return Nan::ThrowError("Wrong number of arguments.");
    }

    if(!node::Buffer::HasInstance(info[0]))
    {
        return Nan::ThrowError("Bad argument!");
    }

    Local<Object> buffer_obj = info[0]->ToObject();
    size_t size = Buffer::Length(buffer_obj);
    char* pbuf = Buffer::Data(buffer_obj);
    char* pbuf_end = pbuf + size;

    // check is it mp4
    if(pbuf_end - pbuf < 7)
    {
        return Nan::ThrowError("File is too small.");
    }

    char sign[] = { 0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70 };
    for(int i = 0; i < 8; i++)
    {
        if(i == 3) continue;
        if(sign[i] != *(pbuf + i))
        {
            return Nan::ThrowError("Broken MP4 file.");
        }
    }

    double file_duration = 0.0f;
    try
    {
        file_duration = ParseDuration(pbuf, pbuf_end);
    }
    catch(...)
    {
        return Nan::ThrowError("Broken file or this format is not supported.");
    }

    info.GetReturnValue().Set(Nan::New<Number>(file_duration));
}

NAN_METHOD(ParseViaFile)
{
    Nan::HandleScope scope;

    if(info.Length() < 1)
    {
        return Nan::ThrowError("Wrong number of arguments.");
    }

    Local<Value> pre_filename = info[0];
    if(!pre_filename->IsString()) 
    {
        return Nan::ThrowError("Filename should be a string.");
    }

    String::Utf8Value afilename(info[0]->ToString());
    char* filename = *afilename;

    FILE* fp = fopen(filename, "rb");
    if(NULL == fp)
    {
        char temp[32 + strlen(filename)];
        sprintf(temp, "Can't open this file: %s.", filename);
        return Nan::ThrowError(temp);
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
        return Nan::ThrowError("File is too small.");
    }

    char sign[] = { 0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70 };
    for(int i = 0; i < 8; i++)
    {
        if(i == 3) continue;
        if(sign[i] != *(pbuf + i))
        {
            return Nan::ThrowError("Broken MP4 file.");
        }
    }

    double file_duration = 0.0f;
    try
    {
        file_duration = ParseDuration(pbuf, pbuf_end);
    }
    catch(...)
    {
        return Nan::ThrowError("Broken file or this format is not supported.");
    }

    info.GetReturnValue().Set(Nan::New<Number>(file_duration));
}

NAN_MODULE_INIT(InitAll)
{
    Nan::Set(target, Nan::New<String>("parseByFilename").ToLocalChecked(),
            Nan::GetFunction(Nan::New<FunctionTemplate>(ParseViaFile)).ToLocalChecked());
    Nan::Set(target, Nan::New<String>("parseByBuffer").ToLocalChecked(),
            Nan::GetFunction(Nan::New<FunctionTemplate>(ParseViaBuffer)).ToLocalChecked());
}

NODE_MODULE(mp4duration, InitAll);
