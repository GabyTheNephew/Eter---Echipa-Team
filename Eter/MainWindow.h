#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <unordered_map>
#include "SecondaryWindow.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(const QString& imagePath, QWidget* parent = nullptr);

private:
    QString imagePath;
    std::unordered_map<QString, SecondaryWindow*> secondaryWindows;
protected:
    void resizeEvent(QResizeEvent* event);
};
