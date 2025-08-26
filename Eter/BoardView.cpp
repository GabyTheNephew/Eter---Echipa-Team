#include "BoardView.h"

BoardView::BoardView(Board& boardInstance, QWidget* parent, int maxSize)
    : QWidget(parent), board(boardInstance), maxSize(maxSize) {

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout(this);

    mainLayout->addStretch();
    mainLayout->addLayout(gridLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);

    isMaxSize = false;
}

void BoardView::updateView() {
    // Curățăm layout-ul existent
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Pentru tabla dinamică Eter, calculăm granițele reale
    auto [actualRows, actualCols] = board.getActualBoardBounds();

    // Creăm butoanele pentru fiecare celulă
    for (int row = 0; row < board.getRowSize(); ++row) {
        for (int col = 0; col < board.getColumnSize(); ++col) {
            QPushButton* cellButton = new QPushButton(this);
            cellButton->setFixedSize(100, 100);

            // Pentru tabla dinamică, afișăm doar zonele relevante
            bool isInPlayableArea = true;

            // Dacă tabla nu este încă fixată, evidențiem doar zonele accesibile
            if (!isMaxSize && actualRows > 0 && actualCols > 0) {
                // Calculăm zona expandabilă în jurul cărților existente
                int16_t minRow = board.getRowSize(), maxRow = -1;
                int16_t minCol = board.getColumnSize(), maxCol = -1;

                bool hasCards = false;
                for (int16_t i = 0; i < board.getRowSize(); ++i) {
                    for (int16_t j = 0; j < board.getColumnSize(); ++j) {
                        if (!board[{i, j}].empty()) {
                            hasCards = true;
                            minRow = std::min(minRow, i);
                            maxRow = std::max(maxRow, i);
                            minCol = std::min(minCol, j);
                            maxCol = std::max(maxCol, j);
                        }
                    }
                }

                if (hasCards) {
                    // Zona jucabilă include cărțile existente plus o margine de 1
                    isInPlayableArea = (row >= minRow - 1 && row <= maxRow + 1 &&
                        col >= minCol - 1 && col <= maxCol + 1);
                }
            }

            // Verificăm dacă poziția poate fi utilizată pentru plasarea unei cărți
            bool canPlace = board.canBePlaced(row, col) && isInPlayableArea;

            if (canPlace && board[{row, col}].empty()) {
                // Poziție goală unde se poate plasa o carte
                cellButton->setStyleSheet(
                    "QPushButton {"
                    "    background-color: rgba(144, 238, 144, 100);" // Verde pentru pozițiile valide
                    "    border: 2px solid green;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: rgba(144, 238, 144, 150);"
                    "}"
                );
            }
            else if (board[{row, col}].empty() && isInPlayableArea) {
                // Poziție în zona jucabilă dar nu adiacentă
                cellButton->setStyleSheet(
                    "QPushButton {"
                    "    background-color: rgba(255, 255, 220, 70);" // Galben pal pentru zona jucabilă
                    "    border: 1px solid #DDD;"
                    "}"
                );
                cellButton->setEnabled(false);
            }
            else if (board[{row, col}].empty()) {
                // Poziție în afara zonei jucabile - aproape invizibilă
                cellButton->setStyleSheet(
                    "QPushButton {"
                    "    background-color: rgba(240, 240, 240, 30);" // Foarte pal
                    "    border: 1px solid rgba(200, 200, 200, 50);"
                    "}"
                );
                cellButton->setEnabled(false);
            }
            else {
                // Poziție ocupată
                cellButton->setStyleSheet(
                    "QPushButton {"
                    "    background-color: rgba(255, 255, 255, 80);"
                    "    border: 2px solid black;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: rgba(255, 255, 255, 120);"
                    "}"
                );
            }

            // Dacă celula conține o carte, afișăm imaginea
            if (!board[{row, col}].empty()) {
                const SimpleCard& card = board[{row, col}].back();

                QString imagePath = (card.getColor() == Color::Red || card.getColor() == Color::IlusionRed) ? "red" : "blue";
                imagePath += QString::number(card.getValue()) + ".jpg";

                QPixmap pixmap(imagePath);
                if (!pixmap.isNull()) {
                    QPixmap scaledPixmap = pixmap.scaled(cellButton->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    cellButton->setIcon(QIcon(scaledPixmap));
                    cellButton->setIconSize(cellButton->size());
                }
                else {
                    qDebug() << "Image not found for card: Value =" << card.getValue()
                        << ", Color =" << (card.getColor() == Color::Red ? "Red" : "Blue");
                    cellButton->setText(QString::number(card.getValue()));
                }
            }

            // Conectăm semnalul pentru toate pozițiile relevante
            if (canPlace || !board[{row, col}].empty()) {
                connect(cellButton, &QPushButton::clicked, [this, row, col]() {
                    emit cellClicked(row, col);
                    });
            }

            gridLayout->addWidget(cellButton, row, col);
        }
    }

    gridLayout->activate();
    qDebug() << "Grid updated with size: " << board.getRowSize() << "x" << board.getColumnSize()
        << " (actual bounds: " << actualRows << "x" << actualCols << ")";
}

bool BoardView::canPlaceCard(const SimpleCard& card, int row, int col) const {
    Board::Position pos = { row, col };

    // Verificăm adiacența și dacă carta poate fi pusă deasupra
    return board.canBePlaced(row, col) && board.canBePushed(card, pos);
}

void BoardView::placeCard(const SimpleCard& card, int row, int col) {
    Board::Position pos = { row, col };

    // Această metodă este doar pentru actualizarea vizuală după ce cartea a fost deja plasată
    // Logica de verificare și expandare se face în SecondaryWindow

    // Actualizăm doar vizualizarea celulei specifice
    QLayoutItem* item = gridLayout->itemAtPosition(row, col);
    if (item) {
        QPushButton* cellButton = qobject_cast<QPushButton*>(item->widget());
        if (cellButton) {
            QString imagePath = (card.getColor() == Color::Red ? "red" : "blue");
            imagePath += QString::number(card.getValue()) + ".jpg";

            QPixmap pixmap(imagePath);
            if (!pixmap.isNull()) {
                QSize cellSize = cellButton->size();
                cellButton->setIcon(QIcon(pixmap));
                cellButton->setIconSize(cellSize);
            }
            else {
                qDebug() << "Image not found for card: Value =" << card.getValue()
                    << ", Color =" << (card.getColor() == Color::Red ? "Red" : "Blue");
                cellButton->setText(QString::number(card.getValue()));
            }
        }
    }

    qDebug() << "Card visual update completed at position (" << row << "," << col << ")";
}

Board& BoardView::getBoard()
{
    return board;
}

int BoardView::getMaxSize()
{
    return maxSize;
}

void BoardView::setIsMaxSize(bool isMaxSized)
{
    this->isMaxSize = isMaxSized;
}

bool BoardView::getIsMaxSize() const
{
    return isMaxSize;
}