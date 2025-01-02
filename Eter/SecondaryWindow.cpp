#include "SecondaryWindow.h"

SecondaryWindow::SecondaryWindow(const QString& title, const QString& imagePath, QWidget* parent)
    : QWidget(parent), imagePath(imagePath) {
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
