#include "SecondaryWindow.h"


SecondaryWindow::SecondaryWindow(const QString& title, const QString& imagePath, QWidget* parent)
    : QWidget(parent), imagePath(imagePath){
    setWindowTitle(title);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::gray);
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
    if (event->key() == Qt::Key_Escape) {
        MenuWindow* menu = new MenuWindow(this);
        menu->show();

        connect(menu, &MenuWindow::goToHome, this, [this, menu]() {
            menu->close();
            this->close();
            emit closed(); 
            });

        connect(menu, &MenuWindow::exitApp, []() {
            QApplication::quit();
            });
    }
    else {
        QWidget::keyPressEvent(event);
    }
}

