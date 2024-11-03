#pragma once
#include <cstdint>
#include<string_view>
#include "Board.h"

class Game
{

private:
	uint8_t m_round_Counter;
	Board m_gameBoard;
	static Game* m_current_Instance;

private:
	Game() :m_round_Counter{ 0 },m_gameBoard(){}//same as in board.h
	~Game();

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

	GameType stringToGameType(std::string_view cuvant);
	std::string_view gameTypeToString(GameType gameType)const;

	void incrementRoundCounter();


	void startGame(GameType selectedGameType);

};

