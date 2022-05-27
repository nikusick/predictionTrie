#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPushButton>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

std::string MainWindow::getText() {
    std::string str = ui->textEdit->toPlainText().toStdString();
    std::string result = "";
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    return cursor.selectedText().toStdString();
}

void MainWindow::on_textEdit_cursorPositionChanged() {
    std::string word = getText();
    std::vector<std::string> matches = predictionTrie.findBestMatches(word, 10);
    ui->listWidget->clear();
    ui->listWidget->addItem(QString::fromStdString(word));
    for (auto&& word: matches) {
        ui->listWidget->addItem(QString::fromStdString(word));
    }
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item) {
    std::string word = item->text().toStdString();
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    cursor.removeSelectedText();
    ui->textEdit->insertPlainText(QString::fromStdString(word));
    if (predictionTrie.allWordsStartedWith(word).empty()) {
        predictionTrie.insert(word);
    }
}








