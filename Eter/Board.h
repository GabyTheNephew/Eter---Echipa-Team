#pragma once
#include <vector>
#include <deque>
#include <iostream>
#include "SimpleCard.h"
#include <array>
#include <optional>
#include <tuple>
#include "CardColor.h"


using matrix = std::vector<std::vector<std::deque<SimpleCard>>>;

// TODO: refactoring so that I use ranges and iterators instead of basic i and j when going trough matrix
// TODO: refactor the code so that it uses CardColor enum instead of std::string
// TODO: fix usedCard mecanic
//comment commit
class Board
{
private:
	matrix m_board;
	int16_t m_size;
public:
	enum class State
	{
		None,
		Win,
		RedWin,
		BlueWin,
		Draw
	};

	enum class RowExpandDirection
	{
		Down,
		Up
	};

	enum class ColumnExpandDirection
	{
		Left,
		Right
	};

	using Position = std::tuple<int16_t, int16_t>;

public:
	Board(const Board& board);
	Board();//defaul constuctor
	Board(int16_t size);
	Board(Board&& board)noexcept;

	Board& operator=(Board&& board)noexcept;
	Board& operator=(const Board& board);
	
	// matrix& getBoard();  
	void setBoard(const matrix& board); 

	std::deque<SimpleCard>& operator[] (const Position& position);
	const std::deque<SimpleCard>& operator [] (const Position& position) const;

	void expandRow(RowExpandDirection direction);
	void expandColumn(ColumnExpandDirection direction);

	std::vector<int16_t> searchEmptyColumns(); //returns a vector with indices to empty cols
	std::vector<int16_t> searchEmptyRows(); //returns a vector with indices to empty rows
	bool canBePlaced(int16_t x, int16_t y)const;

	/*bool lineWithColor(std::string_view Color)const;
	bool columnWithColor(std::string_view Color)const;
	bool diagonalWithColor(std::string_view Color)const;*/
	State checkWin(bool canCountPoints = false);
	int16_t sumPoints(const Color& color);

	int16_t getSize()const;
	int16_t getRowSize()const;
	int16_t getColumnSize()const;
	void moveSpace(int16_t row, int16_t column, int16_t newRow, int16_t newColumn);
	void emptyRow(int16_t row);
	void emptyColumn(int16_t column);
	void removeRow(int16_t row);
	void removeColumn(int16_t column);
	void resizeBoard(int16_t size);
	void print()const;
	void clear();
	bool checkRow(int16_t row); // checks if a row is full , for Explosions
	bool checkColumn(int16_t column); // checks if a column is full , for Explosions
	void pushCard(const SimpleCard& card, const Position& position);
	bool canBePushed(const SimpleCard& card, const Position& position)const;
	void popCard(const Position& position);
	void popCardAt(const Position& position, const SimpleCard& targetCard);

	friend std::ostream& operator<<(std::ostream& os, const Board& board);
	friend std::istream& operator>>(std::istream& in, Board& board); // this is just for test
};

