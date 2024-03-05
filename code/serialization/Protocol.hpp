#pragma once

#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

const std::string space_sep = " ";
const std::string protocol_sep = "\n";

std::string Encode(std::string &content)
{
    std::string package = std::to_string(content.size());
    package += protocol_sep;
    package += content;
    package += protocol_sep;

    return package;
}

bool Decode(std::string &package, std::string *content)
{

    size_t pos = package.find(protocol_sep);
    if (pos == std::string::npos)
    {
        return false;
    }

    std::string str_len = package.substr(0, pos);
    size_t len = stoi(str_len);

    size_t total_len = len + str_len.size() + 2;
    //std::cout << total_len << std::endl;
    //std::cout << package.size() << std::endl;


    if (total_len > package.size())
    {
        return false;
    }


    *content = package.substr(pos + 1, len);
    package.erase(0, total_len);


    return true;
}

class Request
{
public:
    Request(int data1, int data2, char oper)
        : x(data1), y(data2), op(oper)
    {
    }
    Request()
    {
    }

    bool Serialize(std::string *out)
    {
#ifdef DMS
        std::string s = std::to_string(x);
        s += space_sep;
        s += op;
        s += space_sep;
        s += std::to_string(y);
        *out = s;
        return true;
#else
        Json::Value root;
        root["x"] = x;
        root["y"] = y;
        root["op"] = op;
        Json::FastWriter w;
        *out = w.write(root);
        return true;

#endif
        
    }

    bool Deserialize(const std::string &in)
    {
#ifdef DMS
        size_t left = in.find(space_sep);
        if(left == std::string::npos)
        {
            return false;
        }
        std::string px = in.substr(0, left);

        size_t right = in.rfind(space_sep);
        if(right == std::string::npos)
        {
            return false;
        }

        std::string py = in.substr(right + 1);

        if(left + 2 != right)
        {
            return false;
        }

        op = in[left + 1];
        x = std::stoi(px);
        y = std::stoi(py);
        return true;
#else
    Json::Value root;
    Json::Reader r;

    r.parse(in,root);

    x = root["x"].asInt();
    y = root["y"].asInt();
    op = root["op"].asInt();
    return true;
#endif

    }

    void DebugPrint()
    {
        std::cout << "已序列化请求:  " << x << op << y << "=?" << std::endl;
    }

public:
    int x;
    int y;
    char op;
};


class Response
{
public:
    Response(int re, int c)
    :result(re)
    ,code(c)
    {}

    Response()
    {}

public:
    bool Serialize(std::string *out)
    {
#ifdef DMS
        std::string s = std::to_string(result);

        s += space_sep;
        s += std::to_string(code);
        *out = s;

        return true;
#else
        Json::Value root;
        root["result"] = result;
        root["code"] = code;
        Json::StyledWriter w;
        *out = w.write(root);
        return true;
#endif
    }

    bool Deserialize(const std::string &in)
    {
#ifdef DMS
        size_t pos = in.find(space_sep);

        if(pos == std::string::npos)
        {
            return false;
        }

        std::string left = in.substr(0,pos);
        std::string right = in.substr(pos + 1);

        result = std::stoi(left);
        code = std::stoi(right);
        return true;
#else
        Json::Value root;
        Json::Reader r;
        r.parse(in, root);

        result = root["result"].asInt();
        code = root["code"].asInt();
        return true;

#endif
    }


    void DebugPrint()
    {
        std::cout << "序列化结果, result: " << result << ", code: "<< code << std::endl;
    }
       

public:
    int result;
    int code;
};