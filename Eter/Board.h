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
#include <type_traits>
#include <functional>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <iterator>
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

    template<typename... WinConditions>
    bool checkMultipleWinConditions(int16_t boardMaxSize, WinConditions... conditions) const {
        return (checkSingleWinCondition(boardMaxSize, conditions) || ...);
    }

    template<typename WinCondition>
    bool checkSingleWinCondition(int16_t boardMaxSize, WinCondition condition) const {
        if constexpr (std::is_same_v<WinCondition, std::function<bool()>>) {
            return condition();
        }
        else {
            return false;
        }
    }

    template<typename Operation>
    void forEachPosition(Operation op) {
        for (int16_t i = 0; i < getRowSize(); ++i) {
            for (int16_t j = 0; j < getColumnSize(); ++j) {
                op(i, j, m_board[i][j]);
            }
        }
    }

    template<typename Operation>
    void forEachPosition(Operation op) const {
        for (int16_t i = 0; i < getRowSize(); ++i) {
            for (int16_t j = 0; j < getColumnSize(); ++j) {
                op(i, j, m_board[i][j]);
            }
        }
    }

    template<typename Predicate>
    std::vector<Position> findPositions(Predicate pred) const {
        std::vector<Position> result;
        forEachPosition([&](int16_t row, int16_t col, const auto& stack) {
            if (pred(row, col, stack)) {
                result.emplace_back(row, col);
            }
            });
        return result;
    }

    template<typename Predicate, typename Transform>
    void transformIf(Predicate pred, Transform transform) {
        forEachPosition([&](int16_t row, int16_t col, auto& stack) {
            if (pred(row, col, stack)) {
                transform(row, col, stack);
            }
            });
    }

    template<typename Predicate>
    int countPositions(Predicate pred) const {
        int count = 0;
        forEachPosition([&](int16_t row, int16_t col, const auto& stack) {
            if (pred(row, col, stack)) {
                ++count;
            }
            });
        return count;
    }

    template<typename... Directions>
    bool checkWinInDirections(int16_t boardMaxSize, Directions... directions) const {
        return (checkWinInDirection(boardMaxSize, directions) || ...);
    }

    template<typename Direction>
    bool checkWinInDirection(int16_t boardMaxSize, Direction dir) const {
        if constexpr (std::is_same_v<Direction, std::string>) {
            if (dir == "row") {
                return checkRowWins(boardMaxSize);
            }
            else if (dir == "column") {
                return checkColumnWins(boardMaxSize);
            }
            else if (dir == "diagonal") {
                return checkDiagonalWins(boardMaxSize);
            }
        }
        return false;
    }

    template<typename... Indices>
    void processMultipleRows(std::function<void(int16_t)> operation, Indices... rowIndices) {
        (operation(rowIndices), ...);
    }

    template<typename... Indices>
    void processMultipleColumns(std::function<void(int16_t)> operation, Indices... colIndices) {
        (operation(colIndices), ...);
    }

    template<typename... Positions>
    bool areAllPositionsValid(Positions... positions) const {
        return (isPositionValid(positions) && ...);
    }

    template<typename PositionType>
    bool isPositionValid(PositionType pos) const {
        if constexpr (std::is_same_v<PositionType, Position>) {
            auto [row, col] = pos;
            return row >= 0 && row < getRowSize() && col >= 0 && col < getColumnSize();
        }
        else if constexpr (std::is_same_v<PositionType, std::pair<int16_t, int16_t>>) {
            return pos.first >= 0 && pos.first < getRowSize() &&
                pos.second >= 0 && pos.second < getColumnSize();
        }
        return false;
    }

    template<typename... Operations>
    void processMultipleCards(const SimpleCard& card, Operations... operations) {
        (operations(card), ...);
    }

    template<typename... ExpansionChecks>
    bool needsAnyExpansion(int16_t maxSize, ExpansionChecks... checks) const {
        return (checks(maxSize) || ...);
    }

private:
    bool checkRowWins(int16_t boardMaxSize) const;

    bool checkColumnWins(int16_t boardMaxSize) const;

    bool checkDiagonalWins(int16_t boardMaxSize) const;

public:
    std::vector<int> countCardsPerColumn() const;
    std::vector<int> countCardsPerRow() const;

    bool shouldFixHorizontally(int16_t targetSize = 3) const;
    bool shouldFixVertically(int16_t targetSize = 3) const;

    void fixBoardHorizontally(int16_t targetSize = 3);
    void fixBoardVertically(int16_t targetSize = 3);

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

    void optimizedBoardOperations();
    void smartExpansionWithTemplates(int16_t maxSize);
    bool validateMultiplePositions();
    void optimizedCardProcessing();
    void templateBasedCleanup();

    int16_t sumPointsModern(const Color& color) const;

    std::vector<int16_t> searchEmptyColumnsModern() const;
    std::vector<int16_t> searchEmptyRowsModern() const;

    std::vector<int> countCardsPerColumnModern() const;
    std::vector<int> countCardsPerRowModern() const;

    std::vector<Position> getAllCardPositionsModern() const;
    std::vector<Position> getEmptyPositionsModern() const;
    std::vector<Position> getPositionsByColorModern(const Color& color) const;

    bool hasAnyCardModern() const;
    bool hasCardsInRowModern(int16_t row) const;
    bool hasCardsInColumnModern(int16_t col) const;

    int16_t getTotalCardsModern() const;
    std::pair<int16_t, int16_t> getCardCountByColorModern() const; // {red, blue}

    bool areAllPositionsEmptyModern(const std::vector<Position>& positions) const;
    bool areAnyPositionsEmptyModern(const std::vector<Position>& positions) const;

    template<typename Range, typename Predicate>
    auto countPositionsInRangeModern(Range&& range, Predicate pred) const;

    template<typename Range, typename Transform>
    auto transformPositionsModern(Range&& range, Transform transform) const;

    template<typename Range>
    auto getMaxValueInRangeModern(Range&& range) const;

    template<typename Range>
    auto getMinValueInRangeModern(Range&& range) const;
};