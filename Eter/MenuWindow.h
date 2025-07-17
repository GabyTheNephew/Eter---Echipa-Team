#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QKeyEvent>

class MenuWindow : public QWidget {
    Q_OBJECT

public:
    explicit MenuWindow(QWidget* parent = nullptr);

signals:
    void goToHome();
    void exitApp();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
#pragma region Buttons
    QPushButton* saveButton;
    QPushButton* exitButton;
    QPushButton* homeButton;
#pragma endregion

#pragma region Widgets
    QWidget* backgroundWidget;
    QWidget* buttonContainer;
#pragma endregion

    
};
