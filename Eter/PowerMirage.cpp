#include "PowerMirage.h"



const std::string PowerMirage::m_name = "Mirage";
const std::string PowerMirage::m_description = " Exchange your Illusion (face-down card) with a different card from your hand (face-down).";

PowerMirage::PowerMirage()
{
}

std::string PowerMirage::getName() const
{
	return m_name;
}

std::string PowerMirage::getDescription() const
{
	return m_description;
}

bool PowerMirage::playMiragePower(Board& board,Player& player, const SimpleCard& newCard, int16_t x, int16_t y)
{
	
	if (!checkMiragePower(board, player)) 
	{
		return false;
	}
	
	if (x < 0 || x >= board.getRowSize() || y < 0 || y >= board.getColumnSize()) 
	{
		return false;
	}

	if (board[{x, y}].empty())
	{
		return false;
	}

	Color playerIllusionColor = (player.GetVectorColor() == "Red") ? Color::IlusionRed : Color::IlusionBlue;
	if (board[{x, y}].back().getColor() != playerIllusionColor)
	{
		return false;
	}

	if (newCard.getValue() == 5) 
	{
		return false;
	}

	SimpleCard oldIllusion = board[{x, y}].back();
	SimpleCard newIllusion = newCard;
	newIllusion.setColor(playerIllusionColor);
	board[{x, y}].pop_back();
	board[{x, y}].push_back(newIllusion);

	SimpleCard returnedCard = oldIllusion;
	returnedCard.setColor((player.GetVectorColor() == "Red") ? Color::Red : Color::Blue);
	player.makeCardValid(returnedCard);
	player.makeCardInvalid(newCard);

	return true;
}

bool PowerMirage::checkMiragePower(Board& board,Player& player)
{
	Color playerIllusionColor = (player.GetVectorColor() == "Red") ? Color::IlusionRed : Color::IlusionBlue;

	
	for (int16_t i = 0; i < board.getRowSize(); i++) 
	{
		for (int16_t j = 0; j < board.getColumnSize(); j++) 
		{
			if (!board[{i, j}].empty() && board[{i, j}].back().getColor() == playerIllusionColor) 
			{
				return true;
			}
		}
	}
	return false;
}
