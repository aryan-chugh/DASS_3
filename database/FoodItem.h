#ifndef FOODITEM_H
#define FOODITEM_H

#include <string>
#include <vector>

class FoodItem {
public:
    FoodItem(const std::string &id, const std::vector<std::string> &keywords);
    virtual ~FoodItem() {}
    std::string getId() const;
    std::vector<std::string> getKeywords() const;
    virtual double getCalories() const = 0;
protected:
    std::string id;
    std::vector<std::string> keywords;
};

#endif // FOODITEM_H
