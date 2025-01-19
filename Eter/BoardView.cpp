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
    // Ștergem toate widget-urile existente din layout
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Reconstruim grila complet
    for (int row = 0; row < board.getRowSize(); ++row) {
        for (int col = 0; col < board.getColumnSize(); ++col) {
            QPushButton* cellButton = new QPushButton(this);
            cellButton->setFixedSize(100, 100); // Dimensiunea celulelor
            cellButton->setStyleSheet(
                "QPushButton {"
                "    background-color: rgba(255, 255, 255, 50);"
                "    border: 2px solid black;"
                "}"
                "QPushButton:hover {"
                "    background-color: rgba(255, 255, 255, 100);"
                "}"
            );

            // Verificăm dacă celula conține o carte
            if (!board[{row, col}].empty()) {
                const SimpleCard& card = board[{row, col}].back(); // Ultima carte din celulă

                // Construim calea imaginii
                QString imagePath = (card.getColor() == Color::Red ? "red" : "blue");
                imagePath += QString::number(card.getValue()) + ".jpg";

                QPixmap pixmap(imagePath);
                if (!pixmap.isNull()) {
                    QPixmap scaledPixmap = pixmap.scaled(cellButton->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    cellButton->setIcon(QIcon(scaledPixmap));
                    cellButton->setIconSize(cellButton->size());
                }
                else {
                    // Dacă imaginea nu este găsită, afișăm valoarea ca fallback
                    qDebug() << "Image not found for card: Value =" << card.getValue()
                        << ", Color =" << (card.getColor() == Color::Red ? "Red" : "Blue");
                    cellButton->setText(QString::number(card.getValue()));
                }
            }

            // Conectăm semnalul butonului la clicuri
            connect(cellButton, &QPushButton::clicked, [this, row, col]() {
                emit cellClicked(row, col); // Emită coordonatele clicului
                });

            // Adăugăm butonul în grilă
            gridLayout->addWidget(cellButton, row, col);
        }
    }

    // Forțăm reîmprospătarea layout-ului
    gridLayout->activate();
    qDebug() << "Grid updated with size: " << board.getRowSize() << "x" << board.getColumnSize();
}




bool BoardView::canPlaceCard(const SimpleCard& card, int row, int col) const {
    Board::Position pos = {row, col};
    
    // Verifică dacă poziția este validă și dacă cartea respectă regulile de plasare
    return board.canBePlaced(row, col) &&  board.canBePushed(card, pos);
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

