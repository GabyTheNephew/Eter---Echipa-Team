#include "PowerVifor.h"

PowerVifor::PowerVifor(std::string_view name, std::string_view description) :
	m_name{ "Vifor" },
	m_description{ "Întoarce o carte vizibilă a oponentului în mâna sa." }
{
}

std::string PowerVifor::getName() const
{
	return m_name;
}

std::string PowerVifor::getDescription() const
{
	return m_description;
}

void PowerVifor::playPower(Board& board, Player& player1, Player& player2, uint8_t x,uint8_t y)//nrLvl - numarul de putere petru o carte
{
	
	if (ColorToString(board[{x, y}].value().getColor()) == "Red") {
		player1.makeCardValid(board[{x, y}].value().getValue(), Color::Red);
		//pop the card that came back to a player
	}

	if (ColorToString(board[{x,y}].value().getColor()) == "Blue") {
		player1.makeCardValid(board[{x, y}].value().getValue(), Color::Blue);
		//pop the card that came back to a player
	}
		
}

bool PowerVifor::checkPower(Board& board, uint8_t x, uint8_t y)
{
	return board[{x, y}].has_value();

}
