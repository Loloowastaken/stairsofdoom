#pragma once
#include <Player.h>
#include <map>
                                                            /// !!!!! DESIGN PATTERNS !!!!! ///
                                                            /// SINGLETON - SHOP ///
class Shop {
private:
    static Shop* instance;
    std::map<std::string, int> items; // item name -> pret
    //Constructor private pentru Singleton
    Shop();
public:
    // copy constructor si atribuire delete
    Shop(const Shop& other) = delete;
    Shop& operator=(const Shop&) = delete;
    // get singleton instance
    static Shop& getInstance();
    void displayItems() const;
    bool buyItem(Player& player, const std::string& itemName);
    static void applyItemEffect(Player& player, const std::string& itemName);
};