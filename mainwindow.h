#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "predictiontrie.h"

#include <QMainWindow>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void curCursorPositionChanged();

    void listItemClicked(QListWidgetItem *item);
    void pushButtonClicked();

private:
    std::string getText();

    Ui::MainWindow *ui;
    PredictionTrie predictionTrie;
};
#endif // MAINWINDOW_H
