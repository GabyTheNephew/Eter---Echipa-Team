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
    
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

   
    for (int row = 0; row < board.getRowSize(); ++row) {
        for (int col = 0; col < board.getColumnSize(); ++col) {
            QPushButton* cellButton = new QPushButton(this);
            cellButton->setFixedSize(100, 100); 
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
                const SimpleCard& card = board[{row, col}].back();

           
                QString imagePath = (card.getColor() == Color::Red ? "red" : "blue");
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

       
            connect(cellButton, &QPushButton::clicked, [this, row, col]() {
                emit cellClicked(row, col);
                });

    
            gridLayout->addWidget(cellButton, row, col);
        }
    }

 
    gridLayout->activate();
    qDebug() << "Grid updated with size: " << board.getRowSize() << "x" << board.getColumnSize();
}




bool BoardView::canPlaceCard(const SimpleCard& card, int row, int col) const {
    Board::Position pos = {row, col};
    
    
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

