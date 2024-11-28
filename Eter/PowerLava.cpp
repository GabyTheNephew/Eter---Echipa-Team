#include "PowerLava.h"

PowerLava::PowerLava(std::string_view name, std::string_view description):
	m_name{ "Lava" },
	m_description{ " Alege un număr cu condiția că cel puțin 2 cărți cu acel număr sunt vizibile pe tablă. Toate cărțile vizibile cu acest număr se întorc în mâinile proprietarilor." }
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
			if (board[{i, j}].value().getValue() == nrLvl) {
				if (ColorToString(board[{i, j}].value().getColor()) == "Red") {
					player1.makeCardValid(nrLvl, Color::Red);
					//pop the card that came back to a player
				}

				if (ColorToString(board[{i, j}].value().getColor()) == "Blue") {
					player1.makeCardValid(nrLvl, Color::Blue);
					//pop the card that came back to a player
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
			if (board[{i, j}].value().getValue() == nrLvl) {
				count++;
			}

		}
	}
	return false;
}
