#pragma once
#include <Player.h>
#include <ItemContainer.h>
#include <map>
                                                            /// !!!!! DESIGN PATTERNS !!!!! ///
                                                            /// SINGLETON - SHOP ///
class Shop {
private:
    static Shop* instance;
    //folosesc ItemContainer pentru iteme ca sa am doua instantieri
    ItemContainer<std::string> availableItems;
    // item name -> pret
    std::map<std::string, int> itemPrices;
    // vom avea o cantitate limitata pentru unele iteme
    ItemContainer<std::pair<std::string, int>> itemStock; // item + cantitate
    //Constructor private pentru Singleton
    Shop();
public:
    // copy constructor si atribuire delete
    Shop(const Shop& other) = delete;
    Shop& operator=(const Shop&) = delete;
    // get singleton instance
    static Shop& getInstance();
    void displayItems() const;
    void buyItem(Player& player, const std::string& itemName);
    static void applyItemEffect(Player& player, const std::string& itemName);
    void addLimitedItem(const std::string& itemName, int price, int quantity);
    [[nodiscard]] int getItemCount() const { return static_cast<int>(availableItems.getItemCount()); }
};