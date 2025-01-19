#include "PowerGale.h"


const std::string PowerGale::m_name = "Gale";
const std::string PowerGale::m_description = "Remove any cards that are covered by others(yours as well) - they are returned to their owners’ hands. ";
PowerGale::PowerGale()
{
}



std::string_view PowerGale::getName()const
{
	return m_name;
}

std::string_view PowerGale::getDescription()const
{
	return m_description;
}

bool PowerGale::checkGalePower(Player& player)
{
	return true;
}

void PowerGale::playGalePower(Board& board, Player& player1, Player& player2)
{
    for (int16_t row = 0; row < board.getRowSize(); ++row)
    {
        for (int16_t col = 0; col < board.getColumnSize(); ++col) 
        {
            std::deque<SimpleCard>& stack = board[{row, col}];

            while (stack.size() > 1) 
            {
                SimpleCard card = stack.back();
                stack.pop_back();

                auto& pastCards = player1.getPastVector();
                auto it = std::find(pastCards.begin(), pastCards.end(), card);
				if (card.getColor() == Color::Red)
				{
					card.setColor(Color::usedRed);
					if (it != pastCards.end())
						player1.makeCardValid(card);
				}
				else if (card.getColor() == Color::Blue)
				{
					card.setColor(Color::usedBlue);
					if (it != pastCards.end())
						player1.makeCardValid(card);
				}
               }
            }
        }
    }


