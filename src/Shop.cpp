#include <Shop.h>
#include <Exception.h>
#include <iostream>
    Shop::Shop()
        : availableItems("Shop Inventory", 100),
          itemStock("Limited Stock", 20) {
        availableItems.addItem("HealthPotion");
        availableItems.addItem("AttackBoost");
        availableItems.addItem("DefenseBoost");
        availableItems.addItem("SpeedBoost");

        itemPrices["HealthPotion"] = 50;
        itemPrices["AttackBoost"] = 100;
        itemPrices["DefenseBoost"] = 100;
        itemPrices["SpeedBoost"] = 75;

        itemStock.addItem({"FullHeal",3}); // limited amount
        itemPrices["FullHeal"] = 200;
    }
    Shop& Shop::getInstance() {
        if (instance == nullptr) {
            instance = new Shop();
        }
        return *instance;
    }
    void Shop::displayItems() const {
        std::cout<<"\n=== SHOP ===\n";
        for (const auto& itemName : availableItems) {
            if (auto it = itemPrices.find(itemName); it != itemPrices.end()) {
                std::cout<< " - " << itemName << ": " << it->second << " gold\n";
            }
        }
        //check for limited stock (fullheals)
        if (itemStock.getItemCount() > 0) {
            std::cout << "\n=== LIMITED STOCK ===\n";
            for (const auto& [itemName, quantity] : itemStock) {
                if (auto priceIt = itemPrices.find(itemName); priceIt != itemPrices.end()) {
                    std::cout << " - " << itemName << ": " << priceIt->second;
                    if (quantity>0) std::cout<<" (Only " << quantity << " left!)\n";
                    else std::cout<< " (OUT OF STOCK)\n";
                }
            }
        }
        std::cout << "================\n";
    }
void Shop::buyItem(Player& player, const std::string& itemName) {
        bool isPurchased = false;
        const auto it = itemPrices.find(itemName);
        if (it == itemPrices.end()) {
            throw ItemNotFoundException(itemName);
        }
        const int price = it->second;
        //check if fullheal
        for (auto& [limitedItem, quantity] : itemStock) {
            if (limitedItem == itemName) {
                if (quantity <= 0) {
                    throw InventoryException(itemName + " is out of stock!");
                }
                player.spendGold(price);
                player.addItem(itemName);
                quantity--;  // decrease stock
                isPurchased=true;

                if (quantity<=0) itemStock.removeItem({itemName,0});
            }
        }
        if (!isPurchased) {
            player.spendGold(price);
            player.addItem(itemName);
            std::cout << "Purchased " << itemName << " for " << price << " gold!\n";
        }
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
        else if (itemName == "FullHeal") {
            player.setHealth(player.getMaxHealth());
            std::cout<<"Healed to full HP!\n";
        }
    }
    void Shop::addLimitedItem(const std::string& itemName, const int price, int quantity) {
        itemStock.addItem({itemName, quantity});
        itemPrices[itemName] = price;
    }
Shop* Shop::instance = nullptr;