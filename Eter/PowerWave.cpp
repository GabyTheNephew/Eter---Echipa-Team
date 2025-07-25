#include "PowerWave.h"


const std::string m_name = "Wave";
const std::string m_description = "Move any card/stack to the adjacent empty space,then play a card from your hand to this now empty space.";

PowerWave::PowerWave()
{
}

std::string_view PowerWave::getName() const
{
	return m_name;
}

std::string_view PowerWave::getDescription() const
{
	return m_description;
}

bool PowerWave::checkWavePower(Board& board, Player& player, int16_t x, int16_t y)
{
	if (x < 0 || x >= board.getRowSize() || y < 0 || y >= board.getColumnSize())
	{
		return false;
	}

	if (board[{x, y}].empty())
	{
		return false;
	}
	std::vector<std::pair<int16_t, int16_t>> neighbors = {
		{x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1},
		{x + 1, y + 1}, {x - 1, y + 1}, {x - 1, y - 1}, {x + 1, y - 1}
	};


	for (const auto& [nx, ny] : neighbors) {
		if (nx >= 0 && nx < board.getRowSize() && ny >= 0 && ny < board.getColumnSize()) {
			if (board[{nx,ny}].empty()) {
				return true;
			}
		}
	}
	return false;
}

void PowerWave::playWavePower(Board& board, Player& player, int16_t sourceX, int16_t sourceY,int16_t targetX, int16_t targetY, const SimpleCard& cardToPlay)
{
	board.moveSpace(sourceX, sourceY, targetX, targetY);
	
	board.pushCard(cardToPlay, { sourceX, sourceY });
	player.makeCardInvalid(cardToPlay);
	player.getPastVector().push_back(cardToPlay);
}

