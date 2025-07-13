#include "LoadGameMenu.h"


LoadGameMenu::LoadGameMenu(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);

    setFixedSize(600, 500);
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    move(x, y);

    setStyleSheet("background-color: #222222; border-radius: 15px; color: white;");

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(30, 30, 30, 30);

	savesLabel = new QLabel("Available Saves:",this);
    savesLabel->setStyleSheet("font-size: 18px; padding: 10px; font-weight: bold;");
	savesLabel->setAlignment(Qt::AlignCenter);
	mainLayout->addWidget(savesLabel);


	QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            background-color: #333333;
            border: 1px solid #555555;
            border-radius: 5px;
        }
        QScrollBar:vertical {
            background-color: #444444;
            width: 12px;
            border-radius: 6px;
        }
        QScrollBar::handle:vertical {
            background-color: #666666;
            border-radius: 6px;
        }
    )");

	savesButtonWidget = new QWidget();
	savesButtonLayout = new QVBoxLayout(savesButtonWidget);

	loadAvailableSaves();

	scrollArea->setWidget(savesButtonWidget);
	scrollArea->setWidgetResizable(true);
	scrollArea->setMinimumHeight(300);

	mainLayout->addWidget(scrollArea);

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

	mainLayout->addWidget(goBackButton, 0, Qt::AlignCenter);

	connect(goBackButton, &QPushButton::clicked, this, &LoadGameMenu::goBackSelected);

}

void LoadGameMenu::loadAvailableSaves()
{
    QDir saveDir("saves");
    if (!saveDir.exists()) {
        QLabel* noSavesLabel = new QLabel("No saves directory found.", this);
        noSavesLabel->setStyleSheet("color: #888888; font-size: 14px; padding: 20px;");
        noSavesLabel->setAlignment(Qt::AlignCenter);
        savesButtonLayout->addWidget(noSavesLabel);
        return;
    }

	QStringList saveFiles = saveDir.entryList(QStringList() << "*.json", QDir::Files);

    if (saveFiles.isEmpty()) {
        QLabel* noSavesLabel = new QLabel("No save files found.", this);
        noSavesLabel->setStyleSheet("color: #888888; font-size: 14px; padding: 20px;");
        noSavesLabel->setAlignment(Qt::AlignCenter);
        savesButtonLayout->addWidget(noSavesLabel);
        return;
    }

    for (const QString& saveFile : saveFiles) {
        SaveInfo info = parseSaveFile(saveDir.filePath(saveFile));
        if (!info.isValid) continue;

        QPushButton* saveButton = createSaveButton(info);
        savesButtonLayout->addWidget(saveButton);

        connect(saveButton, &QPushButton::clicked, this, [this, saveFile]() {
            emit saveFileSelected(saveFile);
            });
    }
}

LoadGameMenu::SaveInfo LoadGameMenu::parseSaveFile(const QString& filePath)
{
    SaveInfo info;
    info.filename = QFileInfo(filePath).fileName();
    info.isValid = false;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return info;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull()) {
        return info;
    }

    QJsonObject savedData = doc.object();

    info.email = savedData["email"].toString();
    info.timestamp = savedData["timestamp"].toString();
    info.gameType = savedData["gameType"].toString();
    info.roundCounter = savedData["roundCounter"].toInt();
    info.player1Score = savedData["player1Score"].toInt();
    info.player2Score = savedData["player2Score"].toInt();
    info.isValid = !info.email.isEmpty() && !info.gameType.isEmpty();

    return info;
}

QPushButton* LoadGameMenu::createSaveButton(const SaveInfo& info)
{
    QPushButton* button = new QPushButton(this);

    
    QString buttonText = QString("%1 | %2 | Score: %3-%4")
        .arg(info.email)
        .arg(info.gameType)
        .arg(info.player1Score)
        .arg(info.player2Score);

    button->setText(buttonText);
    button->setStyleSheet(R"(
        QPushButton {
            background-color: #444444;
            color: white;
            border: 1px solid #666666;
            border-radius: 8px;
            padding: 15px;
            text-align: left;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #555555;
            border: 1px solid #777777;
        }
        QPushButton:pressed {
            background-color: #333333;
        }
    )");

    button->setMinimumHeight(50);
    button->setMaximumHeight(50);

    return button;
}



