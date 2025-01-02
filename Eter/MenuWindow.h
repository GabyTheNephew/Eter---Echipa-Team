#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class MenuWindow : public QWidget {
    Q_OBJECT

public:
    explicit MenuWindow(QWidget* parent = nullptr);

signals:
    void goToHome();
    void exitApp();

private:
    QPushButton* saveButton;
    QPushButton* exitButton;
    QPushButton* homeButton;
    QWidget* backgroundWidget;
    QWidget* buttonContainer;
};