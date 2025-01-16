#include "BoardView.h"

BoardView::BoardView(Board& boardInstance, QWidget* parent)
    : QWidget(parent), board(boardInstance) {
    gridLayout = new QGridLayout(this);
    setLayout(gridLayout);
    updateView();
}

void BoardView::updateView() {
    // Ștergem toate widget-urile din grilă
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Creăm butoane pe baza dimensiunii și stării tablei din Board
    for (int row = 0; row < board.getRowSize(); ++row) {
        for (int col = 0; col < board.getColumnSize(); ++col) {
            QPushButton* cellButton = new QPushButton(this);
            cellButton->setFixedSize(50, 50);

            // Dacă celula conține cărți, afișăm detalii
            if (!board[{row, col}].empty()) {
                cellButton->setText(QString::number(board[{row, col}].back().getValue()));
            }

            gridLayout->addWidget(cellButton, row, col);

            // Conectăm logica pentru click, dacă este necesar
            connect(cellButton, &QPushButton::clicked, [this, row, col]() {
                qDebug() << "Cell clicked at:" << row << col;
                // Logica specifică plasării unei cărți poate fi adăugată aici
                });
        }
    }
}
