// GameSaveLibrary.h
#pragma once
#include <string>

#ifdef GAMESAVELIBRARY_EXPORTS
#define GAMESAVELIBRARY_API __declspec(dllexport)
#else
#define GAMESAVELIBRARY_API __declspec(dllimport)
#endif

extern "C" {
    GAMESAVELIBRARY_API bool SaveGame(const std::string& filePath,
        const std::string& gameData);
    GAMESAVELIBRARY_API std::string LoadGame(const std::string& filePath);
}
