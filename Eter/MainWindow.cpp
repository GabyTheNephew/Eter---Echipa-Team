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
            if (text == "Load Game") {
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

                return;
            }

            if (text == "Training") {
                qDebug() << "Intrat in if de Training\n";

                // Creează o fereastră cu fundal pentru IntermediateMenu
                auto* backgroundWindow = new QWidget();
                backgroundWindow->setWindowTitle("Training Setup");
                backgroundWindow->setAttribute(Qt::WA_DeleteOnClose);
                backgroundWindow->setWindowModality(Qt::ApplicationModal);
                backgroundWindow->setAutoFillBackground(true);

                // Setează fundalul
                QPalette palette = backgroundWindow->palette();
                palette.setBrush(QPalette::Window,
                    QBrush(QPixmap(imagePath).scaled(QGuiApplication::primaryScreen()->size(),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                backgroundWindow->setPalette(palette);

                // Creează layout-ul principal
                QVBoxLayout* mainLayout = new QVBoxLayout(backgroundWindow);
                mainLayout->setAlignment(Qt::AlignCenter);

                // Adaugă IntermediateMenu centrat
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                mainLayout->addWidget(intermediateMenu);

                // Ascunde fereastra principală
                this->hide();

                // Conectează semnalul `startSelected` pentru a începe jocul
                connect(intermediateMenu, &IntermediateMenu::startSelected, this, [this, backgroundWindow](bool illusions, bool explosions, bool timer) {
                    backgroundWindow->close();

                    // Configurează jocul
                    Game& gameInstance = Game::get_Instance();
                    gameInstance.setIllusionsEnabled(illusions);
                    gameInstance.setExplosionsEnabled(explosions);

                    // Lansează metoda pentru Training
                    gameInstance.startGame(Game::GameType::Training);
                    });

                // Conectează semnalul `goBackSelected` pentru a reveni la fereastra principală
                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this, [this, backgroundWindow]() {
                    backgroundWindow->close();
                    this->show(); // Reafișează fereastra principală
                    });

                // Afișează fereastra intermediară
                backgroundWindow->showFullScreen();

                return;
            }

            if (text == "Mage Duel") {
                qDebug() << "Intrat în Mage Duel.";

                // Creează o fereastră cu fundal pentru IntermediateMenu
                auto* backgroundWindow = new QWidget();
                backgroundWindow->setWindowTitle("Mage Duel Setup");
                backgroundWindow->setAttribute(Qt::WA_DeleteOnClose);
                backgroundWindow->setWindowModality(Qt::ApplicationModal);
                backgroundWindow->setAutoFillBackground(true);

                // Setează fundalul
                QPalette palette = backgroundWindow->palette();
                palette.setBrush(QPalette::Window,
                    QBrush(QPixmap(imagePath).scaled(QGuiApplication::primaryScreen()->size(),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                backgroundWindow->setPalette(palette);

                // Creează layout-ul principal
                QVBoxLayout* mainLayout = new QVBoxLayout(backgroundWindow);
                mainLayout->setAlignment(Qt::AlignCenter);

                // Adaugă IntermediateMenu centrat
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                mainLayout->addWidget(intermediateMenu);

                // Ascunde fereastra principală
                this->hide();

                // Conectează semnalul `startSelected` pentru a începe Mage Duel
                connect(intermediateMenu, &IntermediateMenu::startSelected, this, [this, backgroundWindow](bool illusions, bool explosions, bool timer) {
                    backgroundWindow->close();

                    // Configurează jocul
                    Game& gameInstance = Game::get_Instance();
                    gameInstance.setIllusionsEnabled(illusions);
                    gameInstance.setExplosionsEnabled(explosions);

                    // Lansează metoda pentru Mage Duel
                    gameInstance.startGame(Game::GameType::MageDuel);
                    });

                // Conectează semnalul `goBackSelected` pentru a reveni la fereastra principală
                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this, [this, backgroundWindow]() {
                    backgroundWindow->close();
                    this->show(); // Reafișează fereastra principală
                    });

                // Afișează fereastra intermediară
                backgroundWindow->showFullScreen();

                return;
            }

            if (text == "Power Duel") {
                qDebug() << "Intrat în Power Duel.";

                // Creează o fereastră cu fundal pentru IntermediateMenu
                auto* backgroundWindow = new QWidget();
                backgroundWindow->setWindowTitle("Power Duel Setup");
                backgroundWindow->setAttribute(Qt::WA_DeleteOnClose);
                backgroundWindow->setWindowModality(Qt::ApplicationModal);
                backgroundWindow->setAutoFillBackground(true);

                // Setează fundalul
                QPalette palette = backgroundWindow->palette();
                palette.setBrush(QPalette::Window,
                    QBrush(QPixmap(imagePath).scaled(QGuiApplication::primaryScreen()->size(),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                backgroundWindow->setPalette(palette);

                // Creează layout-ul principal
                QVBoxLayout* mainLayout = new QVBoxLayout(backgroundWindow);
                mainLayout->setAlignment(Qt::AlignCenter);

                // Adaugă IntermediateMenu centrat
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                mainLayout->addWidget(intermediateMenu);

                // Ascunde fereastra principală
                this->hide();

                // Conectează semnalul `startSelected` pentru a începe Mage Duel
                connect(intermediateMenu, &IntermediateMenu::startSelected, this, [this, backgroundWindow](bool illusions, bool explosions, bool timer) {
                    backgroundWindow->close();

                    // Configurează jocul
                    Game& gameInstance = Game::get_Instance();
                    gameInstance.setIllusionsEnabled(illusions);
                    gameInstance.setExplosionsEnabled(explosions);

                    // Lansează metoda pentru Mage Duel
                    gameInstance.startGame(Game::GameType::Power);
                    });

                // Conectează semnalul `goBackSelected` pentru a reveni la fereastra principală
                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this, [this, backgroundWindow]() {
                    backgroundWindow->close();
                    this->show(); // Reafișează fereastra principală
                    });

                // Afișează fereastra intermediară
                backgroundWindow->showFullScreen();

                return;
            }

            if (text == "Mage and Power Duel") {
                qDebug() << "Intrat în Mage and Power Duel.";

                // Creează o fereastră cu fundal pentru IntermediateMenu
                auto* backgroundWindow = new QWidget();
                backgroundWindow->setWindowTitle("Mage and Power Duel Setup");
                backgroundWindow->setAttribute(Qt::WA_DeleteOnClose);
                backgroundWindow->setWindowModality(Qt::ApplicationModal);
                backgroundWindow->setAutoFillBackground(true);

                // Setează fundalul
                QPalette palette = backgroundWindow->palette();
                palette.setBrush(QPalette::Window,
                    QBrush(QPixmap(imagePath).scaled(QGuiApplication::primaryScreen()->size(),
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                backgroundWindow->setPalette(palette);

                // Creează layout-ul principal
                QVBoxLayout* mainLayout = new QVBoxLayout(backgroundWindow);
                mainLayout->setAlignment(Qt::AlignCenter);

                // Adaugă IntermediateMenu centrat
                auto* intermediateMenu = new IntermediateMenu(backgroundWindow);
                mainLayout->addWidget(intermediateMenu);

                // Ascunde fereastra principală
                this->hide();

                // Conectează semnalul `startSelected` pentru a începe Mage Duel
                connect(intermediateMenu, &IntermediateMenu::startSelected, this, [this, backgroundWindow](bool illusions, bool explosions, bool timer) {
                    backgroundWindow->close();

                    // Configurează jocul
                    Game& gameInstance = Game::get_Instance();
                    gameInstance.setIllusionsEnabled(illusions);
                    gameInstance.setExplosionsEnabled(explosions);

                    // Lansează metoda pentru Mage Duel
                    gameInstance.startGame(Game::GameType::MageDuelAndPower);
                    });

                // Conectează semnalul `goBackSelected` pentru a reveni la fereastra principală
                connect(intermediateMenu, &IntermediateMenu::goBackSelected, this, [this, backgroundWindow]() {
                    backgroundWindow->close();
                    this->show(); // Reafișează fereastra principală
                    });

                // Afișează fereastra intermediară
                backgroundWindow->showFullScreen();

                return;
            }

            if (text == "Exit") {
                QApplication::quit();
                return;
            }


            for (auto& child : this->children()) {
                if (QWidget* widget = qobject_cast<QWidget*>(child)) {
                    widget->hide();
                }
            }

            /*IntermediateMenu* menu = new IntermediateMenu(this);
            menu->show();

            connect(menu, &IntermediateMenu::startSelected, [this, text, menu, imagePath](bool illusions, bool explosions, bool timerActive) {
                menu->close();

                this->hide();

                if (secondaryWindows.find(text) == secondaryWindows.end()) {
                    secondaryWindows[text] = new SecondaryWindow(text, imagePath);

                    connect(secondaryWindows[text], &SecondaryWindow::closed, [this]() {
                        for (auto& child : this->children()) {
                            if (QWidget* widget = qobject_cast<QWidget*>(child)) {
                                widget->show();
                            }
                        }
                        this->showFullScreen();
                        });
                }

                secondaryWindows[text]->showFullScreen();
                });

            connect(menu, &IntermediateMenu::goBackSelected, [this, menu]() {
                menu->close();

                for (auto& child : this->children()) {
                    if (QWidget* widget = qobject_cast<QWidget*>(child)) {
                        widget->show();
                    }
                }
                });*/
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
