#include "Board.h"

int16_t Board::sumPointsModern(const Color& color) const {
    using namespace std::ranges;

    std::vector<SimpleCard> allCards;

    for (auto [row, col] : views::cartesian_product(
        views::iota(0, getRowSize()),
        views::iota(0, getColumnSize())
    )) {
        if (!m_board[row][col].empty()) {
            allCards.push_back(m_board[row][col].back());
        }
    }

    auto matchingCards = allCards | views::filter([color](const SimpleCard& card) {
        Color cardColor = card.getColor();
        return (color == Color::Red && (cardColor == Color::Red || cardColor == Color::IlusionRed)) ||
            (color == Color::Blue && (cardColor == Color::Blue || cardColor == Color::IlusionBlue));
        });

    return std::accumulate(matchingCards.begin(), matchingCards.end(), int16_t{ 0 },
        [](int16_t sum, const SimpleCard& card) {
            if (card.getColor() == Color::IlusionBlue || card.getColor() == Color::IlusionRed) {
                return sum + 1;
            }
            return sum + card.getValue();
        });
}
std::vector<int16_t> Board::searchEmptyColumnsModern() const {
    using namespace std::ranges;

    auto columnIndices = views::iota(0, getColumnSize());

    std::vector<int16_t> result;
    result.reserve(getColumnSize());

    for (auto col : columnIndices | views::filter([this](int16_t col) {
        auto columnCells = views::iota(0, getRowSize()) | views::transform([this, col](int16_t row) {
            return m_board[row][col].empty();
            });
        return all_of(columnCells, std::identity{});
        })) {
        result.push_back(col);
    }

    return result;
}

std::vector<int16_t> Board::searchEmptyRowsModern() const {
    using namespace std::ranges;

    auto rowIndices = views::iota(0, getRowSize());

    std::vector<int16_t> result;
    result.reserve(getRowSize());

    for (auto row : rowIndices | views::filter([this](int16_t row) {
        auto rowCells = views::iota(0, getColumnSize()) | views::transform([this, row](int16_t col) {
            return m_board[row][col].empty();
            });
        return all_of(rowCells, std::identity{});
        })) {
        result.push_back(row);
    }

    return result;
}

std::vector<int> Board::countCardsPerColumnModern() const {
    using namespace std::ranges;

    auto columnIndices = views::iota(0, getColumnSize());

    std::vector<int> result;
    result.reserve(getColumnSize());

    for (auto count : columnIndices | views::transform([this](int16_t col) {
        auto columnCells = views::iota(0, getRowSize()) | views::transform([this, col](int16_t row) {
            return !m_board[row][col].empty();
            });
        return static_cast<int>(std::ranges::count(columnCells, true));
        })) {
        result.push_back(count);
    }

    return result;
}

std::vector<int> Board::countCardsPerRowModern() const {
    using namespace std::ranges;

    auto rowIndices = views::iota(0, getRowSize());

    std::vector<int> result;
    result.reserve(getRowSize());

    for (auto count : rowIndices | views::transform([this](int16_t row) {
        auto rowCells = views::iota(0, getColumnSize()) | views::transform([this, row](int16_t col) {
            return !m_board[row][col].empty();
            });
        return static_cast<int>(std::ranges::count(rowCells, true));
        })) {
        result.push_back(count);
    }

    return result;
}
std::vector<Board::Position> Board::getAllCardPositionsModern() const {
    using namespace std::ranges;

    auto allPositions = views::cartesian_product(
        views::iota(0, getRowSize()),
        views::iota(0, getColumnSize())
    );

    std::vector<Position> result;
    result.reserve(getRowSize() * getColumnSize());

    for (auto pos : allPositions | views::filter([this](const auto& pos) {
        auto [row, col] = pos;
        return !m_board[row][col].empty();
        })) {
        auto [row, col] = pos;
        result.emplace_back(row, col);
    }

    return result;
}

std::vector<Board::Position> Board::getEmptyPositionsModern() const {
    using namespace std::ranges;

    auto allPositions = views::cartesian_product(
        views::iota(0, getRowSize()),
        views::iota(0, getColumnSize())
    );

    std::vector<Position> result;
    result.reserve(getRowSize() * getColumnSize());

    for (auto pos : allPositions | views::filter([this](const auto& pos) {
        auto [row, col] = pos;
        return m_board[row][col].empty();
        })) {
        auto [row, col] = pos;
        result.emplace_back(row, col);
    }

    return result;
}
std::vector<Board::Position> Board::getPositionsByColorModern(const Color& color) const {
    using namespace std::ranges;

    auto allPositions = views::cartesian_product(
        views::iota(0, getRowSize()),
        views::iota(0, getColumnSize())
    );

    std::vector<Position> result;
    result.reserve(getRowSize() * getColumnSize());

    for (auto pos : allPositions | views::filter([this, color](const auto& pos) {
        auto [row, col] = pos;
        if (m_board[row][col].empty()) return false;

        Color cardColor = m_board[row][col].back().getColor();
        return (color == Color::Red && (cardColor == Color::Red || cardColor == Color::IlusionRed)) ||
            (color == Color::Blue && (cardColor == Color::Blue || cardColor == Color::IlusionBlue));
        })) {
        auto [row, col] = pos;
        result.emplace_back(row, col);
    }

    return result;
}
bool Board::hasAnyCardModern() const {
    using namespace std::ranges;

    auto allPositions = views::cartesian_product(
        views::iota(0, getRowSize()),
        views::iota(0, getColumnSize())
    );

    return any_of(allPositions, [this](const auto& pos) {
        auto [row, col] = pos;
        return !m_board[row][col].empty();
        });
}

bool Board::hasCardsInRowModern(int16_t row) const {
    using namespace std::ranges;

    if (row < 0 || row >= getRowSize()) return false;

    auto rowCells = views::iota(0, getColumnSize());

    return any_of(rowCells, [this, row](int16_t col) {
        return !m_board[row][col].empty();
        });
}

bool Board::hasCardsInColumnModern(int16_t col) const {
    using namespace std::ranges;

    if (col < 0 || col >= getColumnSize()) return false;

    auto columnCells = views::iota(0, getRowSize());

    return any_of(columnCells, [this, col](int16_t row) {
        return !m_board[row][col].empty();
        });
}

int16_t Board::getTotalCardsModern() const {
    using namespace std::ranges;

    auto allPositions = views::cartesian_product(
        views::iota(0, getRowSize()),
        views::iota(0, getColumnSize())
    );

    auto nonEmptyPositions = allPositions | views::filter([this](const auto& pos) {
        auto [row, col] = pos;
        return !m_board[row][col].empty();
        });

    return static_cast<int16_t>(distance(nonEmptyPositions.begin(), nonEmptyPositions.end()));
}

std::pair<int16_t, int16_t> Board::getCardCountByColorModern() const {
    using namespace std::ranges;

    auto allPositions = views::cartesian_product(
        views::iota(0, getRowSize()),
        views::iota(0, getColumnSize())
    );

    auto cards = allPositions | views::filter([this](const auto& pos) {
        auto [row, col] = pos;
        return !m_board[row][col].empty();
        }) | views::transform([this](const auto& pos) {
            auto [row, col] = pos;
            return m_board[row][col].back().getColor();
            });

        int16_t redCount = static_cast<int16_t>(count_if(cards, [](Color c) {
            return c == Color::Red || c == Color::IlusionRed;
            }));

        int16_t blueCount = static_cast<int16_t>(count_if(cards, [](Color c) {
            return c == Color::Blue || c == Color::IlusionBlue;
            }));

        return { redCount, blueCount };
}

bool Board::areAllPositionsEmptyModern(const std::vector<Position>& positions) const {
    using namespace std::ranges;

    return all_of(positions, [this](const Position& pos) {
        auto [row, col] = pos;
        return row >= 0 && row < getRowSize() &&
            col >= 0 && col < getColumnSize() &&
            m_board[row][col].empty();
        });
}

