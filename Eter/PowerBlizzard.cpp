#include "PowerBlizzard.h"
#include "Game.h"


const std::string PowerBlizzard::m_name = "PowerBlizzard";
const std::string PowerBlizzard::m_description = "Place this card onto any space and choose a row or column that it will impact. The opponent cannot play any cards onto the chosen row/column during his next turn. Condition - your opponent must have a free space to play a card. Remove this card from play after the next opponent turn. ";

PowerBlizzard::PowerBlizzard()
{
}

std::string PowerBlizzard::getName() const
{
	return m_name;
}

std::string PowerBlizzard::getDescription() const
{
	return m_description;
}

void PowerBlizzard::playBlizzardPower(Game& game, bool isRow, int index)
{
	
	Board& board = game.getBoard();
	game.m_restrictedPositions.clear();

	if (isRow) 
	{
		for (int j = 0; j < board.getColumnSize(); j++) 
		{
			game.m_restrictedPositions.push_back({ index, j });
		}
	}
	else 
	{
		for (int i = 0; i < board.getRowSize(); i++)
		{
			game.m_restrictedPositions.push_back({ i, index });
		}
	}

	game.m_restrictionRemainingTurns = 2;
}

bool PowerBlizzard::checkBlizzardPower(Board& board)
{
	for (int i = 0; i < board.getRowSize(); i++)
	{
		for (int j = 0; j < board.getColumnSize(); j++) 
		{
			if (board.canBePlaced(i, j)) 
			{
				return true;
			}
		}
	}
	return false;
}
