#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QScreen> 
#include <QApplication>

class LoadGameMenu : public QWidget {
    Q_OBJECT

public:
    explicit LoadGameMenu(QWidget* parent = nullptr);

signals:
    void goBackSelected();

private:
    QLabel* savesLabel;
    QPushButton* goBackButton;
};
