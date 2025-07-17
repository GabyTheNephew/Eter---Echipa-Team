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
#pragma region Main Methods
    void loadAvailableSaves();
    SaveInfo parseSaveFile(const QString& filePath);
    QPushButton* createSaveButton(const SaveInfo& info);
#pragma endregion

    

private:
    QLabel* savesLabel;
    QPushButton* goBackButton;
	QWidget* savesButtonWidget;
    QVBoxLayout* savesButtonLayout;


signals:
    void goBackSelected();
    void saveFileSelected(const QString& filename);

};
