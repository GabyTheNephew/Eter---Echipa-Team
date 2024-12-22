#include "PowerAvalanche.h"

const std::string PowerAvalanche::m_name = "Avalanche";
const std::string PowerAvalanche::m_description = "Move two neighboring cards / stacs by one space horizontally or vertically.The space that the first card moves onto must be empty.";

PowerAvalanche::PowerAvalanche()
{
}

std::string_view PowerAvalanche::getName() const
{
	return m_name;
}

std::string_view PowerAvalanche::getDescription() const
{
	return m_description;
}

bool PowerAvalanche::checkAvalanchePower(Board& board, bool rowColumn, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{

	return false;
}

void PowerAvalanche::playAvalanchePower(Board& board, Player& player, bool rowColumn, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	board.moveSpace(x1, y1, x2, y2);
	///row == false
	///column == true


}

