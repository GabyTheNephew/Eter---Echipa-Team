#pragma once

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QApplication>

class IntermediateMenu : public QWidget {
    Q_OBJECT

public:
    explicit IntermediateMenu(QWidget* parent = nullptr);

signals:
    void startSelected(bool illusions, bool explosions, bool timer);
    void goBackSelected();

private:
    QCheckBox* illusionsCheckBox;
    QCheckBox* explosionsCheckBox;
    QCheckBox* timerCheckBox;
    QPushButton* startButton;
    QPushButton* goBackButton;
};
