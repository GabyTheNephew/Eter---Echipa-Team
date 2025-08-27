#include "Board.h"

bool Board::needsDynamicExpansion(int16_t placementRow, int16_t placementCol, int16_t maxSize) const {
    // Pentru o tablă 2x2 inițială, trebuie să extindem dacă jucătorul încearcă să plaseze
    // pe o poziție care ar cere adiacență în afara tablei actuale

    if (getRowSize() >= maxSize && getColumnSize() >= maxSize) {
        return false; // Deja la dimensiunea maximă
    }

    // Verificăm dacă există poziții adiacente în afara tablei
    std::vector<std::pair<int16_t, int16_t>> neighbors = {
        {placementRow + 1, placementCol}, {placementRow - 1, placementCol},
        {placementRow, placementCol + 1}, {placementRow, placementCol - 1},
        {placementRow + 1, placementCol + 1}, {placementRow - 1, placementCol + 1},
        {placementRow - 1, placementCol - 1}, {placementRow + 1, placementCol - 1}
    };

    for (const auto& [nx, ny] : neighbors) {
        // Dacă orice poziție adiacentă este în afara tablei, avem nevoie de extindere
        if (nx < 0 || nx >= getRowSize() || ny < 0 || ny >= getColumnSize()) {
            return true;
        }
    }

    return false;
}

void Board::dynamicExpand(int16_t placementRow, int16_t placementCol, int16_t maxSize) {
    qDebug() << "Starting dynamic expansion for position (" << placementRow << "," << placementCol << ")";

    // Extindem în toate direcțiile necesare pentru a permite adiacența completă
    bool expanded = false;

    // Extindem în sus dacă e necesar
    if (placementRow == 0 && getRowSize() < maxSize) {
        qDebug() << "Expanding UP - adding row above";
        expandRow(RowExpandDirection::Up);
        expanded = true;
    }

    // Extindem în jos dacă e necesar
    if (placementRow == getRowSize() - 1 && getRowSize() < maxSize) {
        qDebug() << "Expanding DOWN - adding row below";
        expandRow(RowExpandDirection::Down);
        expanded = true;
    }

    // Extindem la stânga dacă e necesar
    if (placementCol == 0 && getColumnSize() < maxSize) {
        qDebug() << "Expanding LEFT - adding column to the left";
        expandColumn(ColumnExpandDirection::Left);
        expanded = true;
    }

    // Extindem la dreapta dacă e necesar
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
        return; // Deja la dimensiunea maximă
    }

    bool needsExpansion = false;

    // Găsim toate pozițiile cu cărți
    std::vector<std::pair<int16_t, int16_t>> cardPositions;
    for (int16_t i = 0; i < getRowSize(); ++i) {
        for (int16_t j = 0; j < getColumnSize(); ++j) {
            if (!m_board[i][j].empty()) {
                cardPositions.push_back({ i, j });
            }
        }
    }

    // Pentru fiecare poziție cu carte, verificăm dacă toate pozițiile adiacente sunt în tabla
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
            break; // Extindem o dată, apoi reapelăm dacă e necesar
        }
    }

    // Dacă am extins, verificăm din nou recursiv
    if (needsExpansion) {
        autoExpandForAdiacency(maxSize);
    }
}


void Board::initializeForDynamicPlay(int16_t maxSize) {
    m_size = maxSize;
    int16_t initialSize = (2 < maxSize) ? 2 : maxSize; // Începe cu 2x2 sau mai mic

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
        return { 0, 0 }; // Nicio carte pe tablă
    }

    return { maxRow - minRow + 1, maxCol - minCol + 1 }; // rows, cols
}




