#pragma once

#ifdef AUTHDLL_EXPORTS
#define AUTHDLL_API __declspec(dllexport)
#else
#define AUTHDLL_API __declspec(dllimport)
#endif

extern "C" {
    
    AUTHDLL_API bool ValidateEmail(const char* email);

    
    AUTHDLL_API bool ValidatePassword(const char* password);

    
    AUTHDLL_API const char* GetDLLVersion();
}