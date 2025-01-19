#include "PowerFire.h"

const std::string PowerFire::m_name = "Fire";
const std::string PowerFire::m_description = " Return to their owners’ hands all visible cards of the same value(at least 2 of them) chosen by you - also yours!In case of stacks remove only the top card.";

PowerFire::PowerFire()
{

}

std::string PowerFire::getName() const
{
	return m_name;
}

std::string PowerFire::getDescription() const
{
	return m_description;
}



void PowerFire::playFirePower(Board& board, Player& player1, Player& player2, int16_t value)
{
	for (int16_t i = 0; i < board.getSize(); i++)
		for (int16_t j = 0; j < board.getSize(); j++)
		{
			if (!board[{i, j}].empty())
			{
				if (board[{i, j}].back().getValue() == value) {
					if (board[{i, j}].back().getColor() == Color::Red) {
						auto card = SimpleCard(value, Color::Red);
						player1.makeCardValid(card);
						board.popCard({ i, j });
					}
					else if (board[{i, j}].back().getColor() == Color::Blue){
						auto card = SimpleCard(value, Color::Blue);
						player1.makeCardValid(card);
						board.popCard({ i, j });
					}

				}
			}
		}
}

bool PowerFire::checkFirePower(Board& board, int16_t value)
{
	int16_t count = 0;
	for (int16_t i = 0; i < board.getSize(); i++) {
		if (count >= 2)
		{
			return true;
		}
		for (int16_t j = 0; j < board.getSize(); j++)
		{
			if (board[{i, j}].back().getValue() == value) {
				count++;
			}

		}
	}
	return false;
}