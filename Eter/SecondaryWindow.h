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

class SecondaryWindow : public QWidget {
    Q_OBJECT

public:
    explicit SecondaryWindow(const QString& title, const QString& imagePath, QWidget* parent = nullptr);
    void setBoard(Board& board);
    void setPlayer1Cards(const std::vector<SimpleCard>& cards);
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
};