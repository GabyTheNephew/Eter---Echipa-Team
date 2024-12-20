#include "PowerWaterfall.h"

const std::string PowerWaterfall::m_name = "Waterfall";
const std::string PowerWaterfall::m_description = "Move all cards in a chosen column into one stack at the bottom of that column, placing them on top ofeach other, regardless of their values.The card that was visible on the top of the chosen column stays at the top of thenew stack.Condition - the chosen column must containof minimum 3 cards.";




PowerWaterfall::PowerWaterfall()
{

}

std::string_view PowerWaterfall::getName() const
{
	return m_name;
}

std::string_view PowerWaterfall::getDescription() const
{
	return m_description;
}

bool PowerWaterfall::checkWaterfallPower(Board& board)
{
	for (int16_t i = 0; i < board.getRowSize(); i++)
	{
		if (board.checkColumn(i))
		{
			return true;
		}
	}

	for (int16_t i = 0; i < board.getColumnSize(); i++)
	{
		if (board.checkRow(i))
		{
			return true;
		}
	}
	return false;
}

void PowerWaterfall::PrintColumns(Board& board)
{
	std::cout << "Columns with at least 3 pozitions occupied: \n";

	for (int16_t i = 0; i < board.getRowSize(); i++)
	{
		if (board.checkColumn(i))
		{
			std::cout << "Column " << i << " has at least 3  pozitions occupied.\n";
		}
	}
}

void PowerWaterfall::PrintRows(Board& board)
{
	
	std::cout << "Rows with at least 3 pozitions occupied: \n";
	for (int16_t i = 0; i < board.getColumnSize(); i++)
	{
		if (board.checkRow(i))
		{
			std::cout << "Row " << i << " has at least 3  pozitions occupied.\n";
		}
	}
}

void PowerWaterfall::playWaterfallPower(Board& board, Player& player)
{
	if (checkWaterfallPower(board))
	{
		PrintColumns(board);
		PrintRows(board);
		int16_t choice;
		std::cout << "Chose what you want to move(1 for row , 2 for column): \n";
		std::cin >> choice;

		if (choice != 1 && choice != 2) {
			std::cout << "Invalid Choice.\n";
			return;
		}
		std::cout << "Enter the index of the row/column you want to move: \n";
		int16_t index;
		std::cin >> index;

		std::cout << "Pick the direction you want to move the cards(1 for beginig,2 for end): \n";
		int16_t direction;
		std::cin >> direction;

		std::deque<SimpleCard> mergedStack;
		if (choice == 1)
		{
			if (index < 0 || index >= board.getColumnSize()|| board.canBePlaced(index,0)|| board.canBePlaced(index, (board.getColumnSize()-1)))
			{
				std::cout << "Invalid line index.\n";
				return;
			}

			if (direction == 1)
			{
				
				for (int16_t i = 1; i < board.getRowSize(); i++)
				{
					mergedStack.insert(mergedStack.end(), board[{index, i}].begin(), board[{index, i}].end());
					board[{index, i}].clear();
				}
				auto& topStack = board[{index, 0}];
				mergedStack.insert(mergedStack.end(), topStack.begin(), topStack.end());
				topStack.clear();
				board[{index, 0}] = std::move(mergedStack);
			}
			else
				if(direction==2)
			{
					for (int16_t i = 0; i < board.getRowSize()-1; i++)
					{
						mergedStack.insert(mergedStack.end(), board[{index, i}].begin(), board[{index, i}].end());
						board[{index, i}].clear();
					}
					auto& topStack = board[{index, board.getRowSize()-1}];
					mergedStack.insert(mergedStack.end(), topStack.begin(), topStack.end());
					topStack.clear();
					board[{index, board.getRowSize()-1}] = std::move(mergedStack);
			}
	}
		else
			if (choice == 2)
			{
				if (index < 0 || index >= board.getRowSize() || board.canBePlaced(index, 0) || board.canBePlaced(index, (board.getRowSize() - 1)))
				{
					std::cout << "Invalid line index.\n";
					return;
				}

				if (direction == 1)
				{

					for (int16_t i = 1; i < board.getColumnSize(); i++)
					{
						mergedStack.insert(mergedStack.end(), board[{i, index}].begin(), board[{ i, index}].end());
						board[{ i, index}].clear();
					}
					auto& topStack = board[{ 0, index}];
					mergedStack.insert(mergedStack.end(), topStack.begin(), topStack.end());
					topStack.clear();
					board[{ 0, index}] = std::move(mergedStack);
				}
				else
					if (direction == 2)
					{
						for (int16_t i = 0; i < board.getColumnSize() - 1; i++)
						{
							mergedStack.insert(mergedStack.end(), board[{i, index}].begin(), board[{i, index}].end());
							board[{i, index}].clear();
						}
						auto& topStack = board[{(board.getColumnSize() - 1), index }];
						mergedStack.insert(mergedStack.end(), topStack.begin(), topStack.end());
						topStack.clear();
						board[{ (board.getRowSize() - 1), index}] = std::move(mergedStack);
					}
			}
	else
	{
		std::cout << "No column has at least 3 cards." << std::endl;
	}
}
