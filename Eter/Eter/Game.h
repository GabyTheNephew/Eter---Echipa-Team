#pragma once
#include <cstdint>
#include<string_view>
#include "Board.h"

class Game
{

private:
	uint8_t m_round_Counter;
	Board  m_board;
	static Game* m_current_Instance;

private:
	Game() :m_round_Counter{ 0 },m_board(){}//same as in board.h
	~Game();
	

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

	uint8_t getRoundCounter();
	void setRoundCounter();

	void startGame();
};

