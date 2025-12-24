#include <algorithm>
#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <vector>

                                                            /// !!!!! CLASA DE BAZA 'CHARACTER' !!!!! ///
class Character {
protected:
    std::string name;
    int level;
    int health;
    int maxHealth;
    int attackPower;
    int defense;
    int speed;
    static int totalCharacters;
public:
                                                    // CONSTRUCTORI //

    // Constructor Default
    Character() : name("Unknown"), level(1), health(100), maxHealth(100), attackPower(10), defense(5), speed(10) {
        totalCharacters++;
    }
    // Constructor parametrizat
    Character(std::string  name, int level, int health,
              int attackPower = 10, int defense = 5, int speed = 10)
        : name(std::move(name)), level(level), health(health), maxHealth(health),
          attackPower(attackPower), defense(defense), speed(speed) {
        totalCharacters++;
    }
    // Constructor supraincarcat
    Character(const std::string& name, int level)
        : Character(name, level, 50+level*10, 5+level*2, 2+level, 5+level) {
        totalCharacters++;
    }
    // Constructor copy
    Character(const Character &other)
        : name(other.name),
          level(other.level),
          health(other.health),
          maxHealth(other.maxHealth),
          attackPower(other.attackPower),
          defense(other.defense),
          speed(other.speed) {
        totalCharacters++;
    }
                                                        // OPERATORI MEMBRI //
    //Operator de copiere
    Character& operator=(const Character &other) {
        if (this != &other) {
            name = other.name;
            level = other.level;
            health = other.health;
            maxHealth = other.maxHealth;
            attackPower = other.attackPower;
            defense = other.defense;
            speed = other.speed;
        }
        return *this;
    }

    //Operator membru +=, folosit pt healing
    Character& operator+=(int healAmount) {
        health+=healAmount;
        if (health>maxHealth) health=maxHealth;
        return *this;
    }

    //Operator membru -=, folosit pt damage
    Character& operator-=(int damage) {
        health-=damage;
        if (health<0) health=0;
        return *this;
    }
                                                            // METODE VIRTUALE //

    //metoda pentru atac
    virtual void attack(Character& target) {
        int damage = calculateDamage(target);
        std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n ";
        target.takeDamage(damage);
    }
    //metoda pentru a lua damage
    virtual void takeDamage(int damage) {
        int actualDamage = damage - defense;
        if (actualDamage<1) actualDamage=1;
        health-=actualDamage;
        if (health<0) health=0;
    }
    //metoda virtuala pura, abilitati folosite si de player, si de inamic
    virtual void specialAbility() = 0; //face ca 'Character' sa fie clasa abstracta

                                                            // DESTRUCTORI //
    virtual ~Character() { //destructor virtual
        std::cout<< name << " destroyed\n";
    }
                                                            // GETTERS/SETTERS //
    [[nodiscard]] const std::string& getName() const { return name; }
    [[nodiscard]] int getLevel() const { return level; }
    [[nodiscard]] int getHealth() const { return health; }
    [[nodiscard]] int getMaxHealth() const { return maxHealth; }
    [[nodiscard]] int getAttackPower() const { return attackPower; }
    [[nodiscard]] int getDefense() const { return defense; }
    [[nodiscard]] int getSpeed() const { return speed; }
    void setName(const std::string &setname) { this->name = setname; }
    void setHealth(int sethealth) {
        this->health = (sethealth>maxHealth) ? maxHealth : sethealth;
        if (health < 0) this->health=0;
    }
                                                            // ALTE METODE //
    void displayStatus() const { //vor fi afisate in timpul unei lupte
        std::cout << name << " [LVL " << level << "] "
                  << "HP: " << health << "/" << maxHealth
                  << " ATK: " << attackPower
                  << " DEF: " << defense
                  << " SPD: " << speed << std::endl;
    }
    [[nodiscard]] bool isAlive() const { return health>0; }
    //Metoda statica
    static int getTotalCharacters() { return totalCharacters; }
protected:
    //Metoda helper pentru a calcula damage
    [[nodiscard]] int calculateDamage(const Character& target) const {
        int damage = attackPower - (target.getDefense()/2);
        return (damage < 1) ? 1 : damage;
    }
};

