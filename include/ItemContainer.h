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
    int capacity;
    std::string containerName = "Default Container";
public:
                                                            // CONSTRUCTORI //
    ItemContainer()
    {
        capacity = 30;
        totalContainers++;
    }
    explicit ItemContainer(std::string name, const int capacity)
        : capacity(capacity), containerName(std::move(name)) {
        totalContainers++;
    }
    //Constructor copy
    ItemContainer(const ItemContainer& other)
        : items(other.items),
        capacity(other.capacity), containerName(other.containerName){
        totalContainers++;
    }
    ItemContainer& operator=(const ItemContainer& other) {
        if (this != &other) {
            containerName = other.containerName;
            items = other.items;
            capacity = other.capacity;
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
    [[nodiscard]] int getCapacity() const { return capacity; }
    static int getTotalContainers() { return totalContainers; }

    void display() const {
        std::cout << containerName << " contains " << items.size() << " items\n";
        for (const auto& item : items) {
            std::cout<<" - " << item << "\n";
        }
    }

    // Algoritm STL cu lambda
    template<typename P>
    int countIf(P p) const { // p for predicate
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