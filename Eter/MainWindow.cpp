#include "MainWindow.h"
#include "GameSaveManager.h"
#include "Game.h"
#include "LoadGameMenu.h"

MainWindow::MainWindow(const QString& imagePath, QWidget* parent)
    : QWidget(parent), imagePath(imagePath) {
    setWindowTitle("Main Window");
    this->setAutoFillBackground(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topRightLayout = new QHBoxLayout();
    topRightLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Enter your name");
    nameInput->setFixedWidth(200);

    doneButton = new QPushButton("Done", this);
    nameLabel = new QLabel(this);
    nameLabel->setStyleSheet("color: white; font-size: 14px;");

    topRightLayout->addWidget(nameInput);
    topRightLayout->addWidget(doneButton);

    QVBoxLayout* rightContainer = new QVBoxLayout();
    rightContainer->addLayout(topRightLayout);
    rightContainer->addWidget(nameLabel, 0, Qt::AlignRight);

    mainLayout->addLayout(rightContainer);
    mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QStringList buttonTexts = { "Load Game", "Training", "Mage Duel", "Power Duel", "Mage and Power Duel", "Tournament", "Exit" };

    for (const QString& text : buttonTexts) {
        QPushButton* button = new QPushButton(text, this);
        button->setFixedSize(200, 50);
        mainLayout->addWidget(button, 0, Qt::AlignCenter);

        connect(button, &QPushButton::clicked, [this, text, imagePath]() {
            if (text == "Load Game") {
                Game& gameInstance = Game::get_Instance();
                if (GameSaveManager::Load("savegame.txt", gameInstance)) {
                    QMessageBox::information(this, "Load Game", "Game loaded successfully!");

                    if (auto* win = gameInstance.getCurrentGameWindow()) {
                        win->show();
                        win->raise();
                        win->activateWindow();
                        win->setFocus(Qt::ActiveWindowFocusReason);
                    }
                    this->hide();
                }
                else {
                    QMessageBox::warning(this, "Load Game", "No saved game found!");
                }
                return;
            }

            if (text == "Training") {
                auto* backgroundWindow = new QWidget();
                backgroundWindow->setWindowTitle("Training Setup");
                backgroundWindow->setAttribute(Qt::WA_DeleteOnClose);
                backgroundWindow->setWindowModality(Qt::ApplicationModal);
                backgroundWindow->setAutoFillBackground(true);

                QPalette palette = backgroundWindow->palette();
                palette.setBrush(QPalette::Window,
                    QBrush(QPixmap(imagePath).scaled(QGuiApplication::primaryScreen()->size(),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                backgroundWindow->setPalette(palette);

                QVBoxLayout* layout = new QVBoxLayout(backgroundWindow);
                layout->setAlignment(Qt::AlignCenter);
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                layout->addWidget(intermediateMenu);

                this->setWindowState(Qt::WindowMinimized);

                connect(intermediateMenu, &IntermediateMenu::startSelected, this,
                    [this, backgroundWindow](bool illusions, bool explosions, bool timer) {
                        backgroundWindow->close();
                        Game& gameInstance = Game::get_Instance();
                        gameInstance.setIllusionsEnabled(illusions);
                        gameInstance.setExplosionsEnabled(explosions);
                        gameInstance.setCurrentGameType(Game::GameType::Training);
                        gameInstance.startGame(Game::GameType::Training);
                    });

                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this,
                    [this, backgroundWindow]() {
                        backgroundWindow->close();
                        this->show();
                    });

                backgroundWindow->showFullScreen();
                return;
            }

            if (text == "Mage Duel") {
                auto* backgroundWindow = new QWidget();
                backgroundWindow->setWindowTitle("Mage Duel Setup");
                backgroundWindow->setAttribute(Qt::WA_DeleteOnClose);
                backgroundWindow->setWindowModality(Qt::ApplicationModal);
                backgroundWindow->setAutoFillBackground(true);

                QPalette palette = backgroundWindow->palette();
                palette.setBrush(QPalette::Window,
                    QBrush(QPixmap(imagePath).scaled(QGuiApplication::primaryScreen()->size(),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                backgroundWindow->setPalette(palette);

                QVBoxLayout* layout = new QVBoxLayout(backgroundWindow);
                layout->setAlignment(Qt::AlignCenter);
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                layout->addWidget(intermediateMenu);

                this->setWindowState(Qt::WindowMinimized);

                connect(intermediateMenu, &IntermediateMenu::startSelected, this,
                    [this, backgroundWindow](bool illusions, bool explosions, bool timer) {
                        backgroundWindow->close();
                        Game& gameInstance = Game::get_Instance();
                        gameInstance.setIllusionsEnabled(illusions);
                        gameInstance.setExplosionsEnabled(explosions);
                        gameInstance.setCurrentGameType(Game::GameType::MageDuel);
                        gameInstance.startGame(Game::GameType::MageDuel);
                    });

                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this,
                    [this, backgroundWindow]() {
                        backgroundWindow->close();
                        this->show();
                    });

                backgroundWindow->showFullScreen();
                return;
            }

            if (text == "Power Duel") {
                auto* backgroundWindow = new QWidget();
                backgroundWindow->setWindowTitle("Power Duel Setup");
                backgroundWindow->setAttribute(Qt::WA_DeleteOnClose);
                backgroundWindow->setWindowModality(Qt::ApplicationModal);
                backgroundWindow->setAutoFillBackground(true);

                QPalette palette = backgroundWindow->palette();
                palette.setBrush(QPalette::Window,
                    QBrush(QPixmap(imagePath).scaled(QGuiApplication::primaryScreen()->size(),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                backgroundWindow->setPalette(palette);

                QVBoxLayout* layout = new QVBoxLayout(backgroundWindow);
                layout->setAlignment(Qt::AlignCenter);
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                layout->addWidget(intermediateMenu);

                this->setWindowState(Qt::WindowMinimized);

                connect(intermediateMenu, &IntermediateMenu::startSelected, this,
                    [this, backgroundWindow](bool illusions, bool explosions, bool timer) {
                        backgroundWindow->close();
                        Game& gameInstance = Game::get_Instance();
                        gameInstance.setIllusionsEnabled(illusions);
                        gameInstance.setExplosionsEnabled(explosions);
                        gameInstance.setCurrentGameType(Game::GameType::Power);
                        gameInstance.startGame(Game::GameType::Power);
                    });

                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this,
                    [this, backgroundWindow]() {
                        backgroundWindow->close();
                        this->show();
                    });

                backgroundWindow->showFullScreen();
                return;
            }

            if (text == "Mage and Power Duel") {
                auto* backgroundWindow = new QWidget();
                backgroundWindow->setWindowTitle("Mage and Power Duel Setup");
                backgroundWindow->setAttribute(Qt::WA_DeleteOnClose);
                backgroundWindow->setWindowModality(Qt::ApplicationModal);
                backgroundWindow->setAutoFillBackground(true);

                QPalette palette = backgroundWindow->palette();
                palette.setBrush(QPalette::Window,
                    QBrush(QPixmap(imagePath).scaled(QGuiApplication::primaryScreen()->size(),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                backgroundWindow->setPalette(palette);

                QVBoxLayout* layout = new QVBoxLayout(backgroundWindow);
                layout->setAlignment(Qt::AlignCenter);
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                layout->addWidget(intermediateMenu);

                this->setWindowState(Qt::WindowMinimized);

                connect(intermediateMenu, &IntermediateMenu::startSelected, this,
                    [this, backgroundWindow](bool illusions, bool explosions, bool timer) {
                        backgroundWindow->close();
                        Game& gameInstance = Game::get_Instance();
                        gameInstance.setIllusionsEnabled(illusions);
                        gameInstance.setExplosionsEnabled(explosions);
                        gameInstance.setCurrentGameType(Game::GameType::MageDuelAndPower);
                        gameInstance.startGame(Game::GameType::MageDuelAndPower);
                    });

                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this,
                    [this, backgroundWindow]() {
                        backgroundWindow->close();
                        this->show();
                    });

                backgroundWindow->showFullScreen();
                return;
            }

            if (text == "Tournament") {
                Game& gameInstance = Game::get_Instance();
                gameInstance.setCurrentGameType(Game::GameType::Tournament);
                gameInstance.startGame(Game::GameType::Tournament);
                return;
            }

            if (text == "Exit") {
                QApplication::quit();
                return;
            }
            });
    }

    mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    this->setLayout(mainLayout);
    this->showFullScreen();

    connect(doneButton, &QPushButton::clicked, this, [this]() {
        QString name = nameInput->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Invalid Name", "Please enter a name.");
            return;
        }

        if (containsObsceneWord(name)) {
            QMessageBox::warning(this, "Invalid Name", "Obscene words are not allowed!");
            return;
        }

        nameLabel->setText("Player: " + name);
        });
}

bool MainWindow::containsObsceneWord(const QString& text) {
    QFile file("obscene_words.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open obscene_words.txt";
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString word = in.readLine().trimmed();
        if (word.isEmpty()) continue;

        QRegularExpression regex("\\b" + QRegularExpression::escape(word) + "\\b",
            QRegularExpression::CaseInsensitiveOption);

        if (regex.match(text).hasMatch()) {
            return true;
        }
    }
    return false;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    QPixmap backgroundPixmap(imagePath);
    if (!backgroundPixmap.isNull()) {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window,
            QBrush(backgroundPixmap.scaled(this->size(),
                Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        this->setPalette(palette);
    }
}