bool Board::areAnyPositionsEmptyModern(const std::vector<Position>& positions) const {
    using namespace std::ranges;

    return any_of(positions, [this](const Position& pos) {
        auto [row, col] = pos;
        return row >= 0 && row < getRowSize() &&
            col >= 0 && col < getColumnSize() &&
            m_board[row][col].empty();
        });
}

template<typename Range, typename Predicate>
auto Board::countPositionsInRangeModern(Range&& range, Predicate pred) const {
    using namespace std::ranges;
    return count_if(std::forward<Range>(range), pred);
}

template<typename Range, typename Transform>
auto Board::transformPositionsModern(Range&& range, Transform transform) const {
    using namespace std::ranges;
    using ValueType = std::decay_t<decltype(transform(*range.begin()))>;

    std::vector<ValueType> result;
    result.reserve(std::distance(range.begin(), range.end()));

    for (auto&& item : std::forward<Range>(range) | views::transform(transform)) {
        result.push_back(std::forward<decltype(item)>(item));
    }

    return result;
}

template<typename Range>
auto Board::getMaxValueInRangeModern(Range&& range) const {
    using namespace std::ranges;
    using ValueType = std::decay_t<decltype(*range.begin())>;

    if (std::ranges::empty(range)) {
        return std::optional<ValueType>{};
    }

    auto maxIt = std::ranges::max_element(range);
    return std::optional<ValueType>{*maxIt};
}
template<typename Range>
auto Board::getMinValueInRangeModern(Range&& range) const {
    using namespace std::ranges;
    using ValueType = std::decay_t<decltype(*range.begin())>;

    if (std::ranges::empty(range)) {
        return std::optional<ValueType>{};
    }

    auto minIt = std::ranges::min_element(range);
    return std::optional<ValueType>{*minIt};
}
Board::State Board::checkWin(bool canCountPoints, int16_t boardMaxSize)
{
    qDebug() << "Checking win with boardMaxSize:" << boardMaxSize;

    for (int16_t i = 0; i < getRowSize(); ++i) {
        int16_t redCount = 0, blueCount = 0;
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                Color color = m_board[i][j].back().getColor();
                if (color == Color::Red || color == Color::IlusionRed) {
                    redCount++;
                    blueCount = 0;
                    qDebug() << "Red card found at (" << i << "," << j << ") - consecutive count:" << redCount;
                }
                else if (color == Color::Blue || color == Color::IlusionBlue) {
                    blueCount++;
                    redCount = 0;
                    qDebug() << "Blue card found at (" << i << "," << j << ") - consecutive count:" << blueCount;
                }
                else {
                    redCount = blueCount = 0;
                }

                if (redCount >= boardMaxSize) {
                    qDebug() << "RED WINS with row" << i << "! Found" << redCount << "consecutive red cards";
                    return State::RedWin;
                }
                if (blueCount >= boardMaxSize) {
                    qDebug() << "BLUE WINS with row" << i << "! Found" << blueCount << "consecutive blue cards";
                    return State::BlueWin;
                }
            }
            else {
                redCount = blueCount = 0;
            }
        }
    }

    for (int16_t j = 0; j < getColumnSize(); ++j) {
        int16_t redCount = 0, blueCount = 0;
        for (int16_t i = 0; i < getRowSize(); ++i) {
            if (!m_board[i][j].empty()) {
                Color color = m_board[i][j].back().getColor();
                if (color == Color::Red || color == Color::IlusionRed) {
                    redCount++;
                    blueCount = 0;
                    qDebug() << "Red card found at (" << i << "," << j << ") - consecutive count:" << redCount;
                }
                else if (color == Color::Blue || color == Color::IlusionBlue) {
                    blueCount++;
                    redCount = 0;
                    qDebug() << "Blue card found at (" << i << "," << j << ") - consecutive count:" << blueCount;
                }
                else {
                    redCount = blueCount = 0;
                }

                if (redCount >= boardMaxSize) {
                    qDebug() << "RED WINS with column" << j << "! Found" << redCount << "consecutive red cards";
                    return State::RedWin;
                }
                if (blueCount >= boardMaxSize) {
                    qDebug() << "BLUE WINS with column" << j << "! Found" << blueCount << "consecutive blue cards";
                    return State::BlueWin;
                }
            }
            else {
                redCount = blueCount = 0;
            }
        }
    }

    if (getRowSize() == getColumnSize()) {
        int16_t redCount = 0, blueCount = 0;
        for (int16_t i = 0; i < getRowSize(); ++i) {
            if (!m_board[i][i].empty()) {
                Color color = m_board[i][i].back().getColor();
                if (color == Color::Red || color == Color::IlusionRed) {
                    redCount++;
                    blueCount = 0;
                    qDebug() << "Red card found on main diagonal at (" << i << "," << i << ") - consecutive count:" << redCount;
                }
                else if (color == Color::Blue || color == Color::IlusionBlue) {
                    blueCount++;
                    redCount = 0;
                    qDebug() << "Blue card found on main diagonal at (" << i << "," << i << ") - consecutive count:" << blueCount;
                }
                else {
                    redCount = blueCount = 0;
                }

                if (redCount >= boardMaxSize) {
                    qDebug() << "RED WINS with main diagonal! Found" << redCount << "consecutive red cards";
                    return State::RedWin;
                }
                if (blueCount >= boardMaxSize) {
                    qDebug() << "BLUE WINS with main diagonal! Found" << blueCount << "consecutive blue cards";
                    return State::BlueWin;
                }
            }
            else {
                redCount = blueCount = 0;
            }
        }

        redCount = blueCount = 0;
        for (int16_t i = 0; i < getRowSize(); ++i) {
            int16_t j = getColumnSize() - 1 - i;
            if (!m_board[i][j].empty()) {
                Color color = m_board[i][j].back().getColor();
                if (color == Color::Red || color == Color::IlusionRed) {
                    redCount++;
                    blueCount = 0;
                    qDebug() << "Red card found on secondary diagonal at (" << i << "," << j << ") - consecutive count:" << redCount;
                }
                else if (color == Color::Blue || color == Color::IlusionBlue) {
                    blueCount++;
                    redCount = 0;
                    qDebug() << "Blue card found on secondary diagonal at (" << i << "," << j << ") - consecutive count:" << blueCount;
                }
                else {
                    redCount = blueCount = 0;
                }

                if (redCount >= boardMaxSize) {
                    qDebug() << "RED WINS with secondary diagonal! Found" << redCount << "consecutive red cards";
                    return State::RedWin;
                }
                if (blueCount >= boardMaxSize) {
                    qDebug() << "BLUE WINS with secondary diagonal! Found" << blueCount << "consecutive blue cards";
                    return State::BlueWin;
                }
            }
            else {
                redCount = blueCount = 0;
            }
        }
    }

    qDebug() << "No line-based wins found, checking point-based win...";

    if (canCountPoints) {
        int16_t redSum = sumPointsModern(Color::Red);
        int16_t blueSum = sumPointsModern(Color::Blue);

        qDebug() << "Counting points - Red:" << redSum << ", Blue:" << blueSum;

        if (redSum > blueSum) {
            qDebug() << "RED WINS by points!";
            return State::RedWin;
        }
        else if (blueSum > redSum) {
            qDebug() << "BLUE WINS by points!";
            return State::BlueWin;
        }

        qDebug() << "Game is a DRAW!";
        return State::Draw;
    }

    qDebug() << "No winner found - game continues";
    return State::None;
}
void Board::optimizedBoardOperations() {
    auto emptyPositions = getEmptyPositionsModern();
    qDebug() << "Found" << emptyPositions.size() << "empty positions";

    auto [redCardCount, blueCardCount] = getCardCountByColorModern();
    qDebug() << "Red cards:" << redCardCount << ", Blue cards:" << blueCardCount;
}

