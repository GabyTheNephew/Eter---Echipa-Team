#include "PowerRock.h"

const std::string PowerRock::m_name = "Rock";
const std::string PowerRock::m_description = "Cover any Illusion card with your card, without revealing the value of the Illusion. ";

PowerRock::PowerRock()
{
}

std::string PowerRock::getName() const
{
	return m_name;
}

std::string PowerRock::getDescription() const
{
	return m_description;
}

void PowerRock::playRockPower(Board& board, Player& player, int16_t x, int16_t y)
{
	SimpleCard chosenCard = player.chooseCard();
	board.pushCard(chosenCard, { x, y });
	player.makeCardInvalid(chosenCard);
	std::vector<SimpleCard> pastVector;
	pastVector.push_back(chosenCard);
	player.setPastVector(pastVector);
}

bool PowerRock::checkRockPower(Board& board, int16_t x, int16_t y)
{
	if ((board[{x, y}].back().getColor() == Color::IlusionRed) || (board[{x, y}].back().getColor() == Color::IlusionBlue))
	{
		return true;
	}
	return false;
}
