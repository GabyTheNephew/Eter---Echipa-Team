#include <QApplication>
#include <QDir>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QString imagePath = QDir::currentPath() + QDir::separator() + "eter.png";
    MainWindow mainWindow(imagePath);

    return app.exec();
}