void Board::smartExpansionWithTemplates(int16_t maxSize) {
    auto needsRowExpansion = [this, maxSize](int16_t) { return canExpandVertically(maxSize); };
    auto needsColExpansion = [this, maxSize](int16_t) { return canExpandHorizontally(maxSize); };

    if (needsAnyExpansion(maxSize, needsRowExpansion, needsColExpansion)) {
        qDebug() << "Board needs expansion";

        if (needsRowExpansion(maxSize)) {
            processMultipleRows([this](int16_t row) {
                if (row == 0 && hasCardsInRow(0)) {
                    expandRow(RowExpandDirection::Up);
                }
                else if (row == getRowSize() - 1 && hasCardsInRow(getRowSize() - 1)) {
                    expandRow(RowExpandDirection::Down);
                }
                }, 0, getRowSize() - 1);
        }

        if (needsColExpansion(maxSize)) {
            processMultipleColumns([this](int16_t col) {
                if (col == 0 && hasCardsInColumn(0)) {
                    expandColumn(ColumnExpandDirection::Left);
                }
                else if (col == getColumnSize() - 1 && hasCardsInColumn(getColumnSize() - 1)) {
                    expandColumn(ColumnExpandDirection::Right);
                }
                }, 0, getColumnSize() - 1);
        }
    }
}

bool Board::validateMultiplePositions() {
    Position centerPos = { getRowSize() / 2, getColumnSize() / 2 };
    Position topLeftPos = { 0, 0 };
    Position bottomRightPos = { getRowSize() - 1, getColumnSize() - 1 };

    return areAllPositionsValid(centerPos, topLeftPos, bottomRightPos);
}

void Board::optimizedCardProcessing() {
    auto redCardProcessor = [](const SimpleCard& card) {
        if (card.getColor() == Color::Red || card.getColor() == Color::IlusionRed) {
            qDebug() << "Processing red card with value:" << card.getValue();
        }
        };

    auto blueCardProcessor = [](const SimpleCard& card) {
        if (card.getColor() == Color::Blue || card.getColor() == Color::IlusionBlue) {
            qDebug() << "Processing blue card with value:" << card.getValue();
        }
        };

    auto eterCardProcessor = [](const SimpleCard& card) {
        if (card.getValue() == 5) {
            qDebug() << "Processing Eter card";
        }
        };

    forEachPosition([&](int16_t row, int16_t col, const auto& stack) {
        if (!stack.empty()) {
            const auto& topCard = stack.back();

            processMultipleCards(topCard, redCardProcessor, blueCardProcessor, eterCardProcessor);
        }
        });
}
void Board::templateBasedCleanup() {
    auto isolatedPositions = findPositions([this](int16_t row, int16_t col, const auto& stack) {
        return !stack.empty() && !hasAdjacentCards(row, col);
        });

    for (const auto& pos : isolatedPositions) {
        auto [row, col] = pos;
        m_board[row][col].clear();
        qDebug() << "Cleaned isolated position at (" << row << "," << col << ")";
    }

    auto hasEmptyRows = [this]() { return !searchEmptyRows().empty(); };
    auto hasEmptyCols = [this]() { return !searchEmptyColumns().empty(); };
    auto hasIsolatedCards = [this]() { return countIsolatedPositions() > 0; };

    if (checkMultipleWinConditions(3, hasEmptyRows, hasEmptyCols, hasIsolatedCards)) {
        qDebug() << "Additional cleanup needed";
        cleanupIsolatedPositions();
    }
}

bool Board::checkRowWins(int16_t boardMaxSize) const
{
    for (int16_t i = 0; i < getRowSize(); ++i) {
        int16_t redCount = 0, blueCount = 0;
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                Color color = m_board[i][j].back().getColor();
                if (color == Color::Red || color == Color::IlusionRed) {
                    redCount++;
                    blueCount = 0;
                }
                else if (color == Color::Blue || color == Color::IlusionBlue) {
                    blueCount++;
                    redCount = 0;
                }

                if (redCount >= boardMaxSize || blueCount >= boardMaxSize) {
                    return true;
                }
            }
            else {
                redCount = blueCount = 0;
            }
        }
    }
    return false;
}

bool Board::checkColumnWins(int16_t boardMaxSize) const
{
    for (int16_t j = 0; j < getColumnSize(); ++j) {
        int16_t redCount = 0, blueCount = 0;
        for (int16_t i = 0; i < getRowSize(); ++i) {
            if (!m_board[i][j].empty()) {
                Color color = m_board[i][j].back().getColor();
                if (color == Color::Red || color == Color::IlusionRed) {
                    redCount++;
                    blueCount = 0;
                }
                else if (color == Color::Blue || color == Color::IlusionBlue) {
                    blueCount++;
                    redCount = 0;
                }

                if (redCount >= boardMaxSize || blueCount >= boardMaxSize) {
                    return true;
                }
            }
            else {
                redCount = blueCount = 0;
            }
        }
    }
    return false;
}

bool Board::checkDiagonalWins(int16_t boardMaxSize) const
{
    if (getRowSize() != getColumnSize()) return false;

    int16_t redCount = 0, blueCount = 0;
    for (int16_t i = 0; i < getRowSize(); ++i) {
        if (!m_board[i][i].empty()) {
            Color color = m_board[i][i].back().getColor();
            if (color == Color::Red || color == Color::IlusionRed) {
                redCount++;
                blueCount = 0;
            }
            else if (color == Color::Blue || color == Color::IlusionBlue) {
                blueCount++;
                redCount = 0;
            }

            if (redCount >= boardMaxSize || blueCount >= boardMaxSize) {
                return true;
            }
        }
        else {
            redCount = blueCount = 0;
        }
    }

    redCount = blueCount = 0;
    for (int16_t i = 0; i < getRowSize(); ++i) {
        if (!m_board[i][getColumnSize() - 1 - i].empty()) {
            Color color = m_board[i][getColumnSize() - 1 - i].back().getColor();
            if (color == Color::Red || color == Color::IlusionRed) {
                redCount++;
                blueCount = 0;
            }
            else if (color == Color::Blue || color == Color::IlusionBlue) {
                blueCount++;
                redCount = 0;
            }

            if (redCount >= boardMaxSize || blueCount >= boardMaxSize) {
                return true;
            }
        }
        else {
            redCount = blueCount = 0;
        }
    }

    return false;
}

std::vector<int> Board::countCardsPerColumn() const {
    return countCardsPerColumnModern();
}

std::vector<int> Board::countCardsPerRow() const {
    return countCardsPerRowModern();
}

bool Board::shouldFixHorizontally(int16_t targetSize) const {
    std::vector<int> columnCounts = countCardsPerColumn();

    for (int i = 0; i <= static_cast<int>(columnCounts.size()) - targetSize; ++i) {
        bool allHaveCards = true;
        for (int j = 0; j < targetSize; ++j) {
            if (columnCounts[i + j] <= 0) {
                allHaveCards = false;
                break;
            }
        }

        if (allHaveCards) {
            qDebug() << "Found" << targetSize << "consecutive columns with cards starting at column" << i;
            return true;
        }
    }

    return false;
}
bool Board::shouldFixVertically(int16_t targetSize) const {
    std::vector<int> rowCounts = countCardsPerRow();

    for (int i = 0; i <= static_cast<int>(rowCounts.size()) - targetSize; ++i) {
        bool allHaveCards = true;
        for (int j = 0; j < targetSize; ++j) {
            if (rowCounts[i + j] <= 0) {
                allHaveCards = false;
                break;
            }
        }

        if (allHaveCards) {
            qDebug() << "Found" << targetSize << "consecutive rows with cards starting at row" << i;
            return true;
        }
    }

    return false;
}