int Character::totalCharacters = 0;

                                                            // OPERATORI NON-MEMBRI //
//Operator output
std::ostream& operator<<(std::ostream& os, const Character& c) {
    os << c.getName()
       << " [LVL:" << c.getLevel()
       << " HP:" << c.getHealth()
       << " ATK:" << c.getAttackPower()
       << " DEF:" << c.getDefense()
       << " SPD:" << c.getSpeed() << "]";
    return os;
}
//Operator input
std::istream& operator>>(std::istream& is, Character& c) {
    std::string inputName;
    std::cout << "Enter character name: ";
    is >> inputName;
    c.setName(inputName);
    return is;
}
//Operator <, folosit pentru a determina cine face prima miscare, bazata pe speed
bool operator<(const Character& c1, const Character& c2) {
    return c1.getSpeed() < c2.getSpeed();
}
//Operator ==, folosit pentru a verifica daca sunt acelasi nivel/putere
bool operator==(const Character& c1, const Character& c2) {
    return c1.getLevel() == c2.getLevel() && c1.getAttackPower() == c2.getAttackPower();
}
                                                            /// !!!!! CLASA TEMPLATE ITEMCONTAINER !!!!! ///
template<typename T>
class ItemContainer {
private:
    std::vector<T> items;
    static int totalContainers;
    std::string containerName;
public:
                                                            // CONSTRUCTORI //
    explicit ItemContainer(std::string name = "Container")
        : containerName(std::move(name)) {
        totalContainers++;
    }
    //Constructor copy
    ItemContainer(const ItemContainer& other)
        : items(other.items),
        containerName(other.containerName) {
        totalContainers++;
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
    auto begin() const { return items.begin(); }
    auto end() const { return items.end(); }
};

template<typename T>
int ItemContainer<T>::totalContainers = 0;
                                                            /// !!!! CLASA ITEM !!!! ///
class Item {
private:
    std::string name;
    int value;
public:
                                                            // CONSTRUCTOR //
    explicit Item(std::string n = "", int v = 0)
        : name(std::move(n)), value(v) {}
                                                            // OPERATORI //
    //pentru std::find ne trebuie ==
    bool operator==(const Item& other) const {
        return name == other.name && value == other.value;
    }

    friend std::ostream& operator<<(std::ostream& os, const Item& item) {
        os << item.name << " (Value: " << item.value << ")";
        return os;
    }
                                                            // GETTERI //
    [[nodiscard]] int getValue() const { return value; }
    [[nodiscard]] const std::string& getName() const { return name; }
};
                                                            // !!!! CLASE DERIVATE !!!! //
                                                            /// CLASA 'PLAYER' - DERIVATA LUI 'CHARACTER' ///
class Player : public Character {
protected:
    int gold;
    int experience;
    int experienceToNext;
    std::vector<std::string> inventory;
public:
                                                            // CONSTRUCTORI (apeland clasa de baza) //
    //Constructor default
    explicit Player(const std::string &name)
        : Character(name, 1, 100, 15, 8, 12),
          gold(50), experience(0), experienceToNext(100) {
        inventory.emplace_back("Basic Sword");
        inventory.emplace_back("Leather Rags");
    }
    //Constructor supraincarcat
    Player(const std::string &name, int level)
        : Character(name, level, 80+level*20, 12+level*3, 6+level*2, 12+level),
          gold(level*25), experience(0), experienceToNext(level*100) {
        inventory.emplace_back("Basic Sword");
        inventory.emplace_back("Leather Rags");
    }
    //Constructor copy
    Player (const Player& other) = default;
                                                            // OPERATORI //
    // Operator copy
    Player &operator=(const Player& other) {
        if (this != &other) {
            this->Character::operator=(other);
            this->gold = other.gold;
            this->experience = other.experience;
            this->experienceToNext = other.experienceToNext;
            this->inventory = other.inventory;
        }
        return *this;
    }
                                                            // DESTRUCTORI //
    ~Player() override {
        std::cout << "Player" << getName() << "destroyed\n";
    }
                                                            // METODE VIRTUALE SUPRASCRISE //
    void attack(Character &target) override {
        std::cout << "Player " << getName() << " attacks " << target.getName() << "!\n";
        Character::attack(target); // apelarea implementarii de baza

        //specific pentru player: obtinerea experientei
        gainExperience(25);
    }

