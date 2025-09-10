#include "MenuWindow.h"
#include "GameSaveManager.h"
#include "Game.h"
#include <QMessageBox>

MenuWindow::MenuWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Menu");
    setFixedSize(600, 400);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::NonModal);
    setAttribute(Qt::WA_TranslucentBackground);

    backgroundWidget = new QWidget(this);
    backgroundWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0.8); border-radius: 20px;");
    backgroundWidget->setGeometry(100, 0, 400, 400);

    saveButton = new QPushButton("Save Game", this);
    homeButton = new QPushButton("Home", this);
    exitButton = new QPushButton("Exit", this);

    QString buttonStyle = R"(
        QPushButton {
            background-color: black;
            color: white;
            border-radius: 15px; 
            padding: 20px;
            font-size: 22px; 
            font-weight: bold;
            min-width: 250px; 
            min-height: 60px; 
        }
        QPushButton:hover {
            background-color: #222222; 
        }
    )";

    saveButton->setStyleSheet(buttonStyle);
    homeButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(homeButton);
    buttonLayout->addWidget(exitButton);
    buttonLayout->setSpacing(30);
    buttonLayout->setAlignment(Qt::AlignCenter);

    buttonContainer = new QWidget(this);
    buttonContainer->setLayout(buttonLayout);
    buttonContainer->setStyleSheet("background: transparent;");
    buttonContainer->setGeometry(0, 0, 600, 400);

    connect(exitButton, &QPushButton::clicked, this, &MenuWindow::exitApp);
    connect(homeButton, &QPushButton::clicked, this, &MenuWindow::goToHome);

    connect(saveButton, &QPushButton::clicked, this, [this]() {
        Game& gameInstance = Game::get_Instance();
        if (GameSaveManager::Save("savegame.txt", gameInstance)) {
            QMessageBox::information(this, "Save Game", "Game saved successfully!");
        }
        else {
            QMessageBox::warning(this, "Save Game", "Failed to save game!");
        }
        });

    this->hide();
}

void MenuWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        if (parentWidget()) {
            parentWidget()->show();
            QCoreApplication::sendEvent(parentWidget(), event);
        }
    }
    else {
        QWidget::keyPressEvent(event);
    }
}
