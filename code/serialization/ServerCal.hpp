#pragma once
#include <iostream>
#include "Protocol.hpp"

enum
{
    Div_zero = 1,
    Mode_zero,
    Other_oper
};

class ServerCal
{
public:
    ServerCal()
    {

    }

    Response CalHelper(const Request &req)
    {
        Response resp(0, 0);
        switch (req.op)
        {
        case '+':
            resp.result = req.x + req.y;
            break;
        case '-':
            resp.result = req.x - req.y;
            break;
        case '*':
            resp.result = req.x * req.y;
            break;
        case '/':
        {
            if(req.y == 0)
            {
                resp.code = Div_zero;
            }
            else
            {
                resp.result = req.x / req.y;
            }
            break;
        }
        case '%':
        {
            if(req.y == 0)
            {
                resp.code = Mode_zero;
            }
            else
            {
                resp.result = req.x % req.y;
            }
            break;
        }
        default:
        resp.code = Other_oper;
            break;
        }

        return resp;

    }

    std::string Cal(std::string &package)
    {
        std::string content ;
        bool r = Decode(package, &content);
        if(r == false)
        {
            return "";
        }


        Request req;
        r = req.Deserialize(content);
        if(r == false)
        {
            return "";
        }

        content = "";
        
        Response res = CalHelper(req);

        res.Serialize(&content);
        content = Encode(content);
        
        return content;



    }

    ~ServerCal()
    {}

};