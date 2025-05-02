#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include "art1_algorithm.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCalculateClicked();
    void onSuggestClicked();

private:
    void setupUI();
    void initializeData();

    QLineEdit *inputEdit;
    QTextEdit *outputEdit;
    QPushButton *calculateButton;
    QPushButton *suggestButton;
    QLabel *thresholdLabel;
    QLineEdit *thresholdEdit;

    ART1Algorithm *art1Algorithm;
    std::vector<std::vector<int>> musicData;
};

#endif // MAINWINDOW_H