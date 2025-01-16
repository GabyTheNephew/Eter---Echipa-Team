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
    // Ștergem widget-urile existente
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Calculăm dimensiunea unui buton
    // Luăm cea mai mică dimensiune dintre lățime și înălțime și o împărțim la numărul de celule
    int availableSize = width() - (2 * 10); // Scădem marginile
    int cellSize = (availableSize) / board.getRowSize();

    // Setăm spațierea între celule
    gridLayout->setSpacing(15);

    // Creăm butoanele pentru fiecare celulă
    for (int row = 0; row < board.getRowSize(); ++row) {
        for (int col = 0; col < board.getColumnSize(); ++col) {
            QPushButton* cellButton = new QPushButton(this);

            // Setăm dimensiunea fixă pentru a menține forma pătrată
            cellButton->setFixedSize(cellSize, cellSize);

            cellButton->setStyleSheet(
                "QPushButton {"
                "    background-color: rgba(255, 255, 255, 50);"
                "    border: 2px solid black;"
                "}"
                "QPushButton:hover {"
                "    background-color: rgba(255, 255, 255, 100);"
                "}"
            );

            if (!board[{row, col}].empty()) {
                cellButton->setText(QString::number(board[{row, col}].back().getValue()));
            }

            gridLayout->addWidget(cellButton, row, col);

            connect(cellButton, &QPushButton::clicked, [this, row, col]() {
                qDebug() << "Cell clicked at:" << row << col;
                });
        }
    }

    // Forțăm actualizarea layout-ului
    gridLayout->activate();
}