    void specialAbility() override {
        std::cout << getName() << " uses special ability! Double damage next attack!\n";
        attackPower*=2;
    }
                                                            // METODE SPECIFICE PLAYER //
    void gainExperience(int xp) {
        experience += xp;
        std::cout << "Obtained " << xp << " experience. Total: "
                  << experience << "/" << experienceToNext << std::endl;
        while (experience>=experienceToNext) {
            experience-=experienceToNext;
            levelUp();
            experienceToNext += getLevel()*100;
        }
    }

    void levelUp() {
        level+=1;
        maxHealth+=30;
        health=maxHealth; // heal la levelup
        attackPower+=8;
        defense+=4;
        speed+=2;
        std::cout << getName() << " leveled up to level " << getLevel() << "!\n";
    }

    void addGold(int amount) {
        gold += amount;
        std::cout << "Obtained" << amount << " gold. Total: " << gold << "\n";
    }

    bool spendGold(int amount) {
        if (gold >= amount) {
            gold -= amount;
            std::cout << "Spent " << amount << " gold. Total remaining: " << gold << "\n";
            return true;
        }
        std::cout << "Not enough gold! You need " << amount << ", but you have " << gold << "\n";
        return false;
    }

    void addItem(const std::string& item) {
        inventory.push_back(item);
        std::cout << "Added " << item << " to inventory\n";
    }

    void showInventory() const {
        std::cout << getName() << "'s Inventory" << " (" << inventory.size() << " items):\n";
        for (const auto& item : inventory) {
            std::cout << " - " << item << std::endl;
        }
        std::cout << "Gold: " << gold << "\n";
    }

                                                            // GETTERI //
    [[nodiscard]] int getGold() const { return gold; }
    [[nodiscard]] int getExp() const { return experience; }
    void setGold(int setgold) { gold = setgold; }
    void setExp(int setexp) { experience = setexp; }

                                                            // ALTE METODE //
    //Metoda pentru demonstrarea downcastingului
    void buyFromShop (const std::string& item, int cost) {
        if (spendGold(cost)) {
            addItem(item);
            std::cout << "Bought: " << item << "\n";
        }
    }

};
                                                            /// CLASA 'ENEMY' - DERIVATA LUI 'CHARACTER' ///
class Enemy : public Character {
public:
    enum class EnemyType {GOBLIN, SKELETON, ORC, SLIME}; // Vom avea mai multe feluri de inamici
protected:
    EnemyType type;
    int baseReward;
    std::string description;
public:
                                                            // CONSTRUCTORI //
    //Constructor default/supraincarcat
    Enemy(const std::string& name, EnemyType type, int level)
        : Character(name, level, 50 + level * 15, 8 + level * 2,
                    3 + level, 8 + level),  // Call base constructor
          type(type), baseReward(level * 10) {
        setDescription();
    }

    // Constructor copy
    Enemy(const Enemy& other) = default;

    // Operator copy
    Enemy& operator=(const Enemy& other) {
        if (this != &other) {
            Character::operator=(other);  // Call base assignment
            type = other.type;
            baseReward = other.baseReward;
            description = other.description;
        }
        return *this;
    }

                                                            // DESTRUCTORI //
    ~Enemy() override {
        std::cout << "Enemy " << getName() << " destroyed\n";
    }

                                                            // METODE VIRTUALE SUPRASCRISE //
    void attack(Character& target) override {
        std::cout << getName() << " (";
        switch(type) {
            case EnemyType::GOBLIN: std::cout << "Goblin"; break;
            case EnemyType::SKELETON: std::cout << "Skeleton"; break;
            case EnemyType::ORC: std::cout << "Orc"; break;
            case EnemyType::SLIME: std::cout << "Slime"; break;
        }
        std::cout << ") attacks !\n";

        Character::attack(target);
    }

