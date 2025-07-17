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
#pragma region Board Methods
 explicit BoardView(Board& board, QWidget* parent = nullptr, int maxSize = 3);
    void updateView();
    Board& getBoard();
    int getMaxSize();
    void setIsMaxSize(bool isMaxSized);
    bool getIsMaxSize()const;

	QString getBoardStatusInfo() const;
#pragma endregion

#pragma region Card Methods
bool canPlaceCard(const SimpleCard& card, int row, int col) const;
    void placeCard(const SimpleCard& card, int row, int col);
    bool shouldDisplayPosition(int row, int col) const;
    QString getButtonStyle(int row, int col) const;
#pragma endregion

signals:
    void cellClicked(int row, int col);

private:
    QGridLayout* gridLayout;
    Board& board;
    int maxSize;
    bool isMaxSize;
};
