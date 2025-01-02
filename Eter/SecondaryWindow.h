#pragma once

#include <QWidget>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QApplication>
#include <QPixmap>
#include <QPalette>
#include <QBrush>

class SecondaryWindow : public QWidget {
    Q_OBJECT

public:
    explicit SecondaryWindow(const QString& title, const QString& imagePath, QWidget* parent = nullptr);

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QString imagePath;
};
