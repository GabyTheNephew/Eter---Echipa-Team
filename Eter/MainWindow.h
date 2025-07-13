#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QMessageBox>
#include <unordered_map>
#include "SecondaryWindow.h"
#include "IntermediateMenu.h"
#include "LoadGameMenu.h"
#include "Game.h"
#include "GameSaver.h"
class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(const QString& imagePath, QWidget* parent = nullptr);

private:
    QString imagePath;
    QString selectedGameMode; 
    std::unordered_map<QString, SecondaryWindow*> secondaryWindows;
    QString readPasswordFromSave(const QString& filename);

protected:
    void resizeEvent(QResizeEvent* event);
};
