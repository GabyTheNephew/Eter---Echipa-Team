#include "BoardView.h"

BoardView::BoardView(Board& boardInstance, QWidget* parent, int maxSize)
    : QWidget(parent), board(boardInstance), maxSize(maxSize) {

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout(this);


    gridLayout->setSpacing(5);
    gridLayout->setContentsMargins(10, 10, 10, 10);

    this->setFixedSize(400, 400);

    mainLayout->addStretch();
    mainLayout->addLayout(gridLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);

    isMaxSize = false;

    setStyleSheet(
        "BoardView {"
        "    background-color: rgba(0, 0, 0, 0.1);"
        "    border-radius: 10px;"
        "}"
    );
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
        cellButton->setStyleSheet(getButtonStyle(0, 0));

        connect(cellButton, &QPushButton::clicked, this, [this]() {
            emit cellClicked(0, 0);
            });
        gridLayout->addWidget(cellButton, 1, 1);
        gridLayout->activate();
        qDebug() << "Grid updated with empty board - no cards present.";
        return;
    }


    int minRow, maxRow, minCol, maxCol;

    if (isMaxSize) {
        minRow = 0;
        maxRow = maxSize - 1;
        minCol = 0;
        maxCol = maxSize - 1;
        qDebug() << "Board is at max size - displaying all squares";
    }
    else {

        minRow = std::max(0, board.getRowSize() == 1 ? -1 : 0);
        maxRow = std::min(maxSize - 1, board.getRowSize() == 1 ? 1 : board.getRowSize() - 1);
        minCol = std::max(0, board.getColumnSize() == 1 ? -1 : 0);
        maxCol = std::min(maxSize - 1, board.getColumnSize() == 1 ? 1 : board.getColumnSize() - 1);

        
        minRow = std::max(-1, minRow - 1);
        maxRow = std::min(maxSize, maxRow + 1);
        minCol = std::max(-1, minCol - 1);
        maxCol = std::min(maxSize, maxCol + 1);
    }

    int offsetRow = minRow < 0 ? 1 : 0;
    int offsetCol = minCol < 0 ? 1 : 0;

    for (int boardRow = minRow; boardRow <= maxRow; ++boardRow) {
        for (int boardCol = minCol; boardCol <= maxCol; ++boardCol) {

            if (!shouldDisplayPosition(boardRow, boardCol)) {
                continue;
            }
            QPushButton* cellButton = new QPushButton(this);
            cellButton->setFixedSize(80, 80);

            int gridRow = boardRow + offsetRow;
            int gridCol = boardCol + offsetCol;


            bool isWithinBoard = (boardRow >= 0 && boardRow < board.getRowSize() &&
                boardCol >= 0 && boardCol < board.getColumnSize());
            bool hasCard = isWithinBoard && !board[{boardRow, boardCol}].empty();

            if (hasCard)
            {

                const SimpleCard& card = board[{boardRow, boardCol}].back();
                QString imagePath;

                if (card.getColor() == Color::IlusionBlue)
                {
                    imagePath = "ib.png";
                }
                else
                    if (card.getColor() == Color::IlusionRed)
                    {
						imagePath = "ir.png";
                   }
                else {
                    imagePath = (card.getColor() == Color::Red ? "red" : "blue");
                    imagePath += QString::number(card.getValue()) + ".jpg";
                }

                QPixmap pixmap(imagePath);
                if (!pixmap.isNull()) {
                    QPixmap scaledPixmap = pixmap.scaled(cellButton->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    cellButton->setIcon(QIcon(scaledPixmap));
                    cellButton->setIconSize(cellButton->size());
                }
                else {
                    cellButton->setText(QString::number(card.getValue()));
                }
            }
            cellButton->setStyleSheet(getButtonStyle(boardRow, boardCol));


            if (!board.canBePlaced(boardRow, boardCol) && !hasCard)
            {
                cellButton->setEnabled(false);
            }

            connect(cellButton, &QPushButton::clicked, [this, boardRow, boardCol]() {
                emit cellClicked(boardRow, boardCol);
                });

            gridLayout->addWidget(cellButton, gridRow, gridCol);


        }
    }

        gridLayout->activate();
        qDebug() << getBoardStatusInfo();
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

QString BoardView::getBoardStatusInfo() const
{
    QString info;
	QString maxsize = getIsMaxSize() ? "Yes" : "No";
    info += "Board Size: " + QString::number(board.getRowSize()) + "x" + QString::number(board.getColumnSize()) + "\n";
    info += "Max Size: " + QString::number(maxSize) + "x" + QString::number(maxSize) + "\n";
    info += "Is Max Size: " +maxsize + "\n";

    int occupiedPositions = 0;
    int totalPositions = board.getRowSize() * board.getColumnSize();

    for (int i = 0; i < board.getRowSize(); ++i) {
        for (int j = 0; j < board.getColumnSize(); ++j) {
            if (!board[{i, j}].empty()) {
                occupiedPositions++;
            }
        }
    }

    info += "Occupied Positions: " + QString::number(occupiedPositions) + "/" + QString::number(totalPositions) + "\n";
    return info;

}


bool BoardView::shouldDisplayPosition(int row, int col) const {
    if (isMaxSize) {
        return (row >= 0 && row < maxSize && col >= 0 && col < maxSize);
    }

    return board.canBePlaced(row, col) ||
        (row >= 0 && row < board.getRowSize() && col >= 0 && col < board.getColumnSize() && !board[{row, col}].empty());
}

QString BoardView::getButtonStyle(int row, int col)const
{
    bool isWithinBoard = (row >= 0 && row < board.getRowSize() && col >= 0 && col < board.getColumnSize());
    bool hasCard = isWithinBoard && !board[{row, col}].empty();

    if (hasCard) {
        return "QPushButton {"
            "    background-color: rgba(255, 255, 255, 80);"
            "    border: 2px solid black;"
            "    border-radius: 5px;"
            "}";
    }
    else if (board.canBePlaced(row, col)) {
        return "QPushButton {"
            "    background-color: rgba(200, 255, 200, 100);"
            "    border: 2px dashed #00AA00;"
            "    border-radius: 5px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(150, 255, 150, 150);"
            "    border: 2px dashed #00DD00;"
            "}";
    }
    else {
        return "QPushButton {"
            "    background-color: rgba(200, 200, 200, 50);"
            "    border: 1px solid #888888;"
            "    border-radius: 5px;"
            "}";
    }

}