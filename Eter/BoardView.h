#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include "Board.h"

class BoardView : public QWidget {
    Q_OBJECT

public:
    explicit BoardView(Board& board, QWidget* parent = nullptr);

    void updateView();

private:
    QGridLayout* gridLayout;
    Board& board;
};
