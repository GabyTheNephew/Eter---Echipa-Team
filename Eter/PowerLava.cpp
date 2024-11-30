#include "PowerLava.h"

PowerLava::PowerLava(std::string_view name, std::string_view description):
	m_name{ "FIRE" },
	m_description{ " Return to their owners’ hands all visible cards of the same value (at least 2 of them) chosen by you - also yours! In case of stacks remove only the top card." }
{
}

std::string PowerLava::getName() const
{
	return m_name;
}

std::string PowerLava::getDescription() const
{
	return m_description;
}

void PowerLava::playPower(Board& board, Player& player1, Player& player2, uint8_t nrLvl)//nrLvl - numarul de putere petru o carte
{
	for (int i = 0; i < board.getSize(); i++) 
		for (int j = 0; j < board.getSize(); j++)
		{
			if (board[{i, j}].getValue() == nrLvl) {
				if (ColorToString(board[{i, j}].getColor()) == "Red") {
					auto card = SimpleCard(nrLvl, Color::Red);
					player1.makeCardValid(card);
					board.popCard({ i, j });
				}
				else{
					if (ColorToString(board[{i, j}].getColor()) == "Blue") {
						auto card = SimpleCard(nrLvl, Color::Blue);
						player1.makeCardValid(card);
						board.popCard({ i, j });
					}
				}
			
			}
		}
}

bool PowerLava::checkPower(Board& board, int8_t nrLvl)
{
	int count = 0;
	for (int i = 0; i < board.getSize(); i++) {
		if (count >= 2)
		{
			return true;
		}
		for (int j = 0; j < board.getSize(); j++)
		{
			if (board[{i, j}].getValue() == nrLvl) {
				count++;
			}

		}
	}
	return false;
}
