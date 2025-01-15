
#include "LoadGameMenu.h"


LoadGameMenu::LoadGameMenu(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);

    setFixedSize(400, 300);
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    move(x, y);

    setStyleSheet("background-color: #222222; border-radius: 15px; color: white;");


    savesLabel = new QLabel("Available saves:", this);
    savesLabel->setStyleSheet("font-size: 16px; padding: 10px;");
    savesLabel->setAlignment(Qt::AlignCenter);


    goBackButton = new QPushButton("Go Back", this);
    goBackButton->setStyleSheet(R"(
        QPushButton {
            background-color: #444444;
            color: white;
            font-size: 16px;
            border: none;
            padding: 10px;
            border-radius: 10px;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #555555;
        }
    )");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->addWidget(savesLabel);
    mainLayout->addWidget(goBackButton, 0, Qt::AlignCenter);

    connect(goBackButton, &QPushButton::clicked, this, &LoadGameMenu::goBackSelected);
}
