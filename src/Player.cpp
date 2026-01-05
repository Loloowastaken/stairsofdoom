#include <limits>
#include <Player.h>
#include <Exception.h>
     Player::Player(const std::string &name)
        : Character(name, 1, 100, 15, 8, 12),
          gold(50), experience(0), experienceToNext(100), inventory("Player Inventory", 30),
          heroicStrikeCooldown(0), shieldBashCooldown(0) {
    }
    Player::Player(const std::string &name, const int level)
        : Character(name, level, 80+level*20, 12+level*3, 6+level*2, 12+level),
          gold(level*25), experience(0), experienceToNext(level*100),
          heroicStrikeCooldown(0), shieldBashCooldown(0) {
    }
    Player& Player::operator=(const Player& other) {
        if (this != &other) {
            this->Character::operator=(other);
            this->gold = other.gold;
            this->experience = other.experience;
            this->experienceToNext = other.experienceToNext;
            this->inventory = other.inventory;
            // cooldowns and initial usage will always be the same
        }
        return *this;
    }
    Player::~Player() {
        std::cout << "Player " << getName() << " destroyed\n";
    }
    void Player::attack(Character &target) {
        int damage=calculateDamage(target);
        //aici aplicam abilitatile
        //tocmai a fost folosit heroic strike? -> cooldown=3, la fel pt shield bash
        if (heroicStrikeCooldown == 3) {
            damage*=2;
            std::cout << "HEROIC STRIKE activated! ";
        }
        if (shieldBashCooldown == 4) {
            damage=static_cast<int>(damage*1.5);
            target.setAttackPower(target.getAttackPower()-10);
            target.setDefense(target.getDefense()-10);
            target.setSpeed(target.getSpeed()-5);
            std::cout << "SHIELD BASH activated!\n";
        }
        std::cout << "Player " << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        target.takeDamage(damage);
        //specific pentru player: obtinerea experientei
        gainExperience(25);
    }

    void Player::specialAbility() {
        std::cout << getName() << "\nChoose special ability:\n";
        std::cout << "1. Heroic Strike (2x damage, 3 turn cooldown)\n";
        std::cout << "2. Shield Bash (1.5x damage + enemy stats lowered, 4 turn cooldown)\n";
        std::cout << "3. Second Wind (heal, x2 uses per battle)\n";
        std::cout << "4. Exit\n";
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1: if (heroicStrike()) break; return;
            case 2: if (shieldBash()) break; return;
            case 3: secondWind(); break;
            case 4: break;
            default: std::cout<<"Invalid choice!\n"; std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); break;
        }
        if (heroicStrikeCooldown > 0) heroicStrikeCooldown--;
        if (shieldBashCooldown > 0) shieldBashCooldown--;

    }
                                                            // METODE SPECIFICE PLAYER //
    void Player::gainExperience(const int xp) {
        experience += xp;
        std::cout << "Obtained " << xp << " experience. Total: "
                  << experience << "/" << experienceToNext << std::endl;
        while (experience>=experienceToNext) {
            experience-=experienceToNext;
            levelUp();
            experienceToNext += getLevel()*100;
        }
    }

    void Player::levelUp() {
        level+=1;
        maxHealth+=30;
        health=maxHealth; // heal la levelup
        attackPower+=8;
        defense+=4;
        speed+=2;
        std::cout << getName() << " leveled up to level " << getLevel() << "!\n";
    }

    void Player::addGold(const int amount) {
        gold += amount;
        std::cout << "Obtained" << amount << " gold. Total: " << gold << "\n";
    }

    bool Player::spendGold(const int amount) {
        if (gold < amount) {
            std::cout<<"Not enough gold. Need " << amount << " gold, but only have " << gold << ".\n";
        }
        gold-=amount;
        return true;
    }

    bool Player::addItem(const std::string& item) {
        if (static_cast<int>(inventory.getItemCount()) >= inventory.getCapacity()) {std::cout<<"Item not found: " << item << "\n"; return false;}
        inventory.addItem(item);
        std::cout << "Added " << item << " to inventory\n";
        return true;
    }
    bool Player::removeItem(const std::string& item) {
         if (!inventory.removeItem(item)) {
             std::cout<<"Item not found: " << item << "\n";
             return false;
         }
         std::cout<<"Removed item " << item << " from inventory.";
         return true;
     }
bool Player::hasItem(const std::string &itemName) const {
    for (const auto& item:inventory) {
        if (item == itemName) {
            return true;
        }
    }
         return false;
}

void Player::showInventory() const {
        inventory.display();
        std::cout << "Gold: " << gold << "\n";
    }
    bool Player::heroicStrike() {
        if (heroicStrikeCooldown > 0) {
            std::cout<<"Heroic Strike is on cooldown for " << heroicStrikeCooldown << " more turns!\n";
            return false;
        }
        heroicStrikeCooldown=3;
        return true;
    }
    bool Player::shieldBash() {
        if (shieldBashCooldown > 0) {
            std::cout<<"Shield Bash is on cooldown for " << shieldBashCooldown << "more turns!\n";
            return false;
        }
        shieldBashCooldown=4;
        return true;
    }
    void Player::secondWind() {
        static int usesRemaining = 2;
        if (usesRemaining <= 0) {
            std::cout << "You're too exhausted to use Second Wind again!\n";
            return;
        }
        std::cout<<name<<" uses SECOND WIND!\n";
        const int healAmount = static_cast<int>(maxHealth*(0.3));
        health+=healAmount;
        if (health > maxHealth) health = maxHealth;
        std::cout<<"Healed for " << healAmount << " HP! (Now: " << health << "/" << maxHealth << ")\n";
        usesRemaining--;
    }