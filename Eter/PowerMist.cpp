#include "PowerMist.h"



const std::string PowerMist::m_name = "PowerMist";
const std::string PowerMist::m_description = " Play an Illusion again. You cannot have 2 Illusions at the same time.";

PowerMist::PowerMist()
{
}

std::string PowerMist::getName() const
{
	return m_name;
}

std::string PowerMist::getDescription() const
{
	return m_description;
}

bool PowerMist::playMistPower(Board& board, Player& player, const SimpleCard& selectedCard, int16_t x, int16_t y)
{
	if (!checkMistPower(board, player))
	{
		return false;
	}

	if (selectedCard.getValue() == 5)
	{
		return false; 
	}

	if (!board.canBePlaced(x, y) || !board[{x, y}].empty()) 
	{
		return false; 
	}

	SimpleCard illusionCard = selectedCard;


	if (player.GetVectorColor() == "Red") 
	{
		illusionCard.setColor(Color::IlusionRed);
	}
	else 
	{
		illusionCard.setColor(Color::IlusionBlue);
	}

	board.pushCard(illusionCard, { x, y });
	player.makeCardInvalid(selectedCard);
	player.getPastVector().push_back(illusionCard);

	return true;


}

bool PowerMist::checkMistPower(Board& board, Player& player)
{
	for (int i = 0; i <board.getRowSize() ; i++)
	{
		for (int j = 0; j <board.getColumnSize() ; j++)
		{
			if (!board[{i, j}].empty()) {
				Color topCardColor = board[{i, j}].back().getColor();

				if (player.GetVectorColor() == "Red" && topCardColor == Color::IlusionRed) 
				{
					return false;
				}
				if (player.GetVectorColor() == "Blue" && topCardColor == Color::IlusionBlue) 
				{
					return false; 
				}
			}
		}
	}
	return true;
}
