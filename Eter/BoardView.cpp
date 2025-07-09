#include "BoardView.h"

BoardView::BoardView(Board& boardInstance, QWidget* parent, int maxSize)
    : QWidget(parent), board(boardInstance), maxSize(maxSize) {

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout(this);


    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(5, 5, 5, 5);

    mainLayout->addStretch();
    mainLayout->addLayout(gridLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);

    isMaxSize = false;
}

void BoardView::updateView() {
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

   
    bool boardHasCards = false;
    for (int i = 0; i < board.getRowSize(); ++i) {
        for (int j = 0; j < board.getColumnSize(); ++j) {
            if (!board[{i, j}].empty()) {
                boardHasCards = true;
                break;
            }
        }
        if (boardHasCards) break;
    }

    
    if (!boardHasCards) {
        QPushButton* cellButton = new QPushButton(this);
        cellButton->setFixedSize(100, 100);
        cellButton->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(200, 255, 200, 100);"
            "    border: 2px dashed #00AA00;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(150, 255, 150, 150);"
            "    border: 2px dashed #00DD00;"
            "}"
        );
        connect(cellButton, &QPushButton::clicked, this, [this]() {
            emit cellClicked(0, 0);
            });
        gridLayout->addWidget(cellButton, 1, 1);
        gridLayout->activate();
        qDebug() << "Grid updated with empty board - no cards present.";
        return;
    }

    
    int minRow = std::max(0, board.getRowSize() == 1 ? -1 : 0);
    int maxRow = std::min(maxSize - 1, board.getRowSize() == 1 ? 1 : board.getRowSize() - 1);
    int minCol = std::max(0, board.getColumnSize() == 1 ? -1 : 0);
    int maxCol = std::min(maxSize - 1, board.getColumnSize() == 1 ? 1 : board.getColumnSize() - 1);

 
    if (!isMaxSize) {
        minRow = std::max(-1, minRow - 1);
        maxRow = std::min(maxSize, maxRow + 1);
        minCol = std::max(-1, minCol - 1);
        maxCol = std::min(maxSize, maxCol + 1);
    }

    
    int offsetRow = minRow < 0 ? 1 : 0;
    int offsetCol = minCol < 0 ? 1 : 0;

    for (int boardRow = minRow; boardRow <= maxRow; ++boardRow) {
        for (int boardCol = minCol; boardCol <= maxCol; ++boardCol) {
            QPushButton* cellButton = new QPushButton(this);
            cellButton->setFixedSize(100, 100);

            int gridRow = boardRow + offsetRow;
            int gridCol = boardCol + offsetCol;

        
            bool isWithinBoard = (boardRow >= 0 && boardRow < board.getRowSize() &&
                boardCol >= 0 && boardCol < board.getColumnSize());
            bool hasCard = isWithinBoard && !board[{boardRow, boardCol}].empty();

            if (hasCard) {
             
                const SimpleCard& card = board[{boardRow, boardCol}].back();
                QString imagePath = (card.getColor() == Color::Red ? "red" : "blue");
                imagePath += QString::number(card.getValue()) + ".jpg";

                QPixmap pixmap(imagePath);
                if (!pixmap.isNull()) {
                    QPixmap scaledPixmap = pixmap.scaled(cellButton->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    cellButton->setIcon(QIcon(scaledPixmap));
                    cellButton->setIconSize(cellButton->size());
                }
                else {
                    cellButton->setText(QString::number(card.getValue()));
                }

                cellButton->setStyleSheet(
                    "QPushButton {"
                    "    background-color: rgba(255, 255, 255, 80);"
                    "    border: 2px solid black;"
                    "}"
                );
            }
            else if (board.canBePlaced(boardRow, boardCol)) {
              
                cellButton->setStyleSheet(
                    "QPushButton {"
                    "    background-color: rgba(200, 255, 200, 100);"
                    "    border: 2px dashed #00AA00;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: rgba(150, 255, 150, 150);"
                    "    border: 2px dashed #00DD00;"
                    "}"
                );
            }
            else {
                delete cellButton;
                continue;
            }
            connect(cellButton, &QPushButton::clicked, [this, boardRow, boardCol]() {
                emit cellClicked(boardRow, boardCol);
                });

            gridLayout->addWidget(cellButton, gridRow, gridCol);
        }
    }

    gridLayout->activate();
    qDebug() << "Grid updated - board size:" << board.getRowSize() << "x" << board.getColumnSize()
        << "display range: rows" << minRow << "to" << maxRow << ", cols" << minCol << "to" << maxCol;

    board.print();
}


bool BoardView::canPlaceCard(const SimpleCard& card, int row, int col) const {
    Board::Position pos = { row, col };


    return board.canBePlaced(row, col) && board.canBePushed(card, pos);
}


void BoardView::placeCard(const SimpleCard& card, int row, int col) {
    Board::Position pos = { row, col };



    {
        if (!canPlaceCard(card, row, col)) {
            qDebug() << "Cannot place card at position (" << row << "," << col << ")";
            return;
        }


        board.pushCard(card, pos);


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


        qDebug() << "Card placed successfully at position (" << row << "," << col << ")";
    }
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
    return isMaxSize;;
}

