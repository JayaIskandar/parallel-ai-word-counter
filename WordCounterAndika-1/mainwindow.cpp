#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "WordCounter.h" // Include your WordCounter header

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->plainTextEdit->toPlainText(); // Get text from the GUI
    string textStdString = text.toStdString(); // Convert QString to std::string
    vector<pair<string, int>> intermediate = Map(textStdString); // Use your Map function
    unordered_map<string, int> result = Reduce({intermediate}); // Use your Reduce function

    // Display result in GUI or do whatever you want with it
    // For example, you can output it to another text box
    QString resultText;
    for (const auto &entry : result) {
        resultText += QString::fromStdString(entry.first) + " : " + QString::number(entry.second) + "\n";
    }
    ui->resultPlainTextEdit->setPlainText(resultText);
}
