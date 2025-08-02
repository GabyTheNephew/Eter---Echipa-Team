#include "pch.h"
#include "AuthDLL.h"
#include <regex>
#include <string>


bool ValidateEmail(const char* email) 
{
    if (!email)
        return false;

    std::string emailStr(email);
    std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(emailStr, emailPattern);
}


bool ValidatePassword(const char* password) 
{
    if (!password) 
        return false;

    std::string passwordStr(password);


    if (passwordStr.length() < 5) 
    {
        return false;
    }


    std::regex passwordPattern(R"(^(?=.*[!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?]).{5,}$)");
    return std::regex_match(passwordStr, passwordPattern);
}

const char* GetDLLVersion() 
{
    return "1.0.0";
}