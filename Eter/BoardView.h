#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QStackedLayout>
#include <Qlabel>
#include "Board.h"

class BoardView : public QWidget {
    Q_OBJECT

public:
    explicit BoardView(Board& board, QWidget* parent = nullptr);

    void updateView();

    bool canPlaceCard(int row, int col) const;
    void placeCard(const SimpleCard& card, int row, int col);

signals:
    void cellClicked(int row, int col);

private:
    QGridLayout* gridLayout;
    Board& board;
};
