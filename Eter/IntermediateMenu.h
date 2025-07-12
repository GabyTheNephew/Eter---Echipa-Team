#pragma once

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
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

signals:
    void startSelected(bool illusions, bool explosions, bool timer, QString email, QString password);
    void goBackSelected();

private slots:
    void onStartClicked();

private:
    bool validateInput();   
    bool isValidEmail(const std::string& email);
    bool isValidPassword(const std::string& password);

private:
    QCheckBox* illusionsCheckBox;
    QCheckBox* explosionsCheckBox;
    QCheckBox* timerCheckBox;
    QPushButton* startButton;
    QPushButton* goBackButton;

    QLabel* emailLabel;
    QLineEdit* emailLineEdit;
    QLabel* passwordLabel;
    QLineEdit* passwordLineEdit;

};
