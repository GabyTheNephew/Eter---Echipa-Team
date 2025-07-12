#include "IntermediateMenu.h"


IntermediateMenu::IntermediateMenu(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);

    setFixedSize(400, 450);
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

   
    emailLabel = new QLabel("Email:", this);
    emailLabel->setStyleSheet("font-size: 14px; color: white; padding: 5px;");

    emailLineEdit = new QLineEdit(this);
    emailLineEdit->setPlaceholderText("Enter your email...");
    emailLineEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: #333333;
            color: white;
            border: 2px solid #555555;
            border-radius: 5px;
            padding: 8px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 2px solid #00ff00;
        }
    )");

    passwordLabel = new QLabel("Password of min 5 charcters and at least 1 special:", this);
    passwordLabel->setStyleSheet("font-size: 14px; color: white; padding: 5px;");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Enter your password...");
    passwordLineEdit->setEchoMode(QLineEdit::Password);  
    passwordLineEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: #333333;
            color: white;
            border: 2px solid #555555;
            border-radius: 5px;
            padding: 8px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 2px solid #00ff00;
        }
    )");

   
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

   
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);

  
    mainLayout->addWidget(illusionsCheckBox);
    mainLayout->addWidget(explosionsCheckBox);
    mainLayout->addWidget(timerCheckBox);

    mainLayout->addSpacing(15);  

    
    mainLayout->addWidget(emailLabel);
    mainLayout->addWidget(emailLineEdit);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLineEdit);

    mainLayout->addSpacing(15);

   
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(goBackButton);
    buttonLayout->addWidget(startButton);
    mainLayout->addLayout(buttonLayout);

    
    connect(startButton, &QPushButton::clicked, this, &IntermediateMenu::onStartClicked);
    connect(goBackButton, &QPushButton::clicked, this, &IntermediateMenu::goBackSelected);
}

void IntermediateMenu::onStartClicked() {
    if (validateInput()) {
        emit startSelected(
            illusionsCheckBox->isChecked(),
            explosionsCheckBox->isChecked(),
            timerCheckBox->isChecked(),
            emailLineEdit->text(),
            passwordLineEdit->text()
        );
    }
}


bool IntermediateMenu::validateInput() {
    QString email = emailLineEdit->text().trimmed();
    QString password = passwordLineEdit->text();

    if (email.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter an email address!");
        return false;
    }

    if (!isValidEmail(email.toStdString())) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid email format!");
        return false;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a password!");
        return false;
    }

    if (!isValidPassword(password.toStdString())) {
        QMessageBox::warning(this, "Validation Error",
            "Password need to be at least 5 charcaters including (!@#$%^&*)!");
        return false;
    }

    return true;
}


bool IntermediateMenu::isValidEmail(const std::string& email) {
    std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, emailPattern);
}

bool IntermediateMenu::isValidPassword(const std::string& password) {
    std::regex passwordPattern(R"(^(?=.*[!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?]).{5,}$)");
    return std::regex_match(password, passwordPattern);
}