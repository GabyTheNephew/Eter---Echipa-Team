#include "PowerEarthquake.h"


const std::string PowerEarthquake::m_name = "EARTHQUAKE";
const std::string PowerEarthquake::m_description = "Remove from play all visible ‘1’ cards.";

PowerEarthquake::PowerEarthquake()
{
}

std::string PowerEarthquake::getName() const
{
	return m_name;
}

std::string PowerEarthquake::getDescription() const
{
	return m_description;
}

void PowerEarthquake::playEarthquakePower(Board& board)
{

	for (int16_t i = 0; i < board.getSize(); i++)
		for (int16_t j = 0; j < board.getSize(); j++)
		{
			if (!board[{i, j}].empty())
			{
				if (board[{i, j}].back().getValue() == 1) {
					board.popCard({ i,j });
				}
			}
		}
}


bool PowerEarthquake::checkEarthquakePower(Board& board)
{
	for (int16_t i = 0; i < board.getSize(); i++)
		for (int16_t j = 0; j < board.getSize(); j++)
		{
			if (!board[{i, j}].empty())
			{
				if (board[{i, j}].back().getValue() == 1) {
					return true;
				}
			}
		}
	return false;

}