void Board::fixBoardHorizontally(int16_t targetSize) {
    qDebug() << "Fixing board horizontally to target size" << targetSize << "...";

    int16_t minCol = getColumnSize();
    int16_t maxCol = -1;

    for (int16_t j = 0; j < getColumnSize(); ++j) {
        for (int16_t i = 0; i < getRowSize(); ++i) {
            if (!m_board[i][j].empty()) {
                minCol = std::min(minCol, j);
                maxCol = std::max(maxCol, j);
                break;
            }
        }
    }

    if (minCol <= maxCol) {
        qDebug() << "Cards span from column" << minCol << "to column" << maxCol;

        int16_t desiredColumns = targetSize;
        int16_t currentSpan = maxCol - minCol + 1;

        while (getColumnSize() > desiredColumns && getColumnSize() > currentSpan) {
            if (getColumnSize() > maxCol + 1) {
                qDebug() << "Removing rightmost empty column";
                removeColumn(getColumnSize() - 1);
            }
            else if (minCol > 0) {
                qDebug() << "Removing leftmost empty column";
                removeColumn(0);
                minCol--;
                maxCol--;
            }
            else {
                break;
            }
        }

        qDebug() << "Horizontal fixing complete. Columns now:" << getColumnSize();
    }
}

void Board::fixBoardVertically(int16_t targetSize) {
    qDebug() << "Fixing board vertically to target size" << targetSize << "...";

    int16_t minRow = getRowSize();
    int16_t maxRow = -1;

    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                minRow = std::min(minRow, i);
                maxRow = std::max(maxRow, i);
                break;
            }
        }
    }

    if (minRow <= maxRow) {
        qDebug() << "Cards span from row" << minRow << "to row" << maxRow;

        int16_t desiredRows = targetSize;
        int16_t currentSpan = maxRow - minRow + 1;

        while (getRowSize() > desiredRows && getRowSize() > currentSpan) {
            if (getRowSize() > maxRow + 1) {
                qDebug() << "Removing bottommost empty row";
                removeRow(getRowSize() - 1);
            }
            else if (minRow > 0) {
                qDebug() << "Removing topmost empty row";
                removeRow(0);
                minRow--;
                maxRow--;
            }
            else {
                break;
            }
        }

        qDebug() << "Vertical fixing complete. Rows now:" << getRowSize();
    }
}

void Board::smartBoardManagement(int16_t maxSize) {
    qDebug() << "Starting smart board management (maxSize:" << maxSize << ")...";

    auto allCardPositions = getAllCardPositionsModern();
    if (allCardPositions.empty()) {
        qDebug() << "No cards on board - nothing to manage";
        return;
    }

    auto [redCount, blueCount] = getCardCountByColorModern();
    int16_t totalCards = redCount + blueCount;

    int16_t targetSize;
    if (maxSize == 3 || maxSize == 4) {
        targetSize = 3;
    }
    else if (maxSize == 5) {
        targetSize = 4;
    }
    else {
        targetSize = maxSize - 1;
    }

    qDebug() << "Target final size determined as:" << targetSize << " (maxSize was " << maxSize << ", totalCards=" << totalCards << ")";

    ensureAllCardsHaveAdjacency(maxSize);

    bool shouldFixHoriz = shouldFixHorizontally(targetSize);

    bool shouldFixVert = shouldFixVertically(targetSize);

    if (shouldFixHoriz) {
        qDebug() << "Board should be fixed horizontally to target size" << targetSize;
        fixBoardHorizontally(targetSize);
    }

    if (shouldFixVert) {
        qDebug() << "Board should be fixed vertically to target size" << targetSize;
        fixBoardVertically(targetSize);
    }

    removeIsolatedPositions();

    qDebug() << "Smart board management complete. Final size:"
        << getRowSize() << "x" << getColumnSize();
}
void Board::ensureAllCardsHaveAdjacency(int16_t maxSize) {
    qDebug() << "Ensuring all cards have complete adjacency...";
    qDebug() << "Current board size: " << getRowSize() << "x" << getColumnSize()
        << " (max allowed: " << maxSize << "x" << maxSize << ")";

    bool expandedInThisIteration = false;

    do {
        expandedInThisIteration = false;

        std::vector<std::pair<int16_t, int16_t>> cardPositions;
        for (int16_t i = 0; i < getRowSize(); ++i) {
            for (int16_t j = 0; j < getColumnSize(); ++j) {
                if (!m_board[i][j].empty()) {
                    cardPositions.push_back({ i, j });
                }
            }
        }

        if (cardPositions.empty()) return;

        qDebug() << "Found " << cardPositions.size() << " cards on board";

        for (const auto& [row, col] : cardPositions) {
            std::vector<std::pair<int16_t, int16_t>> neighbors = {
                {row + 1, col}, {row - 1, col},
                {row, col + 1}, {row, col - 1},
                {row + 1, col + 1}, {row - 1, col + 1},
                {row - 1, col - 1}, {row + 1, col - 1}
            };

            for (const auto& [nx, ny] : neighbors) {
                if (nx < 0 && canExpandVertically(maxSize)) {
                    qDebug() << "Expanding UP for card at (" << row << "," << col << ")";
                    qDebug() << "Before expansion: " << getRowSize() << " rows";
                    expandRow(RowExpandDirection::Up);
                    qDebug() << "After expansion: " << getRowSize() << " rows";
                    expandedInThisIteration = true;
                    break;
                }
                if (nx >= getRowSize() && canExpandVertically(maxSize)) {
                    qDebug() << "Expanding DOWN for card at (" << row << "," << col << ")";
                    qDebug() << "Before expansion: " << getRowSize() << " rows";
                    expandRow(RowExpandDirection::Down);
                    qDebug() << "After expansion: " << getRowSize() << " rows";
                    expandedInThisIteration = true;
                    break;
                }
                if (ny < 0 && canExpandHorizontally(maxSize)) {
                    qDebug() << "Expanding LEFT for card at (" << row << "," << col << ")";
                    qDebug() << "Before expansion: " << getColumnSize() << " columns";
                    expandColumn(ColumnExpandDirection::Left);
                    qDebug() << "After expansion: " << getColumnSize() << " columns";
                    expandedInThisIteration = true;
                    break;
                }
                if (ny >= getColumnSize() && canExpandHorizontally(maxSize)) {
                    qDebug() << "Expanding RIGHT for card at (" << row << "," << col << ")";
                    qDebug() << "Before expansion: " << getColumnSize() << " columns";
                    expandColumn(ColumnExpandDirection::Right);
                    qDebug() << "After expansion: " << getColumnSize() << " columns";
                    expandedInThisIteration = true;
                    break;
                }
            }

            if (expandedInThisIteration) break;
        }

    } while (expandedInThisIteration);

    qDebug() << "Adjacency ensured. Final board size: "
        << getRowSize() << "x" << getColumnSize();
}

bool Board::canExpandHorizontally(int16_t maxSize) const {
    bool canExpand = getColumnSize() < maxSize;

    qDebug() << "Horizontal expansion check: current columns=" << getColumnSize()
        << ", maxSize=" << maxSize
        << ", canExpand=" << canExpand;

    return canExpand;
}

bool Board::canExpandVertically(int16_t maxSize) const {
    bool canExpand = getRowSize() < maxSize;

    qDebug() << "Vertical expansion check: current rows=" << getRowSize()
        << ", maxSize=" << maxSize
        << ", canExpand=" << canExpand;

    return canExpand;
}


int16_t Board::getMinRowWithCards() const {
    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                return i;
            }
        }
    }
    return -1;
}

int16_t Board::getMaxRowWithCards() const {
    for (int16_t i = getRowSize() - 1; i >= 0; --i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                return i;
            }
        }
    }
    return -1;
}

int16_t Board::getMinColWithCards() const {
    for (int16_t j = 0; j < getColumnSize(); ++j) {
        for (int16_t i = 0; i < getRowSize(); ++i) {
            if (!m_board[i][j].empty()) {
                return j;
            }
        }
    }
    return -1;
}

int16_t Board::getMaxColWithCards() const {
    for (int16_t j = getColumnSize() - 1; j >= 0; --j) {
        for (int16_t i = 0; i < getRowSize(); ++i) {
            if (!m_board[i][j].empty()) {
                return j;
            }
        }
    }
    return -1;
}

