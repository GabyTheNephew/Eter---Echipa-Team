#pragma once
#include <vector>
#include <deque>
#include <iostream>
#include "SimpleCard.h"
#include <array>
#include <optional>
#include <tuple>

typedef std::vector<std::vector<std::deque<std::optional<SimpleCard>>>> matrix;

// TODO: refactoring so that I use ranges and iterators instead of basic i and j when going trough matrix
// TODO: calculate points
// TODO: is a line with a color?
// TODO: test the expand methods

class Board
{
private:
	matrix m_board;

public:
	enum class State
	{
		None,
		Win,
		Draw
	};

	using Position = std::tuple<uint8_t, uint8_t>;

public:
	Board(const Board& board);
	Board();//defaul constuctor
	Board(uint8_t size);
	Board(Board&& board)noexcept;

	Board& operator=(Board&& board)noexcept;
	Board& operator=(const Board& board);
	
	matrix& getBoard();  
	void setBoard(const matrix& board); 
	std::optional<SimpleCard>& operator[] (const Position& position);
	const std::optional<SimpleCard>& operator [] (const Position& position) const;

	void expandLeftUpCorner();
	void expandRightUpCorner();
	void expandLeftBottomCorner();
	void expandRightBottomCorner();

	void expandRight();
	void expandLeft();
	void expandDown();
	void expandUp();

	std::vector<uint8_t> searchEmptyColumns(); //returns a vector with indices to empty cols
	std::vector<uint8_t> searchEmptyRows(); //returns a vector with indices to empty rows
	bool canBePlaced(int x,int y)const;

	/*bool lineWithColor(std::string_view Color)const;
	bool columnWithColor(std::string_view Color)const;
	bool diagonalWithColor(std::string_view Color)const;*/
	State checkWin();


	void moveSpace(uint8_t row, uint8_t column, uint8_t newRow, uint8_t newColumn);
	void emptyRow(uint8_t row);
	void emptyColumn(uint8_t column);
	void removeRow(uint8_t row);
	void removeColumn(uint8_t column);
	// void resizeBoard(uint8_t size);
	void print()const;
	void clear();
	bool checkRow(uint8_t row);
	bool checkColumn(uint8_t column);

	friend std::ostream& operator<<(std::ostream& os, const Board& board);
	friend std::istream& operator>>(std::istream& in, Board& board); // this is just for test
};

