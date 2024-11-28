#include "PowerCutremur.h"

PowerCutremur::PowerCutremur(std::string_view name, std::string_view description) :
	m_name{ "Vifor" },
	m_description{ "Întoarce o carte vizibilă a oponentului în mâna sa." }
{
}

std::string PowerCutremur::getName() const
{
	return m_name;
}

std::string PowerCutremur::getDescription() const
{
	return m_description;
}

void PowerCutremur::playPower(Board& board)
{

	for (int i = 0; i < board.getSize(); i++)
		for (int j = 0; j < board.getSize(); j++)
		{
			if (board[{i, j}].value().getValue() == 1) {

				//pop the card
			}
		}
}

bool PowerCutremur::checkPower()
{
	return true;

}
