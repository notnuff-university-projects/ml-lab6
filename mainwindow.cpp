#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QStringList>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , art1Algorithm(new ART1Algorithm(this))
{
    setupUI();
    initializeData();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Threshold input
    QHBoxLayout *thresholdLayout = new QHBoxLayout();
    thresholdLabel = new QLabel("Поріг схожості (0-1):", this);
    thresholdEdit = new QLineEdit("0.5", this);
    thresholdLayout->addWidget(thresholdLabel);
    thresholdLayout->addWidget(thresholdEdit);
    mainLayout->addLayout(thresholdLayout);
    
    // Input field
    QLabel *inputLabel = new QLabel("Введіть ваші музичні вподобання (0 або 1 через пробіл):", this);
    inputEdit = new QLineEdit(this);
    mainLayout->addWidget(inputLabel);
    mainLayout->addWidget(inputEdit);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    calculateButton = new QPushButton("Кластеризувати", this);
    suggestButton = new QPushButton("Отримати рекомендації", this);
    buttonLayout->addWidget(calculateButton);
    buttonLayout->addWidget(suggestButton);
    mainLayout->addLayout(buttonLayout);
    
    // Output field
    outputEdit = new QTextEdit(this);
    outputEdit->setReadOnly(true);
    mainLayout->addWidget(outputEdit);
    
    // Connect signals
    connect(calculateButton, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
    connect(suggestButton, &QPushButton::clicked, this, &MainWindow::onSuggestClicked);
    
    setWindowTitle("Кластеризація музичних вподобань (ART1)");
    resize(600, 400);
}

void MainWindow::initializeData()
{
    // Приклад даних: кожен рядок представляє користувача, де 1 - пісня прослухана, 0 - ні
    musicData = {
        {1, 0, 1, 0, 1, 0}, // Користувач 1
        {1, 1, 1, 0, 1, 0}, // Користувач 2
        {0, 1, 0, 1, 0, 1}, // Користувач 3
        {0, 1, 1, 1, 0, 1}, // Користувач 4
        {1, 1, 0, 0, 1, 0}  // Користувач 5
    };
    
    art1Algorithm->setData(musicData);
}

void MainWindow::onCalculateClicked()
{
    bool ok;
    double threshold = thresholdEdit->text().toDouble(&ok);
    
    if (!ok || threshold < 0 || threshold > 1) {
        QMessageBox::warning(this, "Помилка", "Будь ласка, введіть коректне значення порогу (0-1)");
        return;
    }
    
    art1Algorithm->setThreshold(threshold);
    art1Algorithm->cluster();
    
    auto clusters = art1Algorithm->getClusters();
    QString output = "Знайдено кластерів: " + QString::number(clusters.size()) + "\n\n";
    
    for (size_t i = 0; i < clusters.size(); ++i) {
        output += "Кластер " + QString::number(i + 1) + ":\n";
        for (const auto& userVector : clusters[i]) {
            output += "Користувач: [";
            for (size_t j = 0; j < userVector.size(); ++j) {
                output += QString::number(userVector[j]);
                if (j < userVector.size() - 1) output += ", ";
            }
            output += "]\n";
        }
        output += "\n";
    }
    
    outputEdit->setText(output);
}

void MainWindow::onSuggestClicked()
{
    QString input = inputEdit->text();
    QStringList numbers = input.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    
    std::vector<int> userVector;
    bool valid = true;
    
    for (const QString& num : numbers) {
        bool ok;
        int value = num.toInt(&ok);
        if (!ok || (value != 0 && value != 1)) {
            valid = false;
            break;
        }
        userVector.push_back(value);
    }
    
    if (!valid || userVector.size() != musicData[0].size()) {
        QMessageBox::warning(this, "Помилка", 
            "Будь ласка, введіть коректний вектор вподобань (0 та 1 через пробіл).\n"
            "Довжина вектора повинна бути " + QString::number(musicData[0].size()));
        return;
    }
    
    auto suggestions = art1Algorithm->suggestItems(userVector);
    
    if (suggestions.empty()) {
        outputEdit->setText("Немає рекомендацій для даного користувача.");
    } else {
        QString output = "Рекомендовані пісні (індекси): ";
        for (size_t i = 0; i < suggestions.size(); ++i) {
            output += QString::number(suggestions[i]);
            if (i < suggestions.size() - 1) output += ", ";
        }
        outputEdit->setText(output);
    }
} 