#pragma once
#include <vector>
#include <deque>
#include <iostream>
#include "SimpleCard.h"
#include <array>
#include <optional>
#include <tuple>
#include "CardColor.h"
#include <QDebug>

using matrix = std::vector<std::vector<std::deque<SimpleCard>>>;

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
#pragma region Board Methods

	Board(const Board& board);
	Board();
	Board(int16_t size);
	Board(Board&& board)noexcept;
	Board& operator=(Board&& board)noexcept;
	Board& operator=(const Board& board);

	std::deque<SimpleCard>& operator[] (const Position& position);
	const std::deque<SimpleCard>& operator [] (const Position& position) const;
#pragma endregion

#pragma region Search Methods
	std::vector<int16_t> searchEmptyColumns();
	std::vector<int16_t> searchEmptyRows();
	bool canBePlaced(int16_t x, int16_t y)const;
	void emptyRow(int16_t row);
	void emptyColumn(int16_t column);
#pragma endregion

#pragma region Alter Methods
	void moveSpace(int16_t row, int16_t column, int16_t newRow, int16_t newColumn);
	void removeRow(int16_t row);
	void removeColumn(int16_t column);
	void resizeBoard(int16_t size);
	void print()const;
	void clear();
	void expandRow(RowExpandDirection direction);
	void expandColumn(ColumnExpandDirection direction);
	void setBoard(const matrix& board);

	bool canBePushed(const SimpleCard& card, const Position& position)const;
	void popCard(const Position& position);
	void popCardAt(const Position& position, const SimpleCard& targetCard);
#pragma endregion

#pragma region Getters
	int16_t getSize()const;
	int16_t getRowSize()const;
	int16_t getColumnSize()const;
	int getIndexOfFirstRowOfBoard();
	int getIndexOfLastRowOfBoard();
	int getIndexOfFirstColumnOfBoard();
	int getIndexOfLastColumnOfBoard();
	int getNumberOfRowsWithCards()const;
	int getNumberOfColumnsWithCards()const;
	bool isFirstColumnEmpty()const;
	bool isLastColumnEmpty()const;
	bool isFirstRowEmpty()const;
	bool isLastRowEmpty()const;

#pragma endregion

#pragma region Checking Methods
	State checkWin(bool canCountPoints = false, int16_t boardMaxSize = 3);
	int16_t sumPoints(const Color& color);
	bool checkRow(int16_t row);
	bool checkColumn(int16_t column);
	void pushCard(const SimpleCard& card, const Position& position);
#pragma endregion



	friend std::ostream& operator<<(std::ostream& os, const Board& board);
	friend std::istream& operator>>(std::istream& in, Board& board); 
};

