#ifndef ART1_ALGORITHM_H
#define ART1_ALGORITHM_H

#include <vector>
#include <QObject>

class ART1Algorithm : public QObject
{
    Q_OBJECT

public:
    explicit ART1Algorithm(QObject *parent = nullptr);
    
    void setThreshold(double threshold);
    void setData(const std::vector<std::vector<int>>& data);
    void cluster();
    std::vector<std::vector<std::vector<int>>> getClusters() const;
    std::vector<int> suggestItems(const std::vector<int>& userVector) const;

private:
    double threshold_;
    std::vector<std::vector<int>> data_;
    std::vector<std::vector<std::vector<int>>> clusters_;
    
    double calculateSimilarity(const std::vector<int>& base, const std::vector<int>& item) const;
};

#endif // ART1_ALGORITHM_H