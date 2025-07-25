#include "PowerSpark.h"

const std::string PowerSpark::m_name = "Spark";
const std::string PowerSpark::m_description = "Take any of your cards that is covered by an opponent’s cards and play it onto a different space. In case of stacks, you can choose any of your covered cards.";

PowerSpark::PowerSpark()
{
}

std::string_view PowerSpark::getName() const
{
	return m_name;
}

std::string_view PowerSpark::getDescription() const
{
	return m_name;
}


bool PowerSpark::checkSparkPower(Board& board, Color playerColor)
{

	for (int16_t i = 0; i < board.getRowSize(); i++)
	{
		for (int16_t j = 0; j < board.getColumnSize(); j++)
		{
			const auto& cards = board[{i, j}];
			if (cards.size() > 1)
			{
				Color topCardColor = cards.back().getColor();
				bool isOpponentCard = false;

				if (playerColor == Color::Red)
				{
					isOpponentCard = (topCardColor == Color::Blue ||topCardColor == Color::increasedBlue ||topCardColor == Color::decreasedBlue);
				}
				else
				{
					isOpponentCard = (topCardColor == Color::Red ||topCardColor == Color::increasedRed ||topCardColor == Color::decreasedRed);
				}
				if (isOpponentCard)
				{
					for (int16_t k = 0; k < cards.size() - 1; k++)
					{
						Color cardColor = cards[k].getColor();
						if (playerColor == Color::Red)
						{
							if (cardColor == Color::Red ||cardColor == Color::increasedRed ||cardColor == Color::decreasedRed)
							{
								return true;
							}
						}
						else
						{
							if (cardColor == Color::Blue ||cardColor == Color::increasedBlue ||cardColor == Color::decreasedBlue)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool PowerSpark::playSparkPower(Board& board, Color playerColor, int16_t sourceX, int16_t sourceY, int16_t cardIndex, int16_t destX, int16_t destY)
{

	if (sourceX < 0 || sourceX >= board.getRowSize() || sourceY < 0 || sourceY >= board.getColumnSize())
	{
		return false;
	}

	auto& sourceStack = board[{sourceX, sourceY}];
	if (sourceStack.size() <= 1 || cardIndex >= sourceStack.size() - 1)
	{
		return false;
	}

	SimpleCard& targetCard = sourceStack[cardIndex];
	bool isPlayerCard = false;

	if (playerColor == Color::Red)
	{
		isPlayerCard = (targetCard.getColor() == Color::Red ||targetCard.getColor() == Color::increasedRed ||targetCard.getColor() == Color::decreasedRed);
	}
	else
	{
		isPlayerCard = (targetCard.getColor() == Color::Blue ||targetCard.getColor() == Color::increasedBlue ||targetCard.getColor() == Color::decreasedBlue);
	}

	if (!isPlayerCard)
	{
		return false;
	}

	Color topCardColor = sourceStack.back().getColor();
	bool isOpponentOnTop = false;

	if (playerColor == Color::Red)
	{
		isOpponentOnTop = (topCardColor == Color::Blue ||topCardColor == Color::increasedBlue ||topCardColor == Color::decreasedBlue);
	}
	else
	{
		isOpponentOnTop = (topCardColor == Color::Red ||topCardColor == Color::increasedRed ||topCardColor == Color::decreasedRed);
	}

	if (!isOpponentOnTop)
	{
		return false;
	}

	SimpleCard cardToMove = targetCard;
	sourceStack.erase(sourceStack.begin() + cardIndex);

	board.pushCard(cardToMove, { destX, destY });

	return true;
}
