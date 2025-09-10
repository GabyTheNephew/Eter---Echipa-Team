// GameSaveManager.h
#pragma once
#include <string>
#include "Game.h"

class GameSaveManager {
public:
    static bool Save(const std::string& filePath, const Game& game);
    static bool Load(const std::string& filePath, Game& game);

private:
    static std::string SerializeGame(const Game& game);
    static void DeserializeGame(const std::string& data, Game& game);
};