void Board::expandRow(RowExpandDirection direction)
{
    int16_t newSize = m_board.size() + 1;
    matrix newMatrix(newSize, std::vector<std::deque<SimpleCard>>(m_board[0].size()));

    if (direction == RowExpandDirection::Up)
    {
        // Mutăm toate rândurile cu o poziție în jos
        for (int16_t i = newSize - 1; i > 0; --i)
        {
            for (int16_t j = 0; j < m_board[0].size(); j++)
            {
                newMatrix[i][j] = std::move(m_board[i - 1][j]);
            }
        }
    }
    else // Down
    {
        // Copiez rândurile existente, ultimul rând rămâne gol
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
        // Mutăm toate coloanele cu o poziție la dreapta
        for (int16_t i = 0; i < m_board.size(); ++i)
        {
            for (int16_t j = newSize - 1; j > 0; --j)
            {
                newMatrix[i][j] = std::move(m_board[i][j - 1]);
            }
        }
    }
    else // Right
    {
        // Copiez coloanele existente, ultima coloană rămâne goală
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
    std::vector<int16_t> emptyCols;
    for (int16_t i = 0; i < getColumnSize(); i++)
    {
        bool isEmpty = true;
        for (int16_t j = 0; j < getRowSize(); j++)
        {
            if (m_board[j][i].size() != 0)
                isEmpty = false;
        }

        if (isEmpty)
        {
            emptyCols.push_back(i);
        }
    }

    return emptyCols;
}

std::vector<int16_t> Board::searchEmptyRows()
{
    std::vector<int16_t> emptyRows;
    for (int16_t i = 0; i < m_board.size(); i++)
    {
        bool isEmpty = true;
        for (int16_t j = 0; j < m_board[i].size(); j++)
        {
            if (m_board[i][j].size() != 0)
                isEmpty = false;
        }

        if (isEmpty)
        {
            emptyRows.push_back(i);
        }
    }

    return emptyRows;
}

bool Board::canBePlaced(int16_t x, int16_t y) const {
    int16_t rows = m_board.size();
    int16_t columns = m_board[0].size();

    // Verificăm dacă poziția este în limitele tablei
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }

    // Pentru prima carte, poate fi plasată oriunde pe o poziție goală
    if (getTotalCardsOnBoard() == 0) {
        return m_board[x][y].empty();
    }

    // Pentru pozițiile ocupate, întotdeauna returnăm true 
    // (verificarea valorii se face în canBePushed)
    if (!m_board[x][y].empty()) {
        return true;
    }

    // Pentru pozițiile goale, verificăm adiacența cu cele 8 direcții
    std::vector<std::pair<int16_t, int16_t>> neighbors = {
        {x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1},
        {x + 1, y + 1}, {x - 1, y + 1}, {x - 1, y - 1}, {x + 1, y - 1}
    };

    for (const auto& [nx, ny] : neighbors) {
        if (nx >= 0 && nx < rows && ny >= 0 && ny < columns) {
            if (!m_board[nx][ny].empty()) {
                return true; // Există cel puțin o carte adiacentă
            }
        }
    }

    return false; // Nu există cărți adiacente pentru poziția goală
}

int Board::getTotalCardsOnBoard() const {
    int count = 0;
    for (int16_t i = 0; i < m_board.size(); ++i) {
        for (int16_t j = 0; j < m_board[i].size(); ++j) {
            if (!m_board[i][j].empty()) {
                count++;
            }
        }
    }
    return count;
}

Board::State Board::checkWin(bool canCountPoints, int16_t boardMaxSize)
{
    int16_t kRows = m_board.size();
    int16_t kColumns = m_board[0].size();

    // Pentru tablă dinamică, creăm array-uri pentru fiecare rând, coloană și diagonale
    std::vector<int16_t> rowResults(kRows, 0);
    std::vector<int16_t> colResults(kColumns, 0);
    int16_t diag1 = 0, diag2 = 0; // diagonalele

    int16_t chessmanCount = 0;

    // Calculăm score-urile pentru fiecare rând, coloană și diagonală
    for (int16_t i = 0; i < kRows; ++i)
    {
        for (int16_t j = 0; j < kColumns; ++j)
        {
            if (!m_board[i][j].empty())
            {
                int16_t value;
                Color color = m_board[i][j].back().getColor();

                switch (color)
                {
                case Color::Red:
                case Color::IlusionRed:
                    value = 1;
                    break;
                case Color::Blue:
                case Color::IlusionBlue:
                    value = -1;
                    break;
                default:
                    value = 0;
                }

                // Rând i
                rowResults[i] += value;

                // Coloană j
                colResults[j] += value;

                // Diagonala principală (doar pentru pătrate)
                if (i == j && kRows == kColumns)
                    diag1 += value;

                // Diagonala secundară (doar pentru pătrate)
                if (i == kColumns - 1 - j && kRows == kColumns)
                    diag2 += value;

                ++chessmanCount;
            }
        }
    }

    // Verificăm câștigul prin rânduri
    for (auto result : rowResults)
    {
        if (result == boardMaxSize) {
            qDebug() << "Red wins with row!";
            return State::RedWin;
        }
        else if (result == -boardMaxSize) {
            qDebug() << "Blue wins with row!";
            return State::BlueWin;
        }
    }

    // Verificăm câștigul prin coloane
    for (auto result : colResults)
    {
        if (result == boardMaxSize) {
            qDebug() << "Red wins with column!";
            return State::RedWin;
        }
        else if (result == -boardMaxSize) {
            qDebug() << "Blue wins with column!";
            return State::BlueWin;
        }
    }

    // Verificăm câștigul prin diagonale (doar pentru table pătrate)
    if (kRows == kColumns) {
        if (diag1 == boardMaxSize || diag2 == boardMaxSize) {
            qDebug() << "Red wins with diagonal!";
            return State::RedWin;
        }
        else if (diag1 == -boardMaxSize || diag2 == -boardMaxSize) {
            qDebug() << "Blue wins with diagonal!";
            return State::BlueWin;
        }
    }

    // Verificăm dacă tabla este plină sau jucătorii au rămas fără cărți
    if (canCountPoints)
    {
        int16_t redSum = sumPoints(Color::Red);
        int16_t blueSum = sumPoints(Color::Blue);

        qDebug() << "Counting points - Red:" << redSum << ", Blue:" << blueSum;

        if (redSum > blueSum)
        {
            return State::RedWin;
        }
        else if (blueSum > redSum)
        {
            return State::BlueWin;
        }

        return State::Draw;
    }

    return State::None;
}

