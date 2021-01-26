#include "convertor.h"

Convertor::Convertor()
{

}

int Convertor::StrToInt(std::string str)
{
    std::istringstream iss(str, std::istringstream::in);
    int result;
    iss >> result;
    return result;
}

std::string Convertor::IntToStr(int value)
{
    std::ostringstream oss;
    oss << value;
    std::string s = oss.str();
    return s;
}

std::wstring Convertor::IntToWStr(int value)
{
    std::ostringstream oss;
    oss << value;
    std::string s = oss.str();
    std::wstring ws(s.begin(), s.end());
    return ws;
}

float Convertor::StrToFlt(std::string str)
{
    std::istringstream iss(str, std::istringstream::in);
    float result;
    iss >> result;
    return result;
}

std::string Convertor::FltToStr(float value)
{
    std::ostringstream oss;
    oss << value;
    std::string s = oss.str();
    return s;
}

std::wstring Convertor::FltToWStr(float value)
{
    std::ostringstream oss;
    oss << value;
    std::string s = oss.str();
    std::wstring ws(s.begin(), s.end());
    return ws;
}
