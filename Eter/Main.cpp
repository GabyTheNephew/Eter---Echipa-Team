#include <QApplication>
#include <QDir>
#include <QTimer>
#include <QEventLoop>
#include "MainWindow.h"
#include "Game.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    qDebug() << "Application starting...";

    // Keep default behavior - let Qt handle window management
    app.setQuitOnLastWindowClosed(true);

    // Remove the problematic lastWindowClosed connection
    // Only keep aboutToQuit for cleanup
    QObject::connect(&app, &QApplication::aboutToQuit, []() {
        qDebug() << "=== APPLICATION ABOUT TO QUIT - CLEANING UP ===";

        // Cleanup will be handled by destructors
        try {
            qDebug() << "Game cleanup completed";
        }
        catch (...) {
            qDebug() << "Exception during cleanup - continuing...";
        }
        });

    QString imagePath = QDir::currentPath() + QDir::separator() + "eter.png";

    try {
        MainWindow* mainWindow = new MainWindow(imagePath);
        mainWindow->setAttribute(Qt::WA_DeleteOnClose);
        mainWindow->setObjectName("MainWindow");
        qDebug() << "MainWindow created successfully";

        int result = app.exec();

        qDebug() << "Application exec finished with result:" << result;
        return result;

    }
    catch (const std::exception& e) {
        qDebug() << "Exception in main:" << e.what();
        return -1;
    }
    catch (...) {
        qDebug() << "Unknown exception in main";
        return -1;
    }
}