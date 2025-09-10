#include "pch.h"
#include "GameSaveLibrary.h"
#include <fstream>

bool SaveGame(const std::string& filePath, const std::string& gameData) {
    std::ofstream out(filePath, std::ios::binary);
    if (!out.is_open()) return false;
    out << gameData;
    return true;
}

std::string LoadGame(const std::string& filePath) {
    std::ifstream in(filePath, std::ios::binary);
    if (!in.is_open()) return {};
    std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return data;
}
