#pragma once
#include <vector>
#include <deque>
#include <iostream>

typedef std::vector<std::vector<std::deque<int>>> matrix; // TODO: instead of int it should use Card class

// TODO: destructor, clear(), operator<< and operator>>

class Board
{
private:
	matrix m_board;
public:
	Board(const Board& board);
	Board();//defaul constuctor
	Board(uint8_t size);
	Board(Board&& board)noexcept;

	Board& operator=(Board&& board)noexcept;
	Board& operator=(const Board& board);

	void emptyRow(int row);
	void emptyColumn(int column);
	void resizeBoard(int size);
	void print();
	void clear();
};

