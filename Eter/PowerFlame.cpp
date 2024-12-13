#include "PowerFlame.h"


const std::string PowerFlame::m_name = "Flame";
const std::string PowerFlame::m_description = "Reveal an opponent’s Illusion and next place your card on any space on the playing field.";


PowerFlame::PowerFlame()
{
}

std::string_view PowerFlame::getName() const
{
	return m_name;
}

std::string_view PowerFlame::getDescription() const
{
	return m_description;
}

bool PowerFlame::checkFlamePower(Board& board, Player& player)
{
	std::string playerColor = player.GetVectorColor();
	bool foundIllusion = false;

	for (int16_t i = 0; i < board.getSize(); i++)
	{
		for (int16_t j = 0; j < board.getSize(); j++)
		{
			auto& cards = board[{i, j}];

			if (!cards.empty() && foundIllusion==false)
			{
				Color topCardColor = cards.back().getColor();

				if(playerColor=="Red" && topCardColor==Color::IlusionBlue)
				{
					foundIllusion = true;
					cards.back().setColor(Color::Blue);
				}
				else
					if (playerColor == "Blue" && topCardColor == Color::IlusionRed)
					{
						foundIllusion = true;
						cards.back().setColor(Color::Red);
					}
			}
		}
	}
	return foundIllusion;
}

void PowerFlame::playFlamePower(Board& board, Player& player)
{

	if (checkFlamePower(board, player)==false)
	{
		std::cout << "You don't have a valid target for Flame power\n";
		return;
	}
	else
	{
		SimpleCard card = player.chooseCard();
		bool canPlayIllusion = false;
		std::vector<SimpleCard> pastcards = player.getPastVector();

		std::cout << "You can place the card anywher on the board\n";
		player.playCard(card, board, pastcards, canPlayIllusion);

	
	}
}
