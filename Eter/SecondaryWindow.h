#pragma once

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include "MenuWindow.h"
#include "BoardView.h"
#include "Board.h"
#include "Game.h"
#include <algorithm>
#include <QMessageBox>


class Game;
class SecondaryWindow : public QWidget {
    Q_OBJECT

public:
    explicit SecondaryWindow(const QString& title, const QString& imagePath, Game* gameInstance, QWidget* parent = nullptr);
    void setBoard(Board& board);
    void setPlayer1Cards(const std::vector<SimpleCard>& cards);
    void setPlayer2Cards(const std::vector<SimpleCard>& cards);

    void setCurrentPlayer(Color player);
    void resetBoard();
    void showWinner(const QString& winnerName);

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QString imagePath;
    MenuWindow* menu = nullptr;
    BoardView* m_boardView;
    QVBoxLayout* mainLayout;
    QHBoxLayout* player1CardsLayout;
    QHBoxLayout* player2CardsLayout;
    SimpleCard selectedCard;
    Color currentPlayer;
    Game* game;

private slots:
    void onCardSelected(const SimpleCard& card); // Gestionare selecție carte
    void onBoardClicked(int row, int col);


signals:
    void boardClicked(int row, int col, int player);

};