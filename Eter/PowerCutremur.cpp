#include "PowerCutremur.h"

PowerCutremur::PowerCutremur(std::string_view name, std::string_view description) :
	m_name{ " EARTHQUAKE" },
	m_description{ "Remove from play all visible ‘1’ cards." }
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
			if (board[{i, j}].getValue() == 1) {

				board.popCard({ i,j });
			}
		}
}

bool PowerCutremur::checkPower()
{
	return true;

}
