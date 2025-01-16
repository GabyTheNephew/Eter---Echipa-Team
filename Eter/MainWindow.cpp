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
                // Instanțiem jocul folosind metoda de Training din Game
                qDebug() << "Intrat in if de Training\n";
                Game& gameInstance = Game::get_Instance();
                gameInstance.startGame(Game::GameType::Training);
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
