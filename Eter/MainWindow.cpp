#include "MainWindow.h"
#include "LoadGameMenu.h"

MainWindow::MainWindow(const QString& imagePath, QWidget* parent)
    : QWidget(parent), imagePath(imagePath) {
    setWindowTitle("Main Window");

    this->setAutoFillBackground(true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);


    mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QStringList buttonTexts = { "Load Game", "Training", "Mage Duel", "Power Duel", "Mage and Power Duel", "Tournament", "Exit" };

    for (const QString& text : buttonTexts) {
        QPushButton* button = new QPushButton(text, this);
        button->setFixedSize(200, 50);
        mainLayout->addWidget(button, 0, Qt::AlignCenter);

        connect(button, &QPushButton::clicked, [this, text, imagePath]() {
            if (text == "Load Game")
            {
                for (auto& child : this->children()) {
                    if (QWidget* widget = qobject_cast<QWidget*>(child)) {
                        if (widget != this) {
                            widget->hide();
                        }
                    }
                }


                LoadGameMenu* loadGameMenu = new LoadGameMenu(this);
                loadGameMenu->setWindowModality(Qt::ApplicationModal);
                loadGameMenu->setAttribute(Qt::WA_DeleteOnClose);     
                loadGameMenu->show();

                connect(loadGameMenu, &LoadGameMenu::goBackSelected, [this, loadGameMenu]() {
                    loadGameMenu->close();
                    for (auto& child : this->children()) {
                        if (QWidget* widget = qobject_cast<QWidget*>(child)) {
                            if (widget != this) {
                                widget->show();
                            }
                        }
                    }
                    });

                connect(loadGameMenu, &LoadGameMenu::saveFileSelected, [this, loadGameMenu, imagePath](const QString& filename) {
                    
                    QString savedPassword = readPasswordFromSave(filename);
                    if (savedPassword.isEmpty()) {
                        QMessageBox::warning(this, "Error", "Could not read save file!");
                        return;
                    }

                 
                    bool ok;
                    QString inputPassword = QInputDialog::getText(this, "Password Required",
                        "Enter password for this save:", QLineEdit::Password, "", &ok);

                    if (!ok) {
                        return; 
                    }

                    
                    if (inputPassword != savedPassword) {
                        QMessageBox::warning(this, "Incorrect Password", "The password you entered is incorrect!");
                        return;
                    }

                  
                    QString email, password;
                    Game& gameInstance = Game::get_Instance();

                    if (GameSaver::loadGame(filename, gameInstance, email, password)) {
                        loadGameMenu->close();
                        this->hide();

                        
                        QString windowTitle;
                        QString mage1Name = "";
                        QString mage2Name = "";
                        QString power1Name = "";
                        QString power2Name = "";
                        bool hasMages = false;
                        bool hasPowers = false;

                        switch (gameInstance.getCurrentGameType()) {
                        case GameType::Training:
                            windowTitle = "Training - Loaded Game";
                            break;
                        case GameType::MageDuel:
                            windowTitle = "Mage Duel - Loaded Game";
                            hasMages = true;
                            mage1Name = QString::fromStdString(gameInstance.getPlayer1().getMage());
                            mage2Name = QString::fromStdString(gameInstance.getPlayer2().getMage());
                            break;
                        case GameType::Power:
                            windowTitle = "Power Duel - Loaded Game";
                            hasPowers = true;
                            power1Name = QString::fromStdString(fromPowerToQString(gameInstance.getPlayer1().getPower()).toStdString());
                            power2Name = QString::fromStdString(fromPowerToQString(gameInstance.getPlayer2().getPower()).toStdString());
                            break;
                        case GameType::MageDuelAndPower:
                            windowTitle = "Mage & Power Duel - Loaded Game";
                            hasMages = true;
                            hasPowers = true;
                            mage1Name = QString::fromStdString(gameInstance.getPlayer1().getMage());
                            mage2Name = QString::fromStdString(gameInstance.getPlayer2().getMage());
                            power1Name = QString::fromStdString(fromPowerToQString(gameInstance.getPlayer1().getPower()).toStdString());
                            power2Name = QString::fromStdString(fromPowerToQString(gameInstance.getPlayer2().getPower()).toStdString());
                            break;
                        default:
                            windowTitle = "Loaded Game";
                            break;
                        }

                       
                        auto* gameWindow = new SecondaryWindow(
                            windowTitle,
                            imagePath,
                            &gameInstance,
                            mage1Name,
                            mage2Name,
                            power1Name,
                            power2Name,
                            hasMages,
                            hasPowers
                        );

                        gameWindow->setAttribute(Qt::WA_DeleteOnClose);
                        gameWindow->setBoard(gameInstance.getBoard(), gameInstance.getBoardMaxSize());
                        gameWindow->setPlayer1Cards(gameInstance.getPlayer1().getVector());
                        gameWindow->setPlayer2Cards(gameInstance.getPlayer2().getVector());
                        gameWindow->setCurrentPlayer(gameInstance.getCurrentPlayerColor());

                        
                        connect(gameWindow, &SecondaryWindow::boardClicked, &gameInstance, &Game::handleBoardClick);

                        connect(&gameInstance, &Game::currentPlayerChanged, gameWindow, &SecondaryWindow::setCurrentPlayer);


                        connect(&gameInstance, &Game::gameEnded, this, [this, email, password]() {
                         
                            this->show();
                            }, Qt::SingleShotConnection);

                        gameWindow->show();
                       
                        QMessageBox::information(this, "Game Loaded",
                            QString("Successfully loaded game for %1\nRound: %2 | Score: %3-%4")
                            .arg(email)
                            .arg(gameInstance.getRoundCounter())
                            .arg(gameInstance.getPlayer1Score())
                            .arg(gameInstance.getPlayer2Score()));
                    }
                    else {
                        QMessageBox::warning(this, "Load Failed",
                            "Failed to load the selected game file.");
                    }
                    });
					return;
            }

            if (text == "Training") {
                qDebug() << "Intrat in if de Training\n";

               
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

           
                QVBoxLayout* mainLayout = new QVBoxLayout(backgroundWindow);
                mainLayout->setAlignment(Qt::AlignCenter);

          
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                mainLayout->addWidget(intermediateMenu);

           
                this->hide();

               
                connect(intermediateMenu, &IntermediateMenu::startSelected, this, [this, backgroundWindow](bool illusions, bool explosions, bool timer, QString email, QString password) {
                    backgroundWindow->close();

                 
                    Game& gameInstance = Game::get_Instance();
                    gameInstance.setIllusionsEnabled(illusions);
                    gameInstance.setExplosionsEnabled(explosions);
                    gameInstance.setUserCredentials(email, password);

                    QObject::connect(&gameInstance, &Game::gameEnded, this, [this, email, password]() {
         
                        this->show();
                        }, Qt::SingleShotConnection);
              
                    gameInstance.startGame(GameType::Training);
                    });

             
                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this, [this, backgroundWindow]() {
                    
                    this->show();
                    backgroundWindow->close();
                    });


                backgroundWindow->showFullScreen();

                return;
            }

            if (text == "Mage Duel") {
                qDebug() << "Intrat în Mage Duel.";

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

      
                QVBoxLayout* mainLayout = new QVBoxLayout(backgroundWindow);
                mainLayout->setAlignment(Qt::AlignCenter);

              
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                mainLayout->addWidget(intermediateMenu);

                this->hide();

      
                connect(intermediateMenu, &IntermediateMenu::startSelected, this, [this, backgroundWindow](bool illusions, bool explosions, bool timer, QString email, QString password) {
                    backgroundWindow->close();

 
                    Game& gameInstance = Game::get_Instance();
                    gameInstance.setIllusionsEnabled(illusions);
                    gameInstance.setExplosionsEnabled(explosions);
                    gameInstance.setUserCredentials(email, password);
                    QObject::connect(&gameInstance, &Game::gameEnded, this, [this, email, password]() {
                    
                        this->show();
                        }, Qt::SingleShotConnection);

                    gameInstance.startGame(GameType::MageDuel);
                    });

    
                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this, [this, backgroundWindow]() {
                    this->show();
                    backgroundWindow->close();
                    });

 
                backgroundWindow->showFullScreen();

                return;
            }

            if (text == "Power Duel") {
                qDebug() << "Intrat în Power Duel.";

            
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

 
                QVBoxLayout* mainLayout = new QVBoxLayout(backgroundWindow);
                mainLayout->setAlignment(Qt::AlignCenter);


                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                mainLayout->addWidget(intermediateMenu);


                this->hide();

                connect(intermediateMenu, &IntermediateMenu::startSelected, this, [this, backgroundWindow](bool illusions, bool explosions, bool timer, QString email, QString password) {
                    backgroundWindow->close();

                    Game& gameInstance = Game::get_Instance();
                    gameInstance.setIllusionsEnabled(illusions);
                    gameInstance.setExplosionsEnabled(explosions);
                    gameInstance.setUserCredentials(email, password);
                    QObject::connect(&gameInstance, &Game::gameEnded, this, [this, email, password]() {
                      
                        this->show();
                        }, Qt::SingleShotConnection);

                    gameInstance.startGame(GameType::Power);
                    });

          
                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this, [this, backgroundWindow]() {
                    this->show();
                    backgroundWindow->close();
                    });

            
                backgroundWindow->showFullScreen();

                return;
            }

            if (text == "Mage and Power Duel") {
                qDebug() << "Intrat în Mage and Power Duel.";

            
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

          
                QVBoxLayout* mainLayout = new QVBoxLayout(backgroundWindow);
                mainLayout->setAlignment(Qt::AlignCenter);

             
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                mainLayout->addWidget(intermediateMenu);


                this->hide();

         
                connect(intermediateMenu, &IntermediateMenu::startSelected, this, [this, backgroundWindow](bool illusions, bool explosions, bool timer, QString email, QString password) {
                    backgroundWindow->close();

            
                    Game& gameInstance = Game::get_Instance();
                    gameInstance.setIllusionsEnabled(illusions);
                    gameInstance.setExplosionsEnabled(explosions);
                    gameInstance.setUserCredentials(email, password);
                    QObject::connect(&gameInstance, &Game::gameEnded, this, [this, email, password]() {
                        
                        this->show();
                        }, Qt::SingleShotConnection);

   
                    gameInstance.startGame(GameType::MageDuelAndPower);
                    });

    
                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this, [this, backgroundWindow]() {
                     this->show();
                    backgroundWindow->close();
                    });

                backgroundWindow->showFullScreen();

                return;
            }

            if (text == "Exit") {
                Game::forceStop();
                return;
            }


            for (auto& child : this->children()) {
                if (QWidget* widget = qobject_cast<QWidget*>(child)) {
                    widget->hide();
                }
            }

            });
    }


    mainLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    this->setLayout(mainLayout);
    this->showFullScreen();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    QPixmap backgroundPixmap(imagePath);
    if (!backgroundPixmap.isNull()) {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, QBrush(backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        this->setPalette(palette);
    }
}

QString MainWindow::readPasswordFromSave(const QString& filename) {
    QFile file("saves/" + filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull()) {
        return QString();
    }

    QJsonObject savedData = doc.object();
    return savedData["password"].toString();
}