    void specialAbility() override {
        std::cout << getName() << " used ";
        switch(type) {
            case EnemyType::GOBLIN:
                std::cout << "TRICK! Player defense lowered!\n"; // implementare in main
                break;
            case EnemyType::SKELETON:
                std::cout << "FORTIFY! Enemy defense heightened!\n";
                defense+=5;
                break;
            case EnemyType::ORC:
                std::cout << "RAGE! Increased attack!\n";
                attackPower+=10;
                break;
            case EnemyType::SLIME:
                std::cout << "ACID! Poison damage over time!\n"; // implementare in main
                break;
        }
    }
                                                            // METODE SPECIFICE INAMICULUI //
    [[nodiscard]] int calculateReward() const {
        int reward = baseReward;
        if (type == EnemyType::ORC) reward*=2; // dublu gold de la orci
        if (type == EnemyType::SLIME) reward/=2; // jumate gold de la slime
        return reward;
    }

    [[nodiscard]] bool isBoss() const {
        return getLevel()>=10; // Inamici peste level 10 sunt considerati bossi
    }
                                                            // GETTERI SI SETTERI //
    [[nodiscard]] EnemyType getType() const { return type; }
    [[nodiscard]] const std::string & getDescription() const { return description; }
    void setDescription() {
        switch (type) {
            case EnemyType::GOBLIN:
                description = "Cunning and small creature";
                break;
            case EnemyType::SKELETON:
                description = "Undead soldier";
                break;
            case EnemyType::ORC:
                description = "Barbaric and powerful humanoid";
                break;
            case EnemyType::SLIME:
                description = "Gelatinous fiend";
                break;
        }
    }

};
                                                            /// CLASA 'BOSS' - DERIVATA LUI 'ENEMY' ///
class Boss : public Enemy {
protected:
    std::string title;
    bool hasSpecialPhase; // la un anumit punct, bossul intra in phase 2 pentru a il distinge mai bine de un inamic normal
    int specialPhaseThreshold;
public:
                                                            // CONSTRUCTORI //
    Boss(const std::string& name, Enemy::EnemyType type, int level,
         std::string title)
        : Enemy(name, type, level),  // apeleaza constructorul 'Enemy'
          title(std::move(title)), hasSpecialPhase(false),
          specialPhaseThreshold(getMaxHealth() / 2) {
        // Bossi sunt mai puternici
        maxHealth*=2;
        health=maxHealth;
        attackPower+=10;
        defense+=5;
    }

    // Constructor copy
    Boss(const Boss& other) = default;

    // Operatorul copy
    Boss& operator=(const Boss& other) {
        if (this != &other) {
            Enemy::operator=(other);  // Apeleaza operatorul copy de la Enemy
            title = other.title;
            hasSpecialPhase = other.hasSpecialPhase;
            specialPhaseThreshold = other.specialPhaseThreshold;
        }
        return *this;
    }

                                                            // DESTRUCTOR //
    ~Boss() override {
        std::cout << "Boss " << getName() << " destroyed\n";
    }
                                                            // METODE SUPRASCRISE //
    void takeDamage(int damage) override {
        Character::takeDamage(damage);

        //vf daca suntem in faza speciala
        if (!hasSpecialPhase && getHealth() <= specialPhaseThreshold) {
            enterSpecialPhase();
        }
    }

    void specialAbility() override {
        if (hasSpecialPhase) {
            std::cout << title << " " << getName()
                      << " used ULTIMATE ATTACK! Devastating damage!\n";
        } else {
            Enemy::specialAbility(); // apelam abilitatea speciala normala
        }
    }
                                                            // METODE SPECIFICE BOSSULUI //
    void enterSpecialPhase() {
        hasSpecialPhase = true;
        std::cout << "\n!!! PHASE 2 !!!\n";
        std::cout << title << " " << getName() << " is enraged!\n";
        attackPower*=2;
        speed*=2;
    }
                                                            // GETTERI //
    [[nodiscard]] const std::string& getTitle() const { return title; }
    [[nodiscard]] bool isInSpecialPhase() const { return hasSpecialPhase; }

    void displayBossInfo() const {
        displayStatus();
        std::cout<< " - " << title << "\n";
        if (hasSpecialPhase) {
            std::cout<<" (ENRAGED)\n";
        }
    }
};

int main() {
};