#pragma once
#include <vector>
#include <deque>

typedef std::vector<std::vector<std::deque<int>>> matrix; // TODO: instead of int it should use Card class

class Board
{
private:
	matrix m_board;
public:
	void emptyRow(int row);
	void emptyColumn(int column);

	Board(const Board& board);
};

