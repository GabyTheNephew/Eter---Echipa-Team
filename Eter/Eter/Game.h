#pragma once
#include <string_view>
#include<cstdint>
#include "Board.h"
#include "Player.h"
#include "Explosion.h"

class Game
{
private:
    uint8_t m_round_Counter;
    Board m_gameBoard;
    static Game m_current_Instance; 
    Player player1, player2;
    Explosion m_explosion;

    
    Game() : m_round_Counter{ 0 }, m_gameBoard() {}

    // Methods for the different game types
    void startTraining();
    void startMageDuel();
    void startPowerDuel();
    void startTournament();

public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    static Game& get_Instance();

    enum class GameType : int8_t
    {
        Training,
        MageDuel,
        Power,
        Tournament
    };

    GameType stringToGameType(std::string_view word);
    std::string_view gameTypeToString(GameType gameType) const;

    void incrementRoundCounter();
    bool checkPlayExplosion(Board& m_board);
    void startGame(GameType selectedGameType);
};
