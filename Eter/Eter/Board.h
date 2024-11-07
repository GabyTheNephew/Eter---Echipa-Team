#pragma once
#include <vector>
#include <deque>
#include <iostream>
#include "SimpleCard.h"

typedef std::vector<std::vector<std::deque<SimpleCard>>> matrix;

// TODO: multiple expand functions so that the board can be expanded in multiple directions

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
	
	matrix& getBoard() const;  
	void setBoard(const matrix& board); 

	void expand(uint8_t newRows);
	void emptyRow(int row);
	void emptyColumn(int column);
	void resizeBoard(int size);
	void print();
	void clear();
	friend std::ostream& operator<<(std::ostream& os, const Board& board);
	friend std::istream& operator>>(std::istream& in, Board& board); // this is just for test
};

