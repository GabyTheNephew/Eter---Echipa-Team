#include "BoardView.h"

BoardView::BoardView(Board& boardInstance, QWidget* parent)
    : QWidget(parent), board(boardInstance) {
    // Creăm un layout principal pentru centrare
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout();

    // Adăugăm grila în centru
    mainLayout->addStretch();
    mainLayout->addLayout(gridLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
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

            // Setăm dimensiunile butonului
            cellButton->setFixedSize(50, 50);

            // Adăugăm margini clare
            cellButton->setStyleSheet(
                "QPushButton {"
                "   background-color: rgba(255, 255, 255, 30);" // un pic de alb pentru a fi vizibil
                "   border: 3px solid black;"                   // border mai gros
                "   border-radius: 0px;"
                "   min-width: 80px;"                          // dimensiune minimă explicită
                "   min-height: 80px;"
                "}"
                "QPushButton:hover {"
                "   background-color: rgba(255, 255, 255, 100);"
                "}"
            );

            // Forțăm conținut text pentru vizibilitate
            cellButton->setText(" ");

            // Adăugăm butonul în layout
            gridLayout->addWidget(cellButton, row, col);

            // Conectăm click-ul pentru debug
            connect(cellButton, &QPushButton::clicked, [this, row, col]() {
                qDebug() << "Cell clicked at:" << row << col;
                });
        }
    }

    // Actualizăm layout-ul pentru a forța desenarea
    this->update();
    this->repaint();
}
