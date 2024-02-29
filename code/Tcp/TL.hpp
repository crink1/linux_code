#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "Log.hpp"

const std::string dictname = "./dict.txt";
const std::string sep = ":";

static bool split(std::string &s, std::string *part1, std::string *part2)
{
    size_t pos = s.find(sep);
    if(pos == std::string::npos)
    {
        return false;
    }
    *part1 = s.substr(0, pos);
    *part2 = s.substr(pos+1);
    return true;
}


class Tl
{
public:
    Tl()
    {
        std::ifstream in(dictname);
        if(!in.is_open())
        {
            lg(Fatal, "ifstream open %s error", dictname.c_str());
            exit(1);
        }
        std::string line;
        while(std::getline(in, line))
        {
            std::string left ,right;
            split(line, &left, &right);
            dict[left] = right;
        }
        in.close();
    }

    std::string translation(const std::string &key)
    {
        auto it = dict.find(key);

        if(it != dict.end())
        {
            return it->second;
        }
        else
        {
            return "Unkonw";
        }

    }
private:
    std::unordered_map<std::string, std::string> dict;
};