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

bool PowerHurricane::checkHurricanePower(Board& board, Player& player)
{
    for (int16_t row = 0; row < board.getRowSize(); ++row) {
        if (board.checkRow(row)) {
            return true;
        }
    }
    for (int16_t col = 0; col < board.getColumnSize(); ++col) {
        if (board.checkColumn(col)) {
            return true;
        }
    }

    
    return false;

}

void PowerHurricane::playHurricanePower(Board& board, Player& player)
{
	if (checkHurricanePower(board, player))
	{
        int16_t choice;
        std::cout << "Choose an option: \n1. Move Row\n2. Move Column\n";
        std::cin >> choice;
		if (choice == 1)
		{
			int16_t row;
			std::cout << "Enter the row you want to move from 1 to 3: ";
			std::cin >> row;
			row -= 1;
			if (row < 0 || row >= board.getRowSize() || !board.checkRow(row))
			{
				std::cout << "Invalid row\n";
				return;
			}
			auto& lastCardStack = board[{row, board.getColumnSize() - 1}];
			auto pastcards = player.getPastVector();
			for (auto& card : lastCardStack)
			{
				player.makeCardValid(card);
				pastcards.erase(std::remove(pastcards.begin(), pastcards.end(), card), pastcards.end());
			}
			lastCardStack.clear();
			player.setPastVector(pastcards);

			for (int16_t col = board.getColumnSize() - 1; col > 1; col--)
			{
				board[{row, col}] = std::move(board[{row,col-1}]);
			}
			board[{row, 0}].clear();
        }
        else
			if (choice == 2)
			{
				int16_t col;
				std::cout << "Enter the column you want to move from 1 to 3: ";
				std::cin >> col;
				col -= 1;
				if (col < 0 || col >= board.getColumnSize())
				{
					std::cout << "Invalid column\n";
					return;
				}
				auto& lastCardStack = board[{board.getRowSize() - 1,col}];
				auto pastcards = player.getPastVector();
				for (auto& card : lastCardStack)
				{
					player.makeCardValid(card);
					pastcards.erase(std::remove(pastcards.begin(), pastcards.end(), card), pastcards.end());
				}
				lastCardStack.clear();
				player.setPastVector(pastcards);

				for (int16_t row = board.getColumnSize() - 1; row > 1; row--)
				{
					board[{row, col}] = std::move(board[{row-1, col}]);
				}
				board[{0, col}].clear();


			}
			else
			{
				std::cout << "Invalid choice\n";
				return;
			}
	}
    else
    {
		std::cout << "No row or column is full\n";
        return;
    }
    
}
