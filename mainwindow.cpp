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
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(listWidgetClicked(QListWidgetItem*)));
}

MainWindow::~MainWindow() {
    delete ui;
}

std::string MainWindow::getText() {
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    return cursor.selectedText().toStdString();
}

void MainWindow::onTextEditCursorPositionChanged() {
    std::string word = getText();
    std::vector<std::string> matches = predictionTrie.findBestMatches(word, 20);
    ui->listWidget->clear();
    ui->listWidget->addItem(QString::fromStdString(word));
    for (auto&& word: matches) {
        ui->listWidget->addItem(QString::fromStdString(word));
    }
}


void MainWindow::onListWidgetItemClicked(QListWidgetItem* item) {
    std::string word = item->text().toStdString();
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    cursor.removeSelectedText();
    ui->textEdit->insertPlainText(QString::fromStdString(word));
    predictionTrie.insert(word);
}



