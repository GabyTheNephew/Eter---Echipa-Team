#include "MenuWindow.h"

MenuWindow::MenuWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Menu");
    setFixedSize(600, 400);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);


    backgroundWidget = new QWidget(this);
    backgroundWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0.7); border-radius: 20px;");
    backgroundWidget->setGeometry(100, 0, 400, 400);


    saveButton = new QPushButton("Save Game", this);
    homeButton = new QPushButton("Home", this);
    exitButton = new QPushButton("Exit", this);


    QString buttonStyle = R"(
        QPushButton {
            background-color: black;
            color: white;
            border-radius: 25px; 
            padding: 15px;
            font-size: 18px; 
            min-width: 200px; 
            min-height: 50px; 
        }
        QPushButton:hover {
            background-color: #444444; 
        }
    )";

    saveButton->setStyleSheet(buttonStyle);
    homeButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(homeButton);
    buttonLayout->addWidget(exitButton);
    buttonLayout->setSpacing(20); 
    buttonLayout->setAlignment(Qt::AlignCenter);

    
    buttonContainer = new QWidget(this);
    buttonContainer->setLayout(buttonLayout);
    buttonContainer->setStyleSheet("background: transparent;");
    buttonContainer->setGeometry(0, 0, 600, 400);

    
    connect(exitButton, &QPushButton::clicked, this, &MenuWindow::exitApp);
    connect(homeButton, &QPushButton::clicked, this, &MenuWindow::goToHome);

    this->show();
}
