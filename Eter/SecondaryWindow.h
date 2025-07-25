#pragma once
#include <memory>
#include <QWidget>
#include <QLabel>
#include <QTimer>
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
#include <QInputDialog>
#include "FirePowers.h"
#include "WaterPowers.h"
#include "EarthPowers.h"
#include "AirPowers.h"
class Game;
class SecondaryWindow : public QWidget {
    Q_OBJECT

public:
    explicit SecondaryWindow(const QString& title, const QString& imagePath, Game* gameInstance,const QString& mage1Name, const QString& mage2Name,  const QString& power1Name, const QString& power2Name ,bool checkMage, bool checkPower, QWidget* parent = nullptr);
#pragma region Board Relatedv Methods
    void setBoard(Board& board, int boardMaxSize);
    void resetBoard();
    void updateBoardView();
    void resetView();
#pragma endregion
   

#pragma region Player Related Methods
    void setCurrentPlayer(Color player);
    void setPlayer1Cards(const std::vector<SimpleCard>& cards);
    void setPlayer2Cards(const std::vector<SimpleCard>& cards);
    void showWinner(const QString& winnerName);
    void setMages(const QString& mage1Name, const QString& mage2Name);

    void setPowers(const QString& power1Name, const QString& power2Name);

    void setMagesAndPowers(const QString& mage1Name, const QString& mage2Name, const QString& power1Name, const QString& power2Name);
#pragma endregion

private:
#pragma region Board Methods
    bool handleCardPlacement(int row, int col, const SimpleCard& cardToPlace);
    bool handleIllusionCovering(int row, int col, const SimpleCard& attackCard, SimpleCard& illusionCard);
    bool expandBoardForPosition(int row, int col);
#pragma endregion

    
#pragma region Board Methods

    void optimizeBoard();
    void updateTimerDisplay();
    void updateMageButtons();
    void updatePowerButtons();
#pragma endregion


private:
#pragma region Game
    Game* game;
    QString imagePath;
    std::unique_ptr<MenuWindow> menu;
    BoardView* m_boardView;
    QVBoxLayout* mainLayout;
    bool selectedCardAsIllusion = false;
#pragma endregion

#pragma region Player
    QHBoxLayout* player1CardsLayout;
    QHBoxLayout* player2CardsLayout;
    SimpleCard selectedCard;
    Color currentPlayer;
    QLabel* player1TimerLabel;
    QLabel* player2TimerLabel;
    QTimer* uiUpdateTimer;

    QPushButton* mage1Button = nullptr;
    QPushButton* mage2Button = nullptr;
    QPushButton* power1Button = nullptr;
    QPushButton* power2Button = nullptr;
#pragma endregion

protected:
    void closeEvent(QCloseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onCardSelected(const SimpleCard& card); 
    void onBoardClicked(int row, int col);
    void onMageClicked(const QString& mageName, const Color& color);
    void onPowerClicked(const QString& powerName, const Color& color);

signals:
    void closed();
    void boardClicked(int row, int col, int player);
    
};