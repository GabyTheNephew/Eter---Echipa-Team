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

void PowerMist::playMistPower(Board& board, Player player, int16_t x, int16_t y)
{
	if (player.GetVectorColor() == "Red")
	{

		SimpleCard card = player.chooseCard();
		card.setColor(Color::IlusionRed);
		player.getPastVector().push_back(card);
		board[{x, y}].push_back(card);


	}
	if (player.GetVectorColor() == "Blue")
	{
		SimpleCard card = player.chooseCard();
		card.setColor(Color::IlusionBlue);
		player.getPastVector().push_back(card);
		board[{x, y}].push_back(card);
	}


}

bool PowerMist::checkMistPower(Board& board, Player& player)
{
	bool notMoreThanOne = true;
	for (int i = 0; i <board.getSize() ; i++)
	{
		for (int j = 0; j <board.getSize() ; j++)
		{
			if (!board[{i, j}].empty()) {

				if (player.GetVectorColor() == "Red")
				{
					if (board[{i, j}].back().getColor() == Color::IlusionRed)
					{
						notMoreThanOne = false;
					}
				}
				else if (player.GetVectorColor() == "Blue")
				{
					if (board[{i, j}].back().getColor() == Color::IlusionBlue)
					{
						notMoreThanOne = false;
					}
				}
			}
		}
	}
	return notMoreThanOne;
}
