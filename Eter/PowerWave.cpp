#include "PowerWave.h"


static const std::string m_name = "Wave";
static const std::string m_description = "Move any card/stack to the adjacent empty space,then play a card from your hand to this now empty space.";

PowerWave::PowerWave()
{
}

std::string_view PowerWave::getName() const
{
	return m_name;
}

std::string_view PowerWave::getDescription() const
{
	return m_description;
}

bool PowerWave::checkWavePower(Board& board, Player& player, int16_t x, int16_t y)
{
	if (board[{x, y}].empty())
	{
		return false;
	}
	std::vector<std::pair<int, int>> neighbors = {
		{x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1},
		{x + 1, y + 1}, {x - 1, y + 1}, {x - 1, y - 1}, {x + 1, y - 1}
	};


	for (const auto& [nx, ny] : neighbors) {
		if (nx >= 0 && nx < board.getRowSize() && ny >= 0 && ny < board.getColumnSize()) {
			if (board[{nx,ny}].empty()) {
				return true;
			}
		}
	}
	return false;
}

void PowerWave::playWavePower(Board& board, Player& player, int16_t x, int16_t y)
{

	if (checkWavePower(board, player, x, y))
	{
		printEmptyAdjacentStacks(board, x, y);

		int16_t newX, newY;
		std::cout << "Enter the coordinates of the empty space you want to move the stack to:\n";
		std::cin >> newX >> newY;

		if (newX < 0 || newX >= board.getRowSize() || newY < 0 || newY >= board.getColumnSize() || !board[{newX, newY}].empty())
		{
			std::cout << "Invalid coordinates, try again" << std::endl;
			return;
		}

		board.moveSpace(x, y, newX, newY);
		std::cout << "Stack moved successfully\n";
		auto card = player.chooseCard();
		
		if (board.canBePlaced(x, y))
		{
			board.pushCard(card, { x, y });
			player.makeCardInvalid(card);
		}
	}
	else
	{
		std::cout << "Invalid move, try again" << std::endl;
	}
}

void PowerWave::printEmptyAdjacentStacks(Board& board, int16_t x, int16_t y)
{
	std::cout << "Empty adjacent positions (including corners) for stack at (" << x << ", " << y << "):" << std::endl;

	std::vector<Board::Position> neighbors = {
		{x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1},
		{x + 1, y + 1}, {x - 1, y - 1}, {x + 1, y - 1}, {x - 1, y + 1}
	};

	bool found = false;
	for (const auto& [nx, ny] : neighbors)
	{
		if (nx >= 0 && nx < board.getRowSize() &&
			ny >= 0 && ny < board.getColumnSize() &&
			board[{nx, ny}].empty())
		{
			std::cout << "- (" << nx << ", " << ny << ")" << std::endl;
			found = true;
		}
	}

	if (!found)
	{
		std::cout << "No empty adjacent positions found." << std::endl;
	}
}
