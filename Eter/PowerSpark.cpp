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

void PowerSpark::printCoveredCards(Board& board, Player& player)
{
	std::string playerColor = player.GetVectorColor();
	bool foundCoveredCards = false;

	std::cout << "Covered cards are: \n";

	for (int16_t i = 0; i < board.getRowSize(); i++)
	{
		for (int16_t j = 0; j < board.getColumnSize(); j++)
		{
			const auto& cards = board[{i, j}];

			if (cards.size() > 1)
			{
				bool playerCardFound = false;
				for (int16_t k = 0; k < cards.size(); k++)
				{
					if ((playerColor == "Red" && cards[k].getColor() == Color::Red) || (playerColor == "Blue" && cards[k].getColor() == Color::Blue))
					{
						playerCardFound = true;
					}
					else if (playerCardFound)
					{
						std::cout << "- Card With Value " << cards[k].getValue() << " at ( " << i << " " << j << ")\n";
						foundCoveredCards = true;
						break;
					}
				}
			}
		}
	}	

}

bool PowerSpark::checkSparkPower(Board& board, Player& player)
{

	std::string playerColor = player.GetVectorColor();


	for (int16_t i = 0; i < board.getSize(); i++)
	{
		for (int16_t j = 0; j < board.getSize(); j++)
		{
			const auto& cards = board[{i, j}];
			if (ColorToString(cards[cards.size()].getColor()) != playerColor)
			{
				if (cards.size() > 1)
				{
					for (int16_t k = 1; k < cards.size(); k++)
					{
						if ((playerColor == "Red" && cards[k].getColor() == Color::Red) || (playerColor == "Blue" && cards[k].getColor() == Color::Blue))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void PowerSpark::playSparkPower(Board& board, Player& player)
{

	printCoveredCards(board, player);

	std::cout << "Chose a card to you want to move";
	SimpleCard selectedCard = player.chooseCard();

	int16_t rowOfTheSelectedCard,columnOfTheSeectedCard;
	std::cout << "Enter the source coordinates of the chosen card (row col): ";
	std::cin >> rowOfTheSelectedCard >> columnOfTheSeectedCard;

	try {
		board.popCardAt({ rowOfTheSelectedCard, columnOfTheSeectedCard }, selectedCard);
	}
	catch (const std::exception& e) {
		std::cout << "Error: Invalid source coordinates or card not found.\n";
		return;
	}

	std::optional<std::pair<bool, bool>> canPlayIllusion = std::make_pair(false,false);
	auto pastvector = player.getPastVector();
	player.playCard(selectedCard, board, pastvector, canPlayIllusion);
}
