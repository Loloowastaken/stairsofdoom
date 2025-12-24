#include <Shop.h>
#include <Exception.h>
    Shop::Shop() {
        //Initializam cateva iteme
        items["Health Potion"] = 50;
        items["Attack Boost"] = 100;
        items["Defense Boost"] = 100;
        items["Speed Boost"] = 75;
    }
    Shop& Shop::getInstance() {
        if (instance == nullptr) {
            instance = new Shop();
        }
        return *instance;
    }
    void Shop::displayItems() const {
        std::cout<<"\n=== SHOP ===\n";
        for (const auto& item : items) {
            std::cout<<" - " << item.first << ": " << item.second << " gold\n";
        }
    }
    bool Shop::buyItem(Player& player, const std::string& itemName) {
        auto it = items.find(itemName);
        if (it != items.end()) {
            throw InventoryException("Item '" + itemName + "' not found in shop!");
        }
        int price = it->second;
        if (player.spendGold(price)) {
            player.addItem(itemName);
            applyItemEffect(player,itemName);
            return true;
        }
        return false;
    }
    void Shop::applyItemEffect(Player& player, const std::string& itemName) {
        if (itemName == "Health Potion") {
            player.setHealth(player.getHealth()+50);
            std::cout<<"Restored 50 HP!\n";
        }
        else if (itemName == "Attack Boost") {
            player.setAttackPower(player.getAttackPower()+5);
            std::cout<<"Gained 5 ATK!\n";
        }
        else if (itemName == "Defense Boost") {
            player.setDefense(player.getDefense()+10);
            std::cout<<"Gained 10 DEF!\n";
        }
        else if (itemName == "Speed Boost") {
            player.setSpeed(player.getSpeed()+3);
            std::cout<<"Gained 3 SPD!\n";
        }
        //Eventual alte efecte
    }
Shop* Shop::instance = nullptr;