void Board::asymmetricExpansion(int16_t maxSize) {
    qDebug() << "Starting asymmetric expansion...";

    std::vector<std::pair<int16_t, int16_t>> cardPositions;
    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                cardPositions.push_back({ i, j });
            }
        }
    }

    if (cardPositions.empty()) return;

    if (shouldFixBoard()) {
        qDebug() << "Board should be fixed - no more expansion";
        fixBoardBoundaries();
        return;
    }

    bool needsExpansion = false;

    for (const auto& [row, col] : cardPositions) {
        std::vector<std::pair<int16_t, int16_t>> neighbors = {
            {row + 1, col}, {row - 1, col},
            {row, col + 1}, {row, col - 1},
            {row + 1, col + 1}, {row - 1, col + 1},
            {row - 1, col - 1}, {row + 1, col - 1}
        };

        for (const auto& [nx, ny] : neighbors) {
            if (nx < 0 && getRowSize() < maxSize) {
                qDebug() << "Expanding UP for adjacency to card at (" << row << "," << col << ")";
                expandRow(RowExpandDirection::Up);
                needsExpansion = true;
                break;
            }
            if (nx >= getRowSize() && getRowSize() < maxSize) {
                qDebug() << "Expanding DOWN for adjacency to card at (" << row << "," << col << ")";
                expandRow(RowExpandDirection::Down);
                needsExpansion = true;
                break;
            }
            if (ny < 0 && getColumnSize() < maxSize) {
                qDebug() << "Expanding LEFT for adjacency to card at (" << row << "," << col << ")";
                expandColumn(ColumnExpandDirection::Left);
                needsExpansion = true;
                break;
            }
            if (ny >= getColumnSize() && getColumnSize() < maxSize) {
                qDebug() << "Expanding RIGHT for adjacency to card at (" << row << "," << col << ")";
                expandColumn(ColumnExpandDirection::Right);
                needsExpansion = true;
                break;
            }
        }

        if (needsExpansion) break;
    }

    removeIsolatedPositions();

    qDebug() << "Asymmetric expansion completed. Board size: "
        << getRowSize() << "x" << getColumnSize();
}

bool Board::shouldFixBoard() const {
    return hasThreeInLineColumnDiagonal() || hasTwoCardsAtDistance();
}

bool Board::hasThreeInLineColumnDiagonal() const {
    for (int16_t i = 0; i < getRowSize(); ++i) {
        int16_t consecutiveCards = 0;
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                consecutiveCards++;
                if (consecutiveCards >= 3) {
                    qDebug() << "Found 3 consecutive cards on row " << i;
                    return true;
                }
            }
            else {
                consecutiveCards = 0;
            }
        }
    }

    for (int16_t j = 0; j < getColumnSize(); ++j) {
        int16_t consecutiveCards = 0;
        for (int16_t i = 0; i < getRowSize(); ++i) {
            if (!m_board[i][j].empty()) {
                consecutiveCards++;
                if (consecutiveCards >= 3) {
                    qDebug() << "Found 3 consecutive cards on column " << j;
                    return true;
                }
            }
            else {
                consecutiveCards = 0;
            }
        }
    }

    if (getRowSize() == getColumnSize()) {
        int16_t consecutiveCards = 0;
        for (int16_t i = 0; i < getRowSize(); ++i) {
            if (!m_board[i][i].empty()) {
                consecutiveCards++;
                if (consecutiveCards >= 3) {
                    qDebug() << "Found 3 consecutive cards on main diagonal";
                    return true;
                }
            }
            else {
                consecutiveCards = 0;
            }
        }

        consecutiveCards = 0;
        for (int16_t i = 0; i < getRowSize(); ++i) {
            if (!m_board[i][getColumnSize() - 1 - i].empty()) {
                consecutiveCards++;
                if (consecutiveCards >= 3) {
                    qDebug() << "Found 3 consecutive cards on secondary diagonal";
                    return true;
                }
            }
            else {
                consecutiveCards = 0;
            }
        }
    }

    return false;
}

bool Board::hasTwoCardsAtDistance() const {
    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize() - 2; ++j) {
            if (!m_board[i][j].empty() && m_board[i][j + 1].empty() && !m_board[i][j + 2].empty()) {
                bool firstHasAdjacent = hasAdjacentCards(i, j);
                bool secondHasAdjacent = hasAdjacentCards(i, j + 2);

                if (firstHasAdjacent && secondHasAdjacent) {
                    qDebug() << "Found two cards at distance on row " << i << " positions " << j << " and " << (j + 2);
                    return true;
                }
            }
        }
    }

    for (int16_t j = 0; j < getColumnSize(); ++j) {
        for (int16_t i = 0; i < getRowSize() - 2; ++i) {
            if (!m_board[i][j].empty() && m_board[i + 1][j].empty() && !m_board[i + 2][j].empty()) {
                bool firstHasAdjacent = hasAdjacentCards(i, j);
                bool secondHasAdjacent = hasAdjacentCards(i + 2, j);

                if (firstHasAdjacent && secondHasAdjacent) {
                    qDebug() << "Found two cards at distance on column " << j << " positions " << i << " and " << (i + 2);
                    return true;
                }
            }
        }
    }

    return false;
}

void Board::fixBoardBoundaries() {
    qDebug() << "Fixing board boundaries...";

    while (getRowSize() > 3) {
        if (isRowEmpty(0) && !isRowEmpty(getRowSize() - 1)) {
            removeRow(0);
            qDebug() << "Removed first empty row";
        }
        else if (isRowEmpty(getRowSize() - 1) && !isRowEmpty(0)) {
            removeRow(getRowSize() - 1);
            qDebug() << "Removed last empty row";
        }
        else {
            break;
        }
    }

    while (getColumnSize() > 3) {
        if (isColumnEmpty(0) && !isColumnEmpty(getColumnSize() - 1)) {
            removeColumn(0);
            qDebug() << "Removed first empty column";
        }
        else if (isColumnEmpty(getColumnSize() - 1) && !isColumnEmpty(0)) {
            removeColumn(getColumnSize() - 1);
            qDebug() << "Removed last empty column";
        }
        else {
            break;
        }
    }

    qDebug() << "Board boundaries fixed. Final size: "
        << getRowSize() << "x" << getColumnSize();
}

void Board::removeIsolatedPositions() {
    if (getTotalCardsOnBoard() <= 1) return;

    std::vector<std::vector<bool>> visited(getRowSize(), std::vector<bool>(getColumnSize(), false));
    std::vector<std::pair<int16_t, int16_t>> allCardPositions;

    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                allCardPositions.push_back({ i, j });
            }
        }
    }

    if (allCardPositions.empty()) return;

    std::vector<std::pair<int16_t, int16_t>> largestComponent;

    for (const auto& [row, col] : allCardPositions) {
        if (!visited[row][col]) {
            auto component = findConnectedComponent(row, col, visited);
            if (component.size() > largestComponent.size()) {
                largestComponent = component;
            }
        }
    }

    for (const auto& [row, col] : allCardPositions) {
        bool inLargestComponent = false;
        for (const auto& [compRow, compCol] : largestComponent) {
            if (row == compRow && col == compCol) {
                inLargestComponent = true;
                break;
            }
        }

        if (!inLargestComponent) {
            qDebug() << "Removing isolated card at (" << row << "," << col << ")";
            m_board[row][col].clear();
        }
    }
}

std::vector<std::pair<int16_t, int16_t>> Board::findConnectedComponent(
    int16_t startRow, int16_t startCol, std::vector<std::vector<bool>>& visited) const {

    std::vector<std::pair<int16_t, int16_t>> component;
    std::queue<std::pair<int16_t, int16_t>> toProcess;

    toProcess.push({ startRow, startCol });
    visited[startRow][startCol] = true;

    while (!toProcess.empty()) {
        auto [row, col] = toProcess.front();
        toProcess.pop();
        component.push_back({ row, col });

        std::vector<std::pair<int16_t, int16_t>> neighbors = {
            {row + 1, col}, {row - 1, col},
            {row, col + 1}, {row, col - 1},
            {row + 1, col + 1}, {row - 1, col + 1},
            {row - 1, col - 1}, {row + 1, col - 1}
        };

        for (const auto& [nx, ny] : neighbors) {
            if (nx >= 0 && nx < getRowSize() && ny >= 0 && ny < getColumnSize() &&
                !visited[nx][ny] && !m_board[nx][ny].empty()) {
                visited[nx][ny] = true;
                toProcess.push({ nx, ny });
            }
        }
    }

    return component;
}

bool Board::hasAdjacentCards(int16_t row, int16_t col) const {
    std::vector<std::pair<int16_t, int16_t>> neighbors = {
        {row + 1, col}, {row - 1, col},
        {row, col + 1}, {row, col - 1},
        {row + 1, col + 1}, {row - 1, col + 1},
        {row - 1, col - 1}, {row + 1, col - 1}
    };

    for (const auto& [nx, ny] : neighbors) {
        if (nx >= 0 && nx < getRowSize() && ny >= 0 && ny < getColumnSize()) {
            if (!m_board[nx][ny].empty()) {
                return true;
            }
        }
    }

    return false;
}

void Board::cleanupIsolatedPositions() {
    qDebug() << "Starting cleanup of isolated positions...";

    std::vector<std::pair<int16_t, int16_t>> positionsToRemove;

    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty() && !hasAdjacentCards(i, j)) {
                positionsToRemove.push_back({ i, j });
                qDebug() << "Found isolated position with card at (" << i << "," << j << ")";
            }
        }
    }

    for (const auto& [row, col] : positionsToRemove) {
        qDebug() << "Removing isolated card at (" << row << "," << col << ")";
        m_board[row][col].clear();
    }

    if (positionsToRemove.empty()) {
        qDebug() << "No isolated positions found.";
    }
    else {
        qDebug() << "Removed" << positionsToRemove.size() << "isolated positions.";
    }
}

void Board::cleanupFirstIsolatedPosition() {
    qDebug() << "Looking for first isolated position to cleanup...";

    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty() && !hasAdjacentCards(i, j)) {
                qDebug() << "Removing isolated card at (" << i << "," << j << ")";
                m_board[i][j].clear();
                return;
            }
        }
    }

    qDebug() << "No isolated positions found.";
}

int Board::countIsolatedPositions() const {
    int count = 0;

    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty() && !hasAdjacentCards(i, j)) {
                count++;
            }
        }
    }

    return count;
}

bool Board::needsDynamicExpansion(int16_t placementRow, int16_t placementCol, int16_t maxSize) const {

    if (getRowSize() >= maxSize && getColumnSize() >= maxSize) {
        return false;
    }

    std::vector<std::pair<int16_t, int16_t>> neighbors = {
        {placementRow + 1, placementCol}, {placementRow - 1, placementCol},
        {placementRow, placementCol + 1}, {placementRow, placementCol - 1},
        {placementRow + 1, placementCol + 1}, {placementRow - 1, placementCol + 1},
        {placementRow - 1, placementCol - 1}, {placementRow + 1, placementCol - 1}
    };

    for (const auto& [nx, ny] : neighbors) {
        if (nx < 0 || nx >= getRowSize() || ny < 0 || ny >= getColumnSize()) {
            return true;
        }
    }

    return false;
}

void Board::dynamicExpand(int16_t placementRow, int16_t placementCol, int16_t maxSize) {
    qDebug() << "Starting dynamic expansion for position (" << placementRow << "," << placementCol << ")";

    bool expanded = false;

    if (placementRow == 0 && getRowSize() < maxSize) {
        qDebug() << "Expanding UP - adding row above";
        expandRow(RowExpandDirection::Up);
        expanded = true;
    }

    if (placementRow == getRowSize() - 1 && getRowSize() < maxSize) {
        qDebug() << "Expanding DOWN - adding row below";
        expandRow(RowExpandDirection::Down);
        expanded = true;
    }

    if (placementCol == 0 && getColumnSize() < maxSize) {
        qDebug() << "Expanding LEFT - adding column to the left";
        expandColumn(ColumnExpandDirection::Left);
        expanded = true;
    }

    if (placementCol == getColumnSize() - 1 && getColumnSize() < maxSize) {
        qDebug() << "Expanding RIGHT - adding column to the right";
        expandColumn(ColumnExpandDirection::Right);
        expanded = true;
    }

    if (expanded) {
        qDebug() << "Board expanded to: " << getRowSize() << "x" << getColumnSize();
    }
}

void Board::autoExpandForAdiacency(int16_t maxSize) {
    if (getRowSize() >= maxSize && getColumnSize() >= maxSize) {
        return;
    }

    bool needsExpansion = false;

    std::vector<std::pair<int16_t, int16_t>> cardPositions;
    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                cardPositions.push_back({ i, j });
            }
        }
    }

    for (const auto& [row, col] : cardPositions) {
        std::vector<std::pair<int16_t, int16_t>> neighbors = {
            {row + 1, col}, {row - 1, col}, {row, col + 1}, {row, col - 1},
            {row + 1, col + 1}, {row - 1, col + 1}, {row - 1, col - 1}, {row + 1, col - 1}
        };

        for (const auto& [nx, ny] : neighbors) {
            if (nx < 0 && getRowSize() < maxSize) {
                qDebug() << "Auto-expanding UP for adjacency";
                expandRow(RowExpandDirection::Up);
                needsExpansion = true;
                break;
            }
            if (nx >= getRowSize() && getRowSize() < maxSize) {
                qDebug() << "Auto-expanding DOWN for adjacency";
                expandRow(RowExpandDirection::Down);
                needsExpansion = true;
                break;
            }
            if (ny < 0 && getColumnSize() < maxSize) {
                qDebug() << "Auto-expanding LEFT for adjacency";
                expandColumn(ColumnExpandDirection::Left);
                needsExpansion = true;
                break;
            }
            if (ny >= getColumnSize() && getColumnSize() < maxSize) {
                qDebug() << "Auto-expanding RIGHT for adjacency";
                expandColumn(ColumnExpandDirection::Right);
                needsExpansion = true;
                break;
            }
        }

        if (needsExpansion) {
            break;
        }
    }

    if (needsExpansion) {
        autoExpandForAdiacency(maxSize);
    }
}


void Board::initializeForDynamicPlay(int16_t maxSize) {
    m_size = maxSize;
    int16_t initialSize = (2 < maxSize) ? 2 : maxSize;

    m_board.clear();
    m_board.resize(initialSize, std::vector<std::deque<SimpleCard>>(initialSize));

    qDebug() << "Board initialized for dynamic play: " << initialSize << "x" << initialSize
        << " (max: " << maxSize << "x" << maxSize << ")";
}
std::pair<int16_t, int16_t> Board::getActualBoardBounds() const {
    int16_t minRow = getRowSize(), maxRow = -1;
    int16_t minCol = getColumnSize(), maxCol = -1;

    bool hasCards = false;
    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                hasCards = true;
                minRow = (i < minRow) ? i : minRow;
                maxRow = (i > maxRow) ? i : maxRow;
                minCol = (j < minCol) ? j : minCol;
                maxCol = (j > maxCol) ? j : maxCol;
            }
        }
    }

    if (!hasCards) {
        return { 0, 0 };
    }

    return { maxRow - minRow + 1, maxCol - minCol + 1 };
}




void Board::expandRow(RowExpandDirection direction)
{
    int16_t newSize = m_board.size() + 1;
    matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(m_board[0].size()));

    if (direction == RowExpandDirection::Up)
    {
        for (int16_t i = newSize - 1; i > 0; --i)
        {
            for (int16_t j = 0; j < m_board[0].size(); j++)
            {
                newMatrix[i][j] = std::move(m_board[i - 1][j]);
            }
        }
    }
    else
    {
        for (int16_t i = 0; i < m_board.size(); ++i)
        {
            for (int16_t j = 0; j < m_board[i].size(); j++)
            {
                newMatrix[i][j] = std::move(m_board[i][j]);
            }
        }
    }

    m_board = std::move(newMatrix);
}

