#include "BoardView.h"

BoardView::BoardView(Board& boardInstance, QWidget* parent, int maxSize)
    : QWidget(parent), board(boardInstance), maxSize(maxSize) {
    // Creăm un layout principal pentru centrare
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout(this);

    // Adăugăm grila în centru
    mainLayout->addStretch();
    mainLayout->addLayout(gridLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);

    isMaxSize = false;
}

void BoardView::updateView() {
    // Ștergem widget-urile existente din grilă
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget(); // Ștergem widget-ul asociat
        delete item;           // Ștergem item-ul din layout
    }

    int rows = (isMaxSize == true ? maxSize : board.getRowSize());
    int cols = (isMaxSize == true ? maxSize : board.getColumnSize());

    /*int rows = isMaxSize == true ? board.getIndexOfLastRowOfBoard() : board.getRowSize();
    int cols = isMaxSize == true ? board.getIndexOfLastColumnOfBoard() : board.getColumnSize();*/

    /*int row = isMaxSize == true ? board.getIndexOfFirstRowOfBoard() : 0;
    int col = isMaxSize == true ? board.getIndexOfFirstColumnOfBoard() : 0;*/

    // Construim grila de celule
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (board.canBePlaced(row, col))
            {
                QPushButton* cellButton = new QPushButton(this);

                // Setăm dimensiunea și stilul pentru celulă
                cellButton->setFixedSize(100, 100); // Exemplu de dimensiune
                cellButton->setStyleSheet(
                    "QPushButton {"
                    "    background-color: rgba(255, 255, 255, 50);"
                    "    border: 2px solid black;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: rgba(255, 255, 255, 100);"
                    "}"
                );

                // Afișăm valoarea ultimei cărți din celulă, dacă există
                if (!board[{row, col}].empty()) {
                    cellButton->setText(QString::number(board[{row, col}].back().getValue()));
                }

                // Adăugăm butonul în grilă
                gridLayout->addWidget(cellButton, row, col);

                // *Conectăm clicul pe celulă la semnalul cellClicked*
                connect(cellButton, &QPushButton::clicked, [this, row, col]() {
                    emit cellClicked(row, col); // Emită semnalul cu coordonatele celulei clicate
                    });
            }
        }
    }

    // Forțăm actualizarea layout-ului pentru a reflecta noile widget-uri
    gridLayout->activate();
}



bool BoardView::canPlaceCard(const SimpleCard& card, int row, int col) const {
    Board::Position pos = {row, col};
    
    // Verifică dacă poziția este validă și dacă cartea respectă regulile de plasare
    return /*board.canBePlaced(row, col) && */ board.canBePushed(card, pos);
}


void BoardView::placeCard(const SimpleCard& card, int row, int col) {
    Board::Position pos = { row, col };

    // Verificăm dacă poziția permite plasarea cărții

    {
        if (!canPlaceCard(card, row, col)) {
            qDebug() << "Cannot place card at position (" << row << "," << col << ")";
            return;
        }

        // Plasăm cartea folosind pushCard din Board
        board.pushCard(card, pos);

        // Actualizăm vizualizarea celulei în grilă
        QLayoutItem* item = gridLayout->itemAtPosition(row, col);
        if (item) {
            QPushButton* cellButton = qobject_cast<QPushButton*>(item->widget());
            if (cellButton) {
                // Construim calea imaginii
                QString imagePath = (card.getColor() == Color::Red ? "red" : "blue");
                imagePath += QString::number(card.getValue()) + ".jpg";

                QPixmap pixmap(imagePath);
                if (!pixmap.isNull()) {
                    // Setăm imaginea pe buton
                    QSize cellSize = cellButton->size();
                    cellButton->setIcon(QIcon(pixmap));
                    cellButton->setIconSize(cellSize);
                }
                else {
                    qDebug() << "Image not found for card: Value =" << card.getValue()
                        << ", Color =" << (card.getColor() == Color::Red ? "Red" : "Blue");
                    cellButton->setText(QString::number(card.getValue())); // Fallback la text
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

