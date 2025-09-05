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
#include "PowerGale.h"
#include "PowerEarthquake.h"
#include "PowerTide.h"
#include "PowerStorm.h"
#include "PowerSquall.h"
#include <algorithm>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <memory> // Added for smart pointers

// Also make sure you have the QTimer include in SecondaryWindow.cpp:
#include <QTimer>
#include <QEventLoop>
class Game;
class SecondaryWindow : public QWidget {
    Q_OBJECT

public:
    void setMagesCompact(const QString& mage1Name, const QString& mage2Name);

    void clearCardSelection();
    explicit SecondaryWindow(const QString& title, const QString& imagePath, Game* gameInstance,
        const QString& mage1Name, const QString& mage2Name, const QString& power1Name, const QString& power2Name, bool checkMage, bool checkPower, QWidget* parent = nullptr);
    void setBoard(Board& board, int boardMaxSize);
    void setPlayer1Cards(const std::vector<SimpleCard>& cards);
    void setPlayer2Cards(const std::vector<SimpleCard>& cards);

    void setCurrentPlayer(Color player);
    void resetBoard();
    void showWinner(const QString& winnerName);
    void updateBoardView();
    void resetView();

    // Round and match management
    void updateMatchInfo(int currentRound, int player1Score, int player2Score, int roundsToWin);
    void showRoundWinner(const QString& winnerName, int currentRound);

    void setMages(const QString& mage1Name, const QString& mage2Name);
    void setPowers(const QString& power1Name, const QString& power2Name);
    void setMagesAndPowers(const QString& mage1Name, const QString& mage2Name, const QString& power1Name, const QString& power2Name);
signals:
    void closed();
    void returnToMainMenu();

protected:
    void closeEvent(QCloseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void refreshCardDisplays();
    // In SecondaryWindow.h, add these declarations to the private section:
    void setPowersCompact(const QString& power1Name, const QString& power2Name);
    void setMagesAndPowersCompact(const QString& mage1Name, const QString& mage2Name, const QString& power1Name, const QString& power2Name);

    int selectedCardIndex = -1;      // Indexul exact al cărții selectate
    Color selectedCardPlayer = Color::Red;
    QString imagePath;

    // Changed from raw pointer to unique_ptr
    std::unique_ptr<MenuWindow> menu{ nullptr };

    // Changed from raw pointer to unique_ptr
    std::unique_ptr<BoardView> m_boardView{ nullptr };

    // These layouts are managed by Qt's parent-child system, so we keep them as raw pointers
    QVBoxLayout* mainLayout;
    QHBoxLayout* player1CardsLayout;
    QHBoxLayout* player2CardsLayout;

    // UI elements for match info - managed by Qt's parent-child system
    QLabel* matchInfoLabel;
    QLabel* roundInfoLabel;
    QHBoxLayout* matchInfoLayout;

    SimpleCard selectedCard;
    Color currentPlayer;
    Game* game; // Keep as raw pointer since Game is a singleton
    void cleanupEmptyBorders();
    void setupMatchInfoUI();

private slots:
    void onCardSelected(const SimpleCard& card, int cardIndex);
    void onBoardClicked(int row, int col);
    void onMageClicked(const QString& mageName, const Color& color);
    void onPowerClicked(const QString& powerName, const Color& color);

signals:
    void boardClicked(int row, int col, int player);
};