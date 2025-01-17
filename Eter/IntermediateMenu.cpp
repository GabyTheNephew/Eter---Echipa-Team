#include "IntermediateMenu.h"


IntermediateMenu::IntermediateMenu(QWidget* parent) : QWidget(parent) {
    
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);

    setFixedSize(400, 300);
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    move(x, y);

   
    setStyleSheet("background-color: #222222; border-radius: 15px; color: white;");


    illusionsCheckBox = new QCheckBox("Illusions", this);
    explosionsCheckBox = new QCheckBox("Explosions", this);
    timerCheckBox = new QCheckBox("Timer", this);

    QString checkboxStyle = R"(
        QCheckBox {
            font-size: 16px;
            padding: 5px;
        }
        QCheckBox::indicator {
            width: 20px;
            height: 20px;
        }
        QCheckBox::indicator:checked {
            background-color: #00ff00;
            border: 1px solid #555;
        }
        QCheckBox::indicator:unchecked {
            background-color: #ff0000;
            border: 1px solid #555;
        }
    )";
    illusionsCheckBox->setStyleSheet(checkboxStyle);
    explosionsCheckBox->setStyleSheet(checkboxStyle);
    timerCheckBox->setStyleSheet(checkboxStyle);


    startButton = new QPushButton("Start", this);
    goBackButton = new QPushButton("Go Back", this);

    QString buttonStyle = R"(
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
    )";
    startButton->setStyleSheet(buttonStyle);
    goBackButton->setStyleSheet(buttonStyle);


    QVBoxLayout* checkBoxLayout = new QVBoxLayout();
    checkBoxLayout->setContentsMargins(30, 30, 30, 10);
    checkBoxLayout->addWidget(illusionsCheckBox);
    checkBoxLayout->addWidget(explosionsCheckBox);
    checkBoxLayout->addWidget(timerCheckBox);
    checkBoxLayout->setSpacing(20);


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(30, 10, 30, 30);
    buttonLayout->addWidget(goBackButton);
    buttonLayout->addWidget(startButton);


    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(checkBoxLayout);
    mainLayout->addLayout(buttonLayout);


    connect(startButton, &QPushButton::clicked, this, [this]() {
        emit startSelected(
            illusionsCheckBox->isChecked(),
            explosionsCheckBox->isChecked(),
            timerCheckBox->isChecked()
        );
        });

    connect(goBackButton, &QPushButton::clicked, this, &IntermediateMenu::goBackSelected);
}
