#include <Shop.h>
#include <Exception.h>
    Shop::Shop() {
        //Initializam cateva iteme
        items["HealthPotion"] = 50;
        items["AttackBoost"] = 100;
        items["DefenseBoost"] = 100;
        items["SpeedBoost"] = 75;
    }
    Shop& Shop::getInstance() {
        if (instance == nullptr) {
            instance = new Shop();
        }
        return *instance;
    }
    void Shop::displayItems() const {
        std::cout<<"\n=== SHOP ===\n";
        for (const auto&[fst, snd] : items) {
            std::cout<<" - " << fst << ": " << snd << " gold\n";
        }
    }
    bool Shop::buyItem(Player& player, const std::string& itemName) {
        const auto it = items.find(itemName);
        if (it == items.end()) {
            throw InventoryException("Item '" + itemName + "' not found in shop!");
        }
        if (int price = it->second; player.spendGold(price)) {
            player.addItem(itemName);
            return true;
        }
        return false;
    }
    void Shop::applyItemEffect(Player& player, const std::string& itemName) {
        if (itemName == "HealthPotion") {
            const int healAmount=static_cast<int>(player.getMaxHealth()*(0.30)); // heals 30%
            player.setHealth(player.getHealth()+healAmount);
            std::cout<<"Restored " << healAmount << " HP!\n";
        }
        else if (itemName == "AttackBoost") {
            player.setAttackPower(player.getAttackPower()+5);
            std::cout<<"Gained 5 ATK!\n";
        }
        else if (itemName == "DefenseBoost") {
            player.setDefense(player.getDefense()+10);
            std::cout<<"Gained 10 DEF!\n";
        }
        else if (itemName == "SpeedBoost") {
            player.setSpeed(player.getSpeed()+3);
            std::cout<<"Gained 3 SPD!\n";
        }
        else {
            std::cout<<"Invalid item name!\n";
        }
        //Eventual alte efecte
    }
Shop* Shop::instance = nullptr;