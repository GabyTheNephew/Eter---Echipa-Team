#include "PowerLava.h"

PowerLava::PowerLava(std::string_view name, std::string_view description):
	m_name{ "Chill Thoughts" },
	m_description{ "On his turn, a Mage may move an opponent’s card (or a stack of cards with an opponent’s card) to a different, empty space." }
{
}

std::string PowerLava::getName() const
{
	return std::string();
}

std::string PowerLava::getDescription() const
{
	return std::string();
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
