#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QScreen> 
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QScrollArea>
#include <QDir>

class LoadGameMenu : public QWidget {
    Q_OBJECT

public:
    explicit LoadGameMenu(QWidget* parent = nullptr);

signals:
    void goBackSelected();
    void saveFileSelected(const QString& filename);

private:
    struct SaveInfo {
        QString filename;
        QString email;
        QString timestamp;
        QString gameType;
        int roundCounter;
        int player1Score;
        int player2Score;
        bool isValid;
    };
    void loadAvailableSaves();
    SaveInfo parseSaveFile(const QString& filePath);
    QPushButton* createSaveButton(const SaveInfo& info);

private:
    QLabel* savesLabel;
    QPushButton* goBackButton;
	QWidget* savesButtonWidget;
    QVBoxLayout* savesButtonLayout;
};
