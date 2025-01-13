#include "MainWindow.h"

MainWindow::MainWindow(const QString& imagePath, QWidget* parent)
    : QWidget(parent), imagePath(imagePath) {
    setWindowTitle("Main Window");

    this->setAutoFillBackground(true);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QStringList buttonTexts = { "Training", "Mage Duel", "Power Duel", "Tournament", "Exit" };

    for (const QString& text : buttonTexts) {
        QPushButton* button = new QPushButton(text, this);
        button->setFixedSize(200, 50);
        layout->addWidget(button, 0, Qt::AlignCenter);

        connect(button, &QPushButton::clicked, [this, text, imagePath]() {
            if (text == "Exit") {
                QApplication::quit();
                return;
            }

            for (auto& child : this->children()) {
                if (QWidget* widget = qobject_cast<QWidget*>(child)) {
                    widget->hide();
                }
            }

            IntermediateMenu* menu = new IntermediateMenu(this);
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
                });
            });
    }

    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
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
