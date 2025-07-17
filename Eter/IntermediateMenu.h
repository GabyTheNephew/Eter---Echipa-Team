#pragma once

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>  
#include <QLineEdit>
#include <QLabel> 
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QApplication>
#include <QMessageBox>
#include <regex>   

class IntermediateMenu : public QWidget {
    Q_OBJECT

public:
    explicit IntermediateMenu(QWidget* parent = nullptr);



private:
#pragma region Validator Methods
    bool validateInput();
    bool isValidEmail(const std::string& email);
    bool isValidPassword(const std::string& password);
#pragma endregion

private:
#pragma region CheckBox 
    QCheckBox* illusionsCheckBox;
    QCheckBox* explosionsCheckBox;
    QCheckBox* timerCheckBox;
    QComboBox* timerComboBox;
#pragma endregion

#pragma region Labels
    QLabel* timerLabel;
    QLabel* emailLabel;
    QLabel* passwordLabel;
#pragma endregion

#pragma region Button
    QPushButton* startButton;
    QPushButton* goBackButton;

#pragma endregion


#pragma region LineEdit
    QLineEdit* emailLineEdit;
    QLineEdit* passwordLineEdit;
#pragma endregion


signals:
    void startSelected(bool illusions, bool explosions, bool timer, int timerDuration, QString email, QString password);
    void goBackSelected();

private slots:
    void onStartClicked();

};
