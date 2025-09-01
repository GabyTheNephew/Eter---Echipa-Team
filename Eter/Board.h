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
#include <queue>
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
        InProgress,
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
    std::vector<int> countCardsPerColumn() const;
    std::vector<int> countCardsPerRow() const;

    // Check if board should be fixed horizontally or vertically with configurable target
    bool shouldFixHorizontally(int16_t targetSize = 3) const;
    bool shouldFixVertically(int16_t targetSize = 3) const;

    // Fix the board by removing empty borders when conditions are met
    void fixBoardHorizontally(int16_t targetSize = 3);
    void fixBoardVertically(int16_t targetSize = 3);

    // Updated board management
    void smartBoardManagement(int16_t maxSize);

    void ensureAllCardsHaveAdjacency(int16_t maxSize = 4);
    bool canExpandHorizontally(int16_t maxSize) const;
    bool canExpandVertically(int16_t maxSize) const;

    void asymmetricExpansion(int16_t maxSize = 3);
    bool shouldFixBoard() const;
    void fixBoardBoundaries();
    bool hasThreeInLineColumnDiagonal() const;
    bool hasTwoCardsAtDistance() const;
    void removeIsolatedPositions();
    std::vector<std::pair<int16_t, int16_t>> findConnectedComponent(int16_t startRow, int16_t startCol,
        std::vector<std::vector<bool>>& visited) const;

    /*void ensureAllCardsHaveAdjacency(int16_t maxSize = 4);
    bool canExpandHorizontally() const;
    bool canExpandVertically() const;*/
    int16_t getMinRowWithCards() const;
    int16_t getMaxRowWithCards() const;
    int16_t getMinColWithCards() const;
    int16_t getMaxColWithCards() const;

    bool hasAdjacentCards(int16_t row, int16_t col) const;
    void cleanupIsolatedPositions();
    void cleanupFirstIsolatedPosition();
    int countIsolatedPositions() const;

    Board(const Board& board);
    Board();
    Board(int16_t size);
    Board(Board&& board)noexcept;

    Board& operator=(Board&& board)noexcept;
    Board& operator=(const Board& board);

    void setBoard(const matrix& board);

    std::deque<SimpleCard>& operator[] (const Position& position);
    const std::deque<SimpleCard>& operator [] (const Position& position) const;

    void expandRow(RowExpandDirection direction);
    void expandColumn(ColumnExpandDirection direction);

    std::vector<int16_t> searchEmptyColumns();
    std::vector<int16_t> searchEmptyRows();
    bool canBePlaced(int16_t x, int16_t y)const;

    int getTotalCardsOnBoard() const;
    bool needsExpansion(int16_t placementRow, int16_t placementCol, int16_t maxSize) const;
    void smartExpand(int16_t placementRow, int16_t placementCol, int16_t maxSize);
    bool isRowEmpty(int16_t row) const;
    bool isColumnEmpty(int16_t col) const;
    bool hasCardsInRow(int16_t row) const;
    bool hasCardsInColumn(int16_t col) const;

    bool needsDynamicExpansion(int16_t placementRow, int16_t placementCol, int16_t maxSize) const;
    void dynamicExpand(int16_t placementRow, int16_t placementCol, int16_t maxSize);
    void autoExpandForAdiacency(int16_t maxSize);
    void initializeForDynamicPlay(int16_t maxSize);
    std::pair<int16_t, int16_t> getActualBoardBounds() const;

    State checkWin(bool canCountPoints = false, int16_t boardMaxSize = 3);
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
    bool checkRow(int16_t row);
    bool checkColumn(int16_t column);
    void pushCard(const SimpleCard& card, const Position& position);
    bool canBePushed(const SimpleCard& card, const Position& position)const;
    void popCard(const Position& position);
    void popCardAt(const Position& position, const SimpleCard& targetCard);

    int getNumberOfRowsWithCards()const;
    int getNumberOfColumnsWithCards()const;

    bool isFirstColumnEmpty()const;
    bool isLastColumnEmpty()const;
    bool isFirstRowEmpty()const;
    bool isLastRowEmpty()const;

    int getIndexOfFirstRowOfBoard();
    int getIndexOfLastRowOfBoard();
    int getIndexOfFirstColumnOfBoard();
    int getIndexOfLastColumnOfBoard();

    friend std::ostream& operator<<(std::ostream& os, const Board& board);
    friend std::istream& operator>>(std::istream& in, Board& board);
};