#include "PowerGust.h"

PowerGust::PowerGust()
{
}

std::string_view PowerGust::getName() const
{
	return std::string_view();
}

std::string_view PowerGust::getDescription() const
{
	return std::string_view();
}

bool PowerGust::checkPower(Board& board,uint8_t x,uint8_t y)
{
	if (board[{x, y}].empty()) return false;
	int currentValue = board[{x, y}].front().getValue();


	std::vector<std::pair<int8_t, int8_t>> neighbors = {
	   {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}
	};


	for (const auto& [nx, ny] : neighbors) {
		if (nx >= 0 && ny >= 0 && nx < board.getRowSize() && ny < board.getColumnSize())
		{
			if (!board[{nx, ny}].empty() && board[{nx, ny}].front().getValue() < currentValue)
			{
				return true;
			}
		}
	}
}

void PowerGust::playPower(Board& board)
{
	std::cout << "Here you have all the cards on the board:\n";
	board.print();
	std::cout << "Type the cooridnates of the card you want to move\n";
	int16_t x, y;
	std::cin >> x >> y;

	if (x < 0 || y < 0 || x >= board.getRowSize() || y >= board.getColumnSize() || board[{x, y}].empty()) {
		std::cout << "Invalid coordinates or no card at the position\n";
		return;
	}


	if (!checkPower(board, x, y)) {
		std::cout << "This card cannot be moved as no valid moves exist.\n";
		return;
	}

	SimpleCard cardToMove = board[{x, y}].front();
	std::cout << "Available positions to move:\n";


	std::vector<std::pair<int8_t, int8_t>> neighbors = {
	   {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}
	};

	std::vector<std::pair<int8_t, int8_t>> validMoves;

	for (const auto& [nx, ny] : neighbors) 
	{
		if (nx >= 0 && ny >= 0 && nx < board.getRowSize() && ny < board.getColumnSize()) 
		{
			if (!board[{nx, ny}].empty() && board[{nx, ny}].front().getValue() < cardToMove.getValue()) 
			{
				validMoves.emplace_back(nx, ny);
				std::cout << "Positions: " << nx << " " << ny << "\n";
			}
		}
	}

	int8_t newX, newY;
	std::cout << "Enter the coordinates of the position you want to move to:\n";
	while (true)
	{
		std::cin >> newX >> newY;
		if (std::find(validMoves.begin(), validMoves.end(), std::make_pair(newX,newY)) != validMoves.end())
		{
			board.pushCard(cardToMove, { newX, newY });
		}
		else
		{
			std::cout << "Invalid move. Please choose a valid position:\n";
		}
	}
	
}
