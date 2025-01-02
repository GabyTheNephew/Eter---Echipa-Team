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

            if (secondaryWindows.find(text) == secondaryWindows.end()) {
                secondaryWindows[text] = new SecondaryWindow(text, imagePath);
                connect(secondaryWindows[text], &SecondaryWindow::closed, [this]() {
                    this->showFullScreen();
                    });
            }

            this->hide();
            secondaryWindows[text]->showFullScreen();
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
