#pragma once
#include <cstdint>
#include<string_view>
#include "Board.h"
#include "Player.h"
class Game
{

private:
	uint8_t m_round_Counter;
	Board m_gameBoard;
	static Game* m_current_Instance;
	Player player1, player2;

private:
	Game() :m_round_Counter{ 0 },m_gameBoard(){}//same as in board.h

	void startTraining();
	void startMageDuel();
	void startPowerDuel();
	void startTournament();
	

public:
	Game(const Game&) = delete;
	Game operator=(const Game&) = delete;

	static Game* get_Instance();

	enum class GameType :int8_t
	{
		Training,
		MageDuel,
		Power,
		Tournament
	};

	GameType stringToGameType(std::string_view word);
	std::string_view gameTypeToString(GameType gameType)const;

	void incrementRoundCounter();


	void startGame(GameType selectedGameType);

};

