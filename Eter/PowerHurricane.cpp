#include "PowerHurricane.h"


const std::string m_name = "Hurricane";
const std::string m_description = "Move any one full row / column by onecard so that the first card /stack is moved off the playing field.Cards moved outside the playing field return to their owners’ hands.";
PowerHurricane::PowerHurricane()
{
}

std::string_view PowerHurricane::getName() const
{
	return m_name;
}

std::string_view PowerHurricane::getDescription() const
{
	return m_description;
}

bool PowerHurricane::checkHurricanePower(Board& board)
{
    for (int16_t row = 0; row < board.getRowSize(); ++row) {
        if (board.checkRow(row)) 
		{
            return true;
        }
    }
    for (int16_t col = 0; col < board.getColumnSize(); ++col) {
        if (board.checkColumn(col))
		{
            return true;
        }
    }

    
    return false;

}


bool PowerHurricane::playHurricanePower(Board& board, Player& player1, Player& player2, bool isRow, int16_t index, int16_t direction)
{
	if (!checkHurricanePower(board))
	{
		return false;
	}


	if (isRow && (index < 0 || index >= board.getRowSize() || !board.checkRow(index)))
	{
		return false;
	}
	if (!isRow && (index < 0 || index >= board.getColumnSize() || !board.checkColumn(index)))
	{
		return false;
	}

	
	int dx[] = { 0, 0, -1, 1 };
	int dy[] = { -1, 1, 0, 0 };


	std::vector<std::pair<int, int>> positions;
	if (isRow)
	{
		for (int col = 0; col < board.getColumnSize(); col++)
		{
			positions.push_back({ index, col });
		}
	}
	else
	{
		for (int row = 0; row < board.getRowSize(); row++)
		{
			positions.push_back({ row, index });
		}
	}
	std::vector<std::deque<SimpleCard>> allStacks;
	for (auto& pos : positions)
	{
		allStacks.push_back(board[{pos.first, pos.second}]);
		board[{pos.first, pos.second}].clear();
	}


	for (int i = 0; i < positions.size(); i++)
	{
		int newX = positions[i].first + dx[direction];
		int newY = positions[i].second + dy[direction];


		if (newX < 0 || newX >= board.getRowSize() || newY < 0 || newY >= board.getColumnSize())
		{
			for (auto& card : allStacks[i])
			{
				if (card.getColor() == Color::Red || card.getColor() == Color::increasedRed || card.getColor() == Color::decreasedRed)
				{
					SimpleCard returnCard(card.getValue(), Color::Red);
					player1.makeCardValid(returnCard);
				}
				else if (card.getColor() == Color::Blue || card.getColor() == Color::increasedBlue || card.getColor() == Color::decreasedBlue)
				{
					SimpleCard returnCard(card.getValue(), Color::Blue);
					player2.makeCardValid(returnCard);
				}
			}
		}
		else
		{
	
			board[{newX, newY}] = std::move(allStacks[i]);
		}
	}

	return true;
}