void Board::expandColumn(ColumnExpandDirection direction)
{
    int16_t newSize = m_board[0].size() + 1;
    matrix newMatrix(m_board.size(), std::vector<std::deque<SimpleCard>>(newSize));

    if (direction == ColumnExpandDirection::Left)
    {
        for (int16_t i = 0; i < m_board.size(); ++i)
        {
            for (int16_t j = newSize - 1; j > 0; --j)
            {
                newMatrix[i][j] = std::move(m_board[i][j - 1]);
            }
        }
    }
    else
    {
        for (int16_t i = 0; i < m_board.size(); ++i)
        {
            for (int16_t j = 0; j < m_board[0].size(); ++j)
            {
                newMatrix[i][j] = std::move(m_board[i][j]);
            }
        }
    }

    m_board = std::move(newMatrix);
}

std::vector<int16_t> Board::searchEmptyColumns()
{
    return searchEmptyColumnsModern();
}

std::vector<int16_t> Board::searchEmptyRows()
{
    return searchEmptyRowsModern();
}

bool Board::canBePlaced(int16_t x, int16_t y) const {
    int16_t rows = m_board.size();
    int16_t columns = m_board[0].size();

    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }

    if (!hasAnyCardModern()) {
        return m_board[x][y].empty();
    }

    if (!m_board[x][y].empty()) {
        return true;
    }

    std::vector<std::pair<int16_t, int16_t>> neighbors = {
        {x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1},
        {x + 1, y + 1}, {x - 1, y + 1}, {x - 1, y - 1}, {x + 1, y - 1}
    };

    for (const auto& [nx, ny] : neighbors) {
        if (nx >= 0 && nx < rows && ny >= 0 && ny < columns) {
            if (!m_board[nx][ny].empty()) {
                return true;
            }
        }
    }

    return false;
}
int Board::getTotalCardsOnBoard() const {
    return getTotalCardsModern();
}


int16_t Board::sumPoints(const Color& color)
{

    return sumPointsModern(color);
}

bool Board::needsExpansion(int16_t placementRow, int16_t placementCol, int16_t maxSize) const {

    if (getRowSize() >= maxSize && getColumnSize() >= maxSize) {
        return false;
    }

    bool needsRowExpansion = (placementRow == 0 && !isRowEmpty(0)) ||
        (placementRow == getRowSize() - 1 && !isRowEmpty(getRowSize() - 1));

    bool needsColExpansion = (placementCol == 0 && !isColumnEmpty(0)) ||
        (placementCol == getColumnSize() - 1 && !isColumnEmpty(getColumnSize() - 1));

    return (needsRowExpansion && getRowSize() < maxSize) ||
        (needsColExpansion && getColumnSize() < maxSize);
}

bool Board::isRowEmpty(int16_t row) const {
    if (row < 0 || row >= getRowSize()) return true;

    for (int16_t j = 0; j < getColumnSize(); ++j) {
        if (!m_board[row][j].empty()) {
            return false;
        }
    }
    return true;
}

bool Board::isColumnEmpty(int16_t col) const {
    if (col < 0 || col >= getColumnSize()) return true;

    for (int16_t i = 0; i < getRowSize(); ++i) {
        if (!m_board[i][col].empty()) {
            return false;
        }
    }
    return true;
}

void Board::smartExpand(int16_t placementRow, int16_t placementCol, int16_t maxSize) {

    if (getRowSize() >= maxSize && getColumnSize() >= maxSize) {
        return;
    }

    if (getRowSize() < maxSize) {
        if (placementRow == 0 && hasCardsInRow(0)) {
            expandRow(RowExpandDirection::Up);
        }
        else if (placementRow == getRowSize() - 1 && hasCardsInRow(getRowSize() - 1)) {
            expandRow(RowExpandDirection::Down);
        }
    }

    if (getColumnSize() < maxSize) {
        if (placementCol == 0 && hasCardsInColumn(0)) {
            expandColumn(ColumnExpandDirection::Left);
        }
        else if (placementCol == getColumnSize() - 1 && hasCardsInColumn(getColumnSize() - 1)) {
            expandColumn(ColumnExpandDirection::Right);
        }
    }
}

bool Board::hasCardsInRow(int16_t row) const {
    return hasCardsInRowModern(row);
}

bool Board::hasCardsInColumn(int16_t col) const {
    return hasCardsInColumnModern(col);
}

int16_t Board::getSize() const
{
    return m_board.size();
}

int16_t Board::getRowSize() const
{
    return m_board.size();
}

int16_t Board::getColumnSize() const
{
    return m_board[0].size();
}

void Board::moveSpace(int16_t row, int16_t column, int16_t newRow, int16_t newColumn)
{
    m_board[newRow][newColumn] = std::move(m_board[row][column]);
    m_board[row][column].clear();
}

void Board::emptyRow(int16_t row)
{
    for (auto& column : m_board[row])
    {
        column.clear();
    }
}

void Board::emptyColumn(int16_t column)
{
    for (auto& row : m_board)
    {
        row[column].clear();
    }
}

void Board::removeRow(int16_t row)
{
    int16_t newSize = m_board.size() - 1;
    matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(m_board[0].size()));
    if (row == 0)
    {
        for (int16_t i = 0; i < newSize; i++)
        {
            for (int16_t j = 0; j < newMatrix[0].size(); j++)
            {
                newMatrix[i][j] = std::move(m_board[i + 1][j]);
            }
        }
    }
    else
    {
        if (row == m_board.size() - 1)
        {
            for (int16_t i = 0; i < newSize; i++)
            {
                for (int16_t j = 0; j < newMatrix[0].size(); j++)
                {
                    newMatrix[i][j] = std::move(m_board[i][j]);
                }
            }
        }
        else
        {
            std::cout << "Nu se poate sterge randul din mijloc!\n";
        }
    }

    m_board = std::move(newMatrix);
}

void Board::removeColumn(int16_t column)
{
    int16_t newSize = m_board[0].size() - 1;
    matrix newMatrix(m_board.size(), std::vector<std::deque<SimpleCard>>(newSize));
    if (column == 0)
    {
        for (int16_t i = 0; i < m_board.size(); i++)
        {
            for (int16_t j = 0; j < newSize; j++)
            {
                newMatrix[i][j] = std::move(m_board[i][j + 1]);
            }
        }
    }
    else
    {
        if (column == m_board[0].size() - 1)
        {
            for (int16_t i = 0; i < m_board.size(); i++)
            {
                for (int16_t j = 0; j < newSize; j++)
                {
                    newMatrix[i][j] = std::move(m_board[i][j]);
                }
            }
        }
        else
        {
            std::cout << "Nu se poate sterge coloana din mijloc!\n";
        }
    }

    m_board = std::move(newMatrix);
}

Board::Board() :
    m_board{},
    m_size{ 0 }
{

}

Board::Board(int16_t size)
{
    this->resizeBoard(size);
    m_size = size;
}

Board::Board(Board&& board) noexcept
    :m_board{ std::move(board.m_board) },
    m_size{ std::move(board.m_size) }
{

}

Board& Board::operator=(Board&& board) noexcept
{
    if (this == &board)
        return *this;

    m_board = std::move(board.m_board);
    m_size = std::move(board.m_size);

    return *this;
}

void Board::resizeBoard(int16_t size)
{
    m_size = size;
    this->m_board.clear();
    this->m_board.resize(size, std::vector<std::deque<SimpleCard>>(size));
}

void Board::print()const
{
    int16_t rows = m_board.size();
    int16_t cols = rows > 0 ? m_board[0].size() : 0;

    QDebug debug = qDebug().nospace();

    for (int16_t i = rows < 3 && cols < 3 ? -1 : 0; i <= rows; i++)
    {
        for (int16_t j = rows < 3 && cols < 3 ? -1 : 0; j <= cols; j++)
        {
            if (rows < 3 && cols < 3)
            {
                if (i < 0 || i >= rows || j < 0 || j >= cols)
                {
                    if (canBePlaced(i, j))
                    {
                        debug << " * ";
                    }
                    else
                    {
                        debug << " ";
                    }
                    continue;
                }
                else if (i >= 0 && i < rows && j >= 0 && j < cols)
                {
                    if (m_board[i][j].empty())
                    {
                        debug << " * ";
                        continue;
                    }
                }
            }

            if (j < cols && i < rows && i >= 0 && j >= 0)
            {
                if (!m_board[i][j].empty())
                {
                    if (m_board[i][j].back().getColor() == Color::IlusionRed)
                    {
                        qDebug().noquote() << "iR" << " ";
                    }
                    else if (m_board[i][j].back().getColor() == Color::IlusionBlue)
                    {
                        qDebug().noquote() << "iB" << " ";
                    }
                    else
                        debug << m_board[i][j].back().getValue() << " ";
                }
                else
                {
                    debug << " * ";
                }
            }

        }

        debug << '\n';
    }
    qDebug() << '\n';
}

void Board::clear()
{
    for (auto& row : m_board)
    {
        for (auto& column : row)
        {
            column.clear();
        }
    }
}

bool Board::checkRow(int16_t row)
{
    for (int16_t i = 0; i < m_board.size(); i++)
    {
        if (m_board[row][i].empty())
        {
            return false;
        }
    }
    return true;
}

bool Board::checkColumn(int16_t column)
{
    for (int16_t i = 0; i < m_board.size(); i++)
    {
        if (m_board[i][column].empty())
        {
            return false;
        }
    }
    return true;
}

void Board::pushCard(const SimpleCard& card, const Position& position)
{
    auto& [line, column] = position;

    if (canBePushed(card, position))
    {
        m_board[line][column].push_back(card);
    }
    else
    {
        std::cout << "Cannot push card because it has a smaller value\n";
    }
}

bool Board::canBePushed(const SimpleCard& card, const Position& position) const
{
    auto& [line, column] = position;

    if (m_board[line][column].empty())
    {
        return true;
    }

    if (card.getValue() > m_board[line][column].back().getValue())
    {
        return true;
    }

    return false;
}

void Board::popCard(const Position& position)
{
    auto& [line, column] = position;

    m_board[line][column].pop_back();
}

void Board::popCardAt(const Position& position, const SimpleCard& targetCard)
{
    auto& [line, column] = position;

    if (line > m_board.size() || column > m_board[0].size())
    {
        throw std::out_of_range("Position out of bounds");
    }

    std::deque<SimpleCard>& dequeWeRemoveFrom = m_board[line][column];

    for (auto it = dequeWeRemoveFrom.begin(); it != dequeWeRemoveFrom.end(); ++it)
    {
        if (it->getValue() == targetCard.getValue() && it->getColor() == targetCard.getColor())
        {
            dequeWeRemoveFrom.erase(it);
            break;
        }
    }
}

int Board::getNumberOfRowsWithCards() const
{
    int nr = 0;
    for (int i = 0; i < getRowSize(); i++)
    {
        for (int j = 0; j < getColumnSize(); j++)
        {
            if (!m_board[i][j].empty())
            {
                nr++;
                break;
            }
        }
    }
    return nr;
}

int Board::getNumberOfColumnsWithCards() const
{
    int nr = 0;
    for (int i = 0; i < getColumnSize(); i++)
    {
        for (int j = 0; j < getRowSize(); j++)
        {
            if (!m_board[j][i].empty())
            {
                nr++;
                break;
            }
        }
    }
    return nr;
}

bool Board::isFirstColumnEmpty() const
{
    for (int i = 0; i < m_board.size(); i++)
    {
        if (!m_board[i][0].empty())
            return false;
    }
    return true;
}

bool Board::isLastColumnEmpty()const
{
    for (int i = 0; i < m_board.size(); i++)
    {
        if (!m_board[i][getColumnSize() - 1].empty())
            return false;
    }
    return true;
}

bool Board::isFirstRowEmpty()const
{
    for (int i = 0; i < m_board[0].size(); i++)
    {
        if (!m_board[0][i].empty())
            return false;
    }
    return true;
}

bool Board::isLastRowEmpty()const
{
    for (int i = 0; i < m_board[0].size(); i++)
    {
        if (!m_board[getRowSize() - 1][i].empty())
            return false;
    }
    return true;
}

int Board::getIndexOfFirstRowOfBoard()
{
    int indexMin = 5;
    for (int i = 0; i < m_board.size(); i++)
    {
        for (int j = 0; j < m_board[i].size(); j++)
        {
            if (!m_board[i][j].empty())
            {
                if (i < indexMin)
                    indexMin = i;
            }
        }
    }
    return indexMin;
}

int Board::getIndexOfLastRowOfBoard()
{
    int indexMax = 0;
    for (int i = 0; i < m_board.size(); i++)
    {
        for (int j = 0; j < m_board[i].size(); j++)
        {
            if (!m_board[i][j].empty())
            {
                if (i > indexMax)
                    indexMax = i;
            }
        }
    }
    return indexMax;
}

int Board::getIndexOfFirstColumnOfBoard()
{
    int indexMin = 5;
    for (int i = 0; i < m_board.size(); i++)
    {
        for (int j = 0; j < m_board[i].size(); j++)
        {
            if (!m_board[i][j].empty())
            {
                if (j < indexMin)
                    indexMin = j;
            }
        }
    }
    return indexMin;
}

int Board::getIndexOfLastColumnOfBoard()
{
    int indexMax = 0;
    for (int i = 0; i < m_board.size(); i++)
    {
        for (int j = 0; j < m_board[i].size(); j++)
        {
            if (!m_board[i][j].empty())
            {
                if (j > indexMax)
                    indexMax = j;
            }
        }
    }
    return indexMax;
}

Board::Board(const Board& board)
    :m_board{ board.m_board },
    m_size{ board.m_size }
{
}

Board& Board::operator=(const Board& other)
{
    if (this == &other) {
        return *this;
    }

    m_board = other.m_board;
    m_size = other.m_size;

    return *this;
}

void Board::setBoard(const matrix& board)
{
    m_board = board;
}

std::ostream& operator<<(std::ostream& os, const Board& board)
{
    for (int16_t i = 0; i < board.m_board.size(); i++) {
        for (int16_t j = 0; j < board.m_board.size(); j++)
        {
            if (board.m_board[i][j].empty() == true) {
                os << 0 << " ";
            }
            else {
                os << board.m_board[i][j].back() << " ";
            }
        }
        os << '\n';
    }

    return os;
}

std::istream& operator>>(std::istream& in, Board& board)
{
    for (int16_t i = 0; i < board.m_board.size(); i++) {
        for (int16_t j = 0; j < board.m_board.size(); j++)
        {
            int16_t val;
            in >> val;
            board.m_board[i][j].push_back(SimpleCard(val, Color::Red));
        }
    }
    return in;
}

std::deque<SimpleCard>& Board::operator[](const Board::Position& position)
{
    auto& [line, column] = position;

    int16_t kRows = m_board.size();
    int16_t kColumns = m_board[0].size();

    if (line < 0 || line >= kRows || column < 0 || column >= kColumns)
        throw std::out_of_range("Position out of bounds");

    return m_board[line][column];
}

const std::deque<SimpleCard>& Board::operator[](const Position& position) const
{
    auto& [line, column] = position;

    int16_t kRows = m_board.size();
    int16_t kColumns = m_board[0].size();

    if (line < 0 || line >= kRows || column < 0 || column >= kColumns)
        throw std::out_of_range("Position out of bounds");

    return m_board[line][column];
}