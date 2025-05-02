#include "art1_algorithm.h"
#include <algorithm>

ART1Algorithm::ART1Algorithm(QObject *parent)
    : QObject(parent)
    , threshold_(0.5)
{
}

void ART1Algorithm::setThreshold(double threshold)
{
    threshold_ = threshold;
}

void ART1Algorithm::setData(const std::vector<std::vector<int>>& data)
{
    data_ = data;
}

void ART1Algorithm::cluster()
{
    clusters_.clear();
    
    for (const auto& item : data_) {
        bool matched = false;
        
        for (auto& cluster : clusters_) {
            const auto& base = cluster[0];
            double similarity = calculateSimilarity(base, item);
            
            if (similarity >= threshold_) {
                cluster.push_back(item);
                matched = true;
                break;
            }
        }
        
        if (!matched) {
            clusters_.push_back({item});
        }
    }
}

std::vector<std::vector<std::vector<int>>> ART1Algorithm::getClusters() const
{
    return clusters_;
}

std::vector<int> ART1Algorithm::suggestItems(const std::vector<int>& userVector) const
{
    for (const auto& cluster : clusters_) {
        for (const auto& person : cluster) {
            if (person == userVector) {
                std::vector<int> jointVector = person;
                for (const auto& other : cluster) {
                    for (size_t i = 0; i < jointVector.size(); ++i) {
                        jointVector[i] = jointVector[i] | other[i];
                    }
                }
                
                std::vector<int> suggestions;
                for (size_t i = 0; i < jointVector.size(); ++i) {
                    if (jointVector[i] == 1 && userVector[i] == 0) {
                        suggestions.push_back(i);
                    }
                }
                return suggestions;
            }
        }
    }
    return {};
}

double ART1Algorithm::calculateSimilarity(const std::vector<int>& base, const std::vector<int>& item) const
{
    int overlap = 0;
    int itemSum = 0;
    
    for (size_t i = 0; i < base.size(); ++i) {
        overlap += base[i] & item[i];
        itemSum += item[i];
    }
    
    return itemSum > 0 ? static_cast<double>(overlap) / itemSum : 0.0;
} 