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
				SimpleCard coveredCard = stack.front();
				stack.pop_front();

                Color originalColor;
                if (coveredCard.getColor() == Color::Red || coveredCard.getColor() == Color::usedRed) {
                    originalColor = Color::Red;
                }
                else if (coveredCard.getColor() == Color::Blue || coveredCard.getColor() == Color::usedBlue) {
                    originalColor = Color::Blue;
                }
                else {
                    continue;
                }

                SimpleCard returnCard(coveredCard.getValue(), originalColor);


                if (originalColor == Color::Red) 
                {
                    player1.makeCardValid(returnCard);
                }
                else 
                {
                    player2.makeCardValid(returnCard);
                }
            }
        }
    }
}


