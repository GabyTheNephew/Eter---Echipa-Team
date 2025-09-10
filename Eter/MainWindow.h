#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QMessageBox>
#include <unordered_map>
#include "IntermediateMenu.h"
#include "LoadGameMenu.h"
#include "Game.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
class SecondaryWindow;
class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(const QString& imagePath, QWidget* parent = nullptr);

private:
    QString imagePath;
    QString selectedGameMode; 
    std::unordered_map<QString, SecondaryWindow*> secondaryWindows;

    QLineEdit* nameInput;
    QPushButton* doneButton;
    QLabel* nameLabel;

    bool containsObsceneWord(const QString& text);
protected:
    void resizeEvent(QResizeEvent* event);
};
