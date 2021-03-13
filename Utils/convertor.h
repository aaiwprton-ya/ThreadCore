#ifndef CONVERTOR_H
#define CONVERTOR_H

#include <iostream>
#include <sstream>
#include <string>

class Convertor
{
public:
    Convertor();
    static int StrToInt(std::string str);
    static std::string IntToStr(int value);
    static std::wstring IntToWStr(int value);
    static float StrToFlt(std::string str);
    static std::string FltToStr(float value);
    static std::wstring FltToWStr(float value);
};

#endif // CONVERTOR_H

#define STRTOINT(s) Convertor::StrToInt(s)
#define INTTOSTR(i) Convertor::IntToStr(i)
#define INTTOWSTR(i) Convertor::IntToWStr(i)
#define STRTOFLT(s) Convertor::StrToFlt(s)
#define FLTTOSTR(f) Convertor::FltToStr(f)
#define FLTTOWSTR(f) Convertor::FltToWStr(f)
