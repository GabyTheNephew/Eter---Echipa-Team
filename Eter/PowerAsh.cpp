#include "PowerAsh.h"


const std::string PowerAsh::m_name = "ASH";
const std::string PowerAsh::m_description = "Rise like a Phoenix! If one of your cards was removed from play, you can immediately play it again.";

PowerAsh::PowerAsh()
{
}


std::string_view PowerAsh::getName()const
{
	return m_name;
}

std::string_view PowerAsh::getDescription()const
{
	return m_description;
}

std::vector<int16_t> PowerAsh::getEliminatedCardValues(Board& board, Player& player)
{
	std::vector<int16_t> eliminatedValues;
	Color playerColor = (player.GetVectorColor() == "Red") ? Color::Red : Color::Blue;

	
	std::map<int16_t, int16_t> handCards;
	for (const auto& card : player.getVector())
	{
		if (card.getColor() == playerColor ||card.getColor() == Color::usedRed ||card.getColor() == Color::usedBlue)
		{
			handCards[card.getValue()]++;
		}
	}

	
	std::map<int16_t, int16_t> boardCards;
	for (int i = 0; i < board.getRowSize(); i++)
	{
		for (int j = 0; j < board.getColumnSize(); j++)
		{
			if (!board[{i, j}].empty())
			{
				for (const auto& boardCard : board[{i, j}])
				{
					if (boardCard.getColor() == playerColor ||boardCard.getColor() == Color::increasedRed ||boardCard.getColor() == Color::increasedBlue ||boardCard.getColor() == Color::decreasedRed ||boardCard.getColor() == Color::decreasedBlue)
					{
						boardCards[boardCard.getValue()]++;
					}
				}
			}
		}
	}

	
	for (const auto& [value, handCount] : handCards)
	{
		int16_t boardCount = boardCards[value];
		if (handCount > boardCount)
		{
		
			for (int16_t i = 0; i < (handCount - boardCount); i++)
			{
				eliminatedValues.push_back(value);
			}
		}
	}

	return eliminatedValues;
}

bool PowerAsh::checkAshPower(Board& board, Player& player)
{
	return !getEliminatedCardValues(board, player).empty();
}

bool PowerAsh::playAshPower(Board& board, Player& player, int16_t cardValue, int16_t x, int16_t y)
{
	std::vector<int16_t> eliminatedValues = getEliminatedCardValues(board, player);

	
	bool canReplay = false;
	for (int16_t value : eliminatedValues)
	{
		if (value == cardValue)
		{
			canReplay = true;
			break;
		}
	}

	if (!canReplay)
	{
		return false;
	}

	
	if (!board.canBePlaced(x, y))
	{
		return false;
	}

	Color playerColor = player.GetVectorColor()=="Red"?Color::Red:Color::Blue;
	
	board.pushCard(SimpleCard(cardValue, playerColor), { x, y });

	return true;
}