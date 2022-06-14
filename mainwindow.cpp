#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPushButton>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->textEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(curCursorPositionChanged()));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this, SLOT(listItemClicked(QListWidgetItem*)));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
}

MainWindow::~MainWindow() {
    delete ui;
}

std::string MainWindow::getText() {
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    std::string word = cursor.selectedText().toStdString();
    if (word == "" && !ui->textEdit->document()->isEmpty()) {
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::PreviousWord);
        cursor.select(QTextCursor::WordUnderCursor);
        word = cursor.selectedText().toStdString();
        predictionTrie.insert(word);
    }
    return word;
}

void MainWindow::curCursorPositionChanged() {
    std::string word = getText();
    std::vector<std::string> matches = predictionTrie.findBestMatches(word, 20);
    ui->listWidget->clear();
    ui->listWidget->addItem(QString::fromStdString(word));
    for (auto&& word: matches) {
        ui->listWidget->addItem(QString::fromStdString(word));
    }
}


void MainWindow::listItemClicked(QListWidgetItem* item) {
    std::string word = item->text().toStdString();
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    cursor.removeSelectedText();
    ui->textEdit->insertPlainText(QString::fromStdString(word));
    predictionTrie.insert(word);
}

void MainWindow::pushButtonClicked() {
    std::string word = ui->listWidget->currentItem()->text().toStdString();
    predictionTrie.remove(word);
    curCursorPositionChanged();
}

