#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QStackedLayout>
#include <Qlabel>
#include "Board.h"
#include <memory>
#include <vector>

class BoardView : public QWidget {
    Q_OBJECT

public:
    explicit BoardView(Board& board, QWidget* parent = nullptr, int maxSize = 3);

    void updateView();

    bool canPlaceCard(const SimpleCard& card, int row, int col) const;
    void placeCard(const SimpleCard& card, int row, int col);

    Board& getBoard();

    int getMaxSize();
    void setIsMaxSize(bool isMaxSized);
    bool getIsMaxSize() const;

signals:
    void cellClicked(int row, int col);

private:
    QGridLayout* gridLayout;
    Board& board;
    int maxSize;
    bool isMaxSize;

    std::vector<std::vector<std::unique_ptr<QPushButton>>> cellButtons;

    void createCellButton(int row, int col);
    void styleCellButton(QPushButton* button, bool canPlace, bool isEmpty, bool isInPlayableArea);
    void setupButtonIcon(QPushButton* button, const SimpleCard& card);
};