int16_t Board::sumPoints(const Color& color)
{
    int16_t sum = 0;

    for (int16_t i = 0; i < m_board.size(); i++)
    {
        for (int16_t j = 0; j < m_board[i].size(); j++)
        {
            if (!m_board[i][j].empty())
            {
                Color cardColor = m_board[i][j].back().getColor();

                if ((color == Color::Red && (cardColor == Color::Red || cardColor == Color::IlusionRed)) ||
                    (color == Color::Blue && (cardColor == Color::Blue || cardColor == Color::IlusionBlue)))
                {
                    if (cardColor == Color::IlusionBlue || cardColor == Color::IlusionRed)
                    {
                        sum += 1; // Iluziile valorează 1
                    }
                    else
                    {
                        sum += m_board[i][j].back().getValue();
                    }
                }
            }
        }
    }

    return sum;
}

bool Board::needsExpansion(int16_t placementRow, int16_t placementCol, int16_t maxSize) const {
    // Verificăm dacă plasarea unei cărți ar necesita extinderea tablei

    // Dacă tabla deja are dimensiunea maximă, nu mai extindem
    if (getRowSize() >= maxSize && getColumnSize() >= maxSize) {
        return false;
    }

    // Verificăm dacă plasarea este pe marginea tablei și ar necesita extinderea
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
    // Extindere inteligentă bazată pe regulile Eter

    if (getRowSize() >= maxSize && getColumnSize() >= maxSize) {
        return; // Nu mai extindem dacă am atins dimensiunea maximă
    }

    // Verificăm dacă trebuie să extindem rândurile
    if (getRowSize() < maxSize) {
        if (placementRow == 0 && hasCardsInRow(0)) {
            expandRow(RowExpandDirection::Up);
        }
        else if (placementRow == getRowSize() - 1 && hasCardsInRow(getRowSize() - 1)) {
            expandRow(RowExpandDirection::Down);
        }
    }

    // Verificăm dacă trebuie să extindem coloanele
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
    if (row < 0 || row >= getRowSize()) return false;

    for (int16_t j = 0; j < getColumnSize(); ++j) {
        if (!m_board[row][j].empty()) {
            return true;
        }
    }
    return false;
}

bool Board::hasCardsInColumn(int16_t col) const {
    if (col < 0 || col >= getColumnSize()) return false;

    for (int16_t i = 0; i < getRowSize(); ++i) {
        if (!m_board[i][col].empty()) {
            return true;
        }
    }
    return false;
}

// Restul metodelor rămân la fel...
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

Board& Board::operator=(const Board& board)
{
    if (this == &board)
    {
        return *this;
    }

    for (int16_t i = 0; i < m_board.size(); i++)
    {
        for (int16_t j = 0; j < m_board[i].size(); j++)
        {
            m_board[i][j] = board.m_board[i][j];
        }
    }

    m_size = board.m_size;

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