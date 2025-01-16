#include "SecondaryWindow.h"


SecondaryWindow::SecondaryWindow(const QString& title, const QString& imagePath, QWidget* parent)
    : QWidget(parent), imagePath(imagePath) {
    setWindowTitle(title);

    // Create and store the layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);  // marje mai mari
    mainLayout->setSpacing(20);  // spacing mai mare

    // Set the background
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::transparent);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    this->showFullScreen();
}


void SecondaryWindow::closeEvent(QCloseEvent* event) {
    emit closed();
    QWidget::closeEvent(event);
}

void SecondaryWindow::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    QPixmap backgroundPixmap(imagePath);
    if (!backgroundPixmap.isNull()) {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, QBrush(backgroundPixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        this->setPalette(palette);
    }
}

void SecondaryWindow::keyPressEvent(QKeyEvent* event) {
    qDebug() << "Key pressed, menu visible:" << (menu && menu->isVisible());

    if (event->key() == Qt::Key_Escape) {
        if (!menu) {
            menu = new MenuWindow(this);

            connect(menu, &MenuWindow::goToHome, this, [this]() {
                menu->hide();
                this->close();
                emit closed();
                });

            connect(menu, &MenuWindow::exitApp, []() {
                QApplication::quit();
                });

            menu->hide(); 
        }

        if (menu->isVisible()) {
            menu->hide();
        }
        else {
            menu->show();
            menu->raise();
            menu->activateWindow();
        }
    }
    else {
        QWidget::keyPressEvent(event);
    }
}

void SecondaryWindow::setBoard(Board& board) {
    // Create BoardView with the provided board
    m_boardView = new BoardView(board, this);

    // Set fixed size for the grid
    m_boardView->setFixedSize(400, 400);  // dimensiune fixă mai mare

    // Setăm un background pentru BoardView să fim siguri că e vizibil
    m_boardView->setStyleSheet("background-color: rgba(255, 255, 255, 30);");

    // Add to layout using mainLayout
    mainLayout->addWidget(m_boardView, 0, Qt::AlignCenter);

    // Forțăm layout-ul să își actualizeze geometria
    mainLayout->activate();

    // Force update
    m_boardView->updateView();
}


