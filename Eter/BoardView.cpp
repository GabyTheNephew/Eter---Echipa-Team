#include "BoardView.h"

BoardView::BoardView(Board& boardInstance, QWidget* parent)
    : QWidget(parent), board(boardInstance) {
    // Creăm un layout principal pentru centrare
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout(this);

    // Adăugăm grila în centru
    mainLayout->addStretch();
    mainLayout->addLayout(gridLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void BoardView::updateView() {
    // Ștergem widget-urile existente din grilă
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget(); // Ștergem widget-ul asociat
        delete item;           // Ștergem item-ul din layout
    }

    // Calculăm dimensiunea celulelor
    int availableSize = width() - (2 * 10); // Dimensiunea disponibilă pe lățime (margini incluse)
    int cellSize = (availableSize) / board.getRowSize();

    // Setăm spațierea între celule
    gridLayout->setSpacing(15);

    // Construim grila de celule
    for (int row = 0; row < board.getRowSize(); ++row) {
        for (int col = 0; col < board.getColumnSize(); ++col) {
            QPushButton* cellButton = new QPushButton(this);

            // Setăm dimensiunea fixă pentru celule (pătrată)
            cellButton->setFixedSize(cellSize, cellSize);

            // Stilizare celule
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

            // Conectăm clicul pe celulă la semnalul `cellClicked`
            connect(cellButton, &QPushButton::clicked, [this, row, col]() {
                emit cellClicked(row, col); // Emită semnalul cu coordonatele celulei clicate
                });
        }
    }

    // Forțăm actualizarea layout-ului pentru a reflecta noile widget-uri
    gridLayout->activate();
}


bool BoardView::canPlaceCard(int row, int col) const {
    return true; // Verificăm dacă celula este goală
}
void BoardView::placeCard(const SimpleCard& card, int row, int col) {
    if (canPlaceCard(row, col)) {
        // Adăugăm cartea în modelul de date
        board[{row, col}].push_back(card);

        // Găsim butonul corespunzător în grilă
        QLayoutItem* item = gridLayout->itemAtPosition(row, col);
        if (item) {
            QPushButton* cellButton = qobject_cast<QPushButton*>(item->widget());
            if (cellButton) {
                // Construim calea imaginii
                QString imagePath = (card.getColor() == Color::Red ? "red" : "blue_");
                imagePath += QString::number(card.getValue()) + ".jpg";

                // Creăm un QLabel pentru imagine
                QLabel* imageLabel = new QLabel(this);
                QPixmap pixmap(imagePath);
                if (!pixmap.isNull()) {
                    // Redimensionăm imaginea pentru a ocupa întreaga zonă a casetei
                    QSize cellSize = cellButton->size();
                    imageLabel->setPixmap(pixmap.scaled(cellSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

                    // Setăm dimensiunea imaginii exact cât dimensiunea casetei
                    imageLabel->setFixedSize(cellSize);

                    imageLabel->setAlignment(Qt::AlignCenter);
                    imageLabel->setAttribute(Qt::WA_TransparentForMouseEvents); // Permite clicul pe buton

                    // Adăugăm imaginea peste buton
                    gridLayout->addWidget(imageLabel, row, col);
                }
                else {
                    qDebug() << "Image not found for card" << card.getValue();
                    cellButton->setText(QString::number(card.getValue())); // Fallback la text
                }
            }
        }
    }
    else {
        qDebug() << "Cannot place card at:" << row << col;
    }
}
