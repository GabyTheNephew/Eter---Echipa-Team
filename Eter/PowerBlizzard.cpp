#include "PowerBlizzard.h"



const std::string PowerBlizzard::m_name = "PowerBlizzard";
const std::string PowerBlizzard::m_description = "Place this card onto any space and choose a row or column that it will impact. The opponent cannot play any cards onto the chosen row/column during his next turn. Condition - your opponent must have a free space to play a card. Remove this card from play after the next opponent turn. ";

PowerBlizzard::PowerBlizzard()
{
}

std::string PowerBlizzard::getName() const
{
	return m_name;
}

std::string PowerBlizzard::getDescription() const
{
	return m_description;
}

void PowerBlizzard::playBlizzardPower(Board& board, Player player1,Player player2, int16_t x, int16_t y,char position, std::vector<SimpleCard>& PastCards, std::optional<std::pair<bool, bool>>& canPlayIllusion)
{
	std::vector<std::pair<int16_t, int16_t>> invalidPos;
	if (position == 'R')
	{
		for (int16_t i = 0; i < board.getSize(); i++)
		{
			invalidPos.push_back({ x,i });
		}
	}
	else
	{
		if (position == 'C')
		{
			for (int16_t i = 0; i < board.getSize(); i++)
			{
				invalidPos.push_back({ i,y });
			}
		}
	}

	if (player1.numberofValidCards() > 0)
	{
		int16_t pozX, pozY;
		std::cout << "Player 1's turn\n";
		bool check = false;


		while (check == false) 
		{
			SimpleCard chosenCard = player1.chooseCard();
			std::cin >> pozX >> pozY;
			std::pair<int16_t, int16_t> poz = { pozX,pozY };

			if (chosenCard.getValue() != 0 && std::find(invalidPos.begin(), invalidPos.end(), poz) == invalidPos.end())
			{
				player2.playCard(chosenCard, board, PastCards, canPlayIllusion);
				check = true;
			}
			board.print();

		}
		
	}

}

bool PowerBlizzard::checkBlizzardPower(Board& board, Player& player, int16_t x, int16_t y)
{
	return (board.canBePlaced(x, y));
}
