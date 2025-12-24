#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
/// !!!!! CLASA TEMPLATE ITEMCONTAINER !!!!! //
template<typename T>
class ItemContainer {
private:
    std::vector<T> items;
    static int totalContainers;
    std::string containerName = "Default Container";
public:
                                                            // CONSTRUCTORI //
    ItemContainer()
    {
        totalContainers++;
    }
    explicit ItemContainer(std::string name)
        : containerName(std::move(name)) {
        totalContainers++;
    }
    //Constructor copy
    ItemContainer(const ItemContainer& other)
        : items(other.items),
        containerName(other.containerName) {
        totalContainers++;
    }
    ItemContainer& operator=(const ItemContainer& other) {
        if (this != &other) {
            containerName = other.containerName;
            items = other.items;
        }
        return *this;
    }
                                                            // DESTRUCTOR //
    ~ItemContainer() {
        totalContainers--;
    }
                                                            // METODE PUBLICE //
    void addItem(const T& item) {
        items.push_back(item);
    }
    bool removeItem(const T& item) {
        auto it = std::find(items.begin(), items.end(), item);
        if (it != items.end()) {
            items.erase(it);
            return true;
        }
        return false;
    }
    [[nodiscard]] size_t getItemCount() const { return items.size(); }
    static int getTotalContainers() { return totalContainers; }

    void display() const {
        std::cout << containerName << " contains " << items.size() << " items\n";
        for (const auto& item : items) {
            std::cout<<" - " << item << "\n";
        }
    }

    // Algoritm STL cu lambda
    template<typename P>
    int countIf(P p) const {
        return std::count_if(items.begin(), items.end(), p);
    }

    // Pentru for loopuri bazate pe range
    auto begin() { return items.begin(); }
    auto end() { return items.end(); }
    [[nodiscard]] auto begin() const { return items.begin(); }
    [[nodiscard]] auto end() const { return items.end(); }
};

template<typename T>
int ItemContainer<T>::totalContainers = 0;