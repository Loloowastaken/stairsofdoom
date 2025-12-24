
/*                                                                                                                   ,----..       ,----..             ____
  .--.--.       ___                                                                                   ,---,       /   /   \     /   /   \          ,'  , `.
 /  /    '.   ,--.'|_                ,--,                                           .--.,           .'  .' `\    /   .     :   /   .     :      ,-+-,.' _ |
|  :  /`. /   |  | :,'             ,--.'|    __  ,-.                       ,---.  ,--.'  \        ,---.'     \  .   /   ;.  \ .   /   ;.  \  ,-+-. ;   , ||
;  |  |--`    :  : ' :             |  |,   ,' ,'/ /|  .--.--.             '   ,'\ |  | /\/        |   |  .`\  |.   ;   /  ` ;.   ;   /  ` ; ,--.'|'   |  ;|
|  :  ;_    .;__,'  /    ,--.--.   `--'_   '  | |' | /  /    '           /   /   |:  : :          :   : |  '  |;   |  ; \ ; |;   |  ; \ ; ||   |  ,', |  ':
 \  \    `. |  |   |    /       \  ,' ,'|  |  |   ,'|  :  /`./          .   ; ,. ::  | |-,        |   ' '  ;  :|   :  | ; | '|   :  | ; | '|   | /  | |  ||
  `----.   \:__,'| :   .--.  .-. | '  | |  '  :  /  |  :  ;_            '   | |: :|  : :/|        '   | ;  .  |.   |  ' ' ' :.   |  ' ' ' :'   | :  | :  |,
  __ \  \  |  '  : |__  \__\/: . . |  | :  |  | '    \  \    `.         '   | .; :|  |  .'        |   | :  |  ''   ;  \; /  |'   ;  \; /  |;   . |  ; |--'
 /  /`--'  /  |  | '.'| ," .--.; | '  : |__;  : |     `----.   \        |   :    |'  : '          '   : | /  ;  \   \  ',  /  \   \  ',  / |   : |  | ,
'--'.     /   ;  :    ;/  /  ,.  | |  | '.'|  , ;    /  /`--'  /         \   \  / |  | |          |   | '` ,/    ;   :    /    ;   :    /  |   : '  |/
  `--'---'    |  ,   /;  :   .'   \;  :    ;---'    '--'.     /           `----'  |  : \          ;   :  .'       \   \ .'      \   \ .'   ;   | |`-'
               ---`-' |  ,     .-./|  ,   /           `--'---'                    |  |,'          |   ,.'          `---`         `---`     |   ;/
                       `--`---'     ---`-'                                        `--'            '---'                                    '---'
*/

#include <algorithm>
#include <iostream>
#include <map>
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
    void setMaxHealth (int setmaxhealth) { this->maxHealth = setmaxhealth; }
    void setAttackPower(int setattackpower) { this->attackPower = setattackpower; }
    void setDefense(int setdefense) { this->defense = setdefense; }
    void setSpeed(int setspeed) { this->speed = setspeed; }
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
    ItemContainer<std::string> inventory;
    int heroicStrikeCooldown;
    int shieldBashCooldown;
public:
                                                            // CONSTRUCTORI (apeland clasa de baza) //
    //Constructor default
    explicit Player(const std::string &name)
        : Character(name, 1, 100, 15, 8, 12),
          gold(50), experience(0), experienceToNext(100), inventory("Player Inventory"),
          heroicStrikeCooldown(0), shieldBashCooldown(0) {
    }
    //Constructor supraincarcat
    Player(const std::string &name, int level)
        : Character(name, level, 80+level*20, 12+level*3, 6+level*2, 12+level),
          gold(level*25), experience(0), experienceToNext(level*100),
          heroicStrikeCooldown(0), shieldBashCooldown(0) {
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
            // cooldowns and initial usage will always be the same
        }
        return *this;
    }
                                                            // DESTRUCTORI //
    ~Player() override {
        std::cout << "Player" << getName() << "destroyed\n";
    }
                                                            // METODE VIRTUALE SUPRASCRISE //
    void attack(Character &target) override {
        int damage=calculateDamage(target);
        //aici aplicam abilitatile
        //tocmai a fost folosit heroic strike? -> cooldown=3, la fel pt shield bash
        if (heroicStrikeCooldown == 3) {
            damage*=2;
            std::cout << "HEROIC STRIKE activated! ";
        }
        if (shieldBashCooldown == 4) {
            damage*=static_cast<int>(1.5);
            target.setSpeed(target.getSpeed()-2);
            std::cout << "SHIELD BASH activated!\n";
        }
        std::cout << "Player " << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        target.takeDamage(damage);
        //specific pentru player: obtinerea experientei
        gainExperience(25);
    }

    void specialAbility() override {
        std::cout << getName() << "\nChoose special ability:\n";
        std::cout << "1. Heroic Strike (2x damage, 3 turn cooldown)\n";
        std::cout << "2. Shield Bash (1.5x damage + enemy SPD lowers by 2, 4 turn cooldown)\n";
        std::cout << "3. Second Wind (heal, x2 uses per battle)\n";
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1: heroicStrike();
            case 2: shieldBash();
            case 3: secondWind();
            default: std::cout<<"Invalid choice!\n";
        }
        if (heroicStrikeCooldown > 0) heroicStrikeCooldown--;
        if (shieldBashCooldown > 0) shieldBashCooldown--;

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
        inventory.addItem(item);
        std::cout << "Added " << item << " to inventory\n";
    }

    void showInventory() const {
        inventory.display();
        std::cout << "Gold: " << gold << "\n";
    }
                                                           // ABILITATI //
private:
    bool heroicStrike() {
        if (heroicStrikeCooldown > 0) {
            std::cout<<"Heroic Strike is on cooldown for " << heroicStrikeCooldown << "more turns!\n";
            return false;
        }
        heroicStrikeCooldown=3;
        return true;
    }
    bool shieldBash() {
        if (shieldBashCooldown > 0) {
            std::cout<<"Shield Bash is on cooldown for " << shieldBashCooldown << "more turns!\n";
            return false;
        }
        shieldBashCooldown=4;
        return true;
    }
    void secondWind() {
        static int usesRemaining = 2;
        if (usesRemaining <= 0) {
            std::cout << "You're too exhausted to use Second Wind again!\n";
            return;
        }
        std::cout<<name<<" uses SECOND WIND!\n";
        int healAmount = maxHealth*static_cast<int>(0.3);
        health+=healAmount;
        if (health > maxHealth) health = maxHealth;
        std::cout<<"Healed for " << healAmount << " HP! (Now: " << health << "/" << maxHealth << ")\n";
        usesRemaining--;
    }

                                                            // GETTERI //
public:
    [[nodiscard]] int getGold() const { return gold; }
    [[nodiscard]] int getExp() const { return experience; }
    void setGold(int setgold) { gold = setgold; }
    void setExp(int setexp) { experience = setexp; }
};
                                                            /// CLASA 'ENEMY' - DERIVATA LUI 'CHARACTER' ///
class Enemy : public Character {
public:
    enum class EnemyType {GOBLIN, SKELETON, ORC, SLIME}; // Vom avea mai multe feluri de inamici
    enum class Difficulty {EASY, MEDIUM, HARD, BOSS}; // Pentru a calcula dinamic atributele inamicului si rewardul
protected:
    EnemyType type;
    Difficulty difficulty;
    int baseReward;
    std::string description;
    bool abilityUsed;
public:
                                                            // CONSTRUCTORI //
    //Constructor default/supraincarcat
    Enemy(const std::string& name, EnemyType type, Difficulty diff, int level)
        : Character(name, level,
                      calculateHealth(type,diff,level),
                      calculateAttack(type,diff,level),
               calculateDefense(type,diff,level),
                calculateSpeed(type,diff,level)),
          type(type), difficulty(diff), baseReward(calculateReward(type,diff,level)), abilityUsed(false) {
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
        if (abilityUsed) {
            std::cout << name <<"'s special ability is on cooldown!\n";
            return;
        }
        std::cout << getName() << " used ";
        switch(type) {
            case EnemyType::GOBLIN:
                goblinSpecial();
                break;
            case EnemyType::SKELETON:
                skeletonSpecial();
                break;
            case EnemyType::ORC:
                orcSpecial();
                break;
            case EnemyType::SLIME:
                slimeSpecial();
                break;
        }
        abilityUsed = true;
    }
                                                            // METODE HELPER //
    //Calcularea atributelor
    static int calculateHealth (EnemyType type, Difficulty diff, int level) {
        int base = 50+level*10;
        switch (type) {
            case EnemyType::GOBLIN: base = 40+level*8; break;
            case EnemyType::SKELETON: base = 60+level*12; break;
            case EnemyType::ORC: base = 80+level*15; break;
            case EnemyType::SLIME: base = 100+level*5; break;
        }
        // Difficulty multiplier
        switch (diff) {
            case Difficulty::EASY: return base;
            case Difficulty::MEDIUM: return base*static_cast<int>(1.5);
            case Difficulty::HARD: return base*2;
            case Difficulty::BOSS: return base*3;
        }
        return base;
    }
    static int calculateAttack (EnemyType type, Difficulty diff, int level) {
        int base = 5+level*2;
        switch (type) {
            case EnemyType::GOBLIN: base = 3+level; break;
            case EnemyType::SKELETON: base = 6+level*2; break;
            case EnemyType::ORC: base = 8+level*3; break;
            case EnemyType::SLIME: base = 2+level; break;
        }
        // Difficulty multiplier
        switch (diff) {
            case Difficulty::EASY: return base;
            case Difficulty::MEDIUM: return base*static_cast<int>(1.3);
            case Difficulty::HARD: return base*static_cast<int>(1.6);
            case Difficulty::BOSS: return base*2;
        }
        return base;
    }
    static int calculateDefense (EnemyType type, Difficulty diff, int level) {
        int base = 3+level*2;
        switch (type) {
            case EnemyType::GOBLIN: base = 3+level*2; break;
            case EnemyType::SKELETON: base = 5+level*3; break;
            case EnemyType::ORC: base = 2+level; break;
            case EnemyType::SLIME: base = 10+level*3; break;
        }
        // Difficulty multiplier
        switch (diff) {
            case Difficulty::EASY: return base;
            case Difficulty::MEDIUM: return base*static_cast<int>(1.3);
            case Difficulty::HARD: return base*static_cast<int>(1.6);
            case Difficulty::BOSS: return base*2;
        }
        return base;
    }
    static int calculateSpeed (EnemyType type, Difficulty diff, int level) {
        int base = 7+level*2;
        switch (type) {
            case EnemyType::GOBLIN: base = 5+level*2; break;
            case EnemyType::SKELETON: base = 2+level*2; break;
            case EnemyType::ORC: base = 3+level; break;
            case EnemyType::SLIME: base = 2+level; break;
        }
        // Difficulty multiplier
        switch (diff) {
            case Difficulty::EASY: return base;
            case Difficulty::MEDIUM: return base*static_cast<int>(1.2);
            case Difficulty::HARD: return base*static_cast<int>(1.4);
            case Difficulty::BOSS: return base*static_cast<int>(1.6);
        }
        return base;
    }
    static int calculateReward(EnemyType type, Difficulty diff, int level) {
        int base = level * 10;
        switch (type) {
            case EnemyType::GOBLIN: base = level*5; break;
            case EnemyType::SKELETON: base = level*12; break;
            case EnemyType::ORC: base = level*15; break;
            case EnemyType::SLIME: base = level*8; break;
        }
        switch (diff) {
            case Difficulty::EASY: return base;
            case Difficulty::MEDIUM: return base*2;
            case Difficulty::HARD: return base*3;
            case Difficulty::BOSS: return base*5;
        }
        return base;
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
        // si acum adaugam si dificultatea
        switch (difficulty) {
            case Difficulty::EASY: description+= " (Easy)"; break;
            case Difficulty::MEDIUM: description+= " (Medium)"; break;
            case Difficulty::HARD: description+= " (Hard)"; break;
            case Difficulty::BOSS: description+= " (BOSS)"; break;
        }
    }
                                                            // ABILITATI INDIVIDUALE //
private:
    void goblinSpecial() {
        std::cout << "TRICK! Goblin increases SPD by 5!\n";
        speed+=5;
    }
    void skeletonSpecial() {
        std::cout << "FORTIFY! Skeleton increases DEF by 10!\n";
        defense+=10;
    }
    void orcSpecial() {
        std::cout << "RAGE! Orc increases ATK by 15, but loses 5 DEF!\n";
        attackPower+=15;
        defense-=5;
    }
    void slimeSpecial() {
        std::cout << "BULWARK FORM! Slime increases DEF by 15, but loses 5 ATK and 5 SPD!\n";
        defense+=15;
        attackPower-=5;
        speed-=5;
    }

};
                                                            /// CLASA 'BOSS' - DERIVATA LUI 'ENEMY' ///
class Boss : public Enemy {
protected:
    std::string title;
    bool hasSpecialPhase; // la un anumit punct, bossul intra in phase 2 pentru a il distinge mai bine de un inamic normal
    int specialPhaseThreshold;
    int turnCounter; // abilitati phase-specific
public:
                                                            // CONSTRUCTORI //
    Boss(const std::string& name, EnemyType type, Difficulty diff, int level,
         std::string title)
        : Enemy(name, type, diff, level),  // apeleaza constructorul 'Enemy'
          title(std::move(title)), hasSpecialPhase(false),
          specialPhaseThreshold(getMaxHealth() / 2), turnCounter(0) {
        // Bossi sunt mai puternici
        maxHealth*=2;
        health=maxHealth;
        attackPower+=20;
        defense+=10;
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
        turnCounter++;
        //vf daca suntem in faza speciala
        if (!hasSpecialPhase && getHealth() <= specialPhaseThreshold) {
            enterSpecialPhase();
        }
        //la fiecare 3 ture, bossul devine mai puternic
        if (turnCounter % 3 == 0) {
            std::cout << title << " " << name << " grows more powerful!\n";
            attackPower += 5;
        }
    }

    void specialAbility() override {
        if (hasSpecialPhase) {
            enragedSpecial();
        } else {
            Enemy::specialAbility(); // apelam abilitatea speciala normala
        }
    }
                                                            // METODE SPECIFICE BOSSULUI //
private:
    void enragedSpecial() {
        std::cout<< "\n!!!" << title << " " << name << " ULTIMATE ATTACK !!!\n";
        attackPower*=4;
    }

    void enterSpecialPhase() {
        hasSpecialPhase = true;
        std::cout << "\n!!! PHASE 2 !!!\n";
        std::cout << title << " " << getName() << " is enraged!\n";
        attackPower*=2;
        speed*=2;
    }
public:
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

                                                            /// !!! IERARHIE DE EXCEPTII !!! ///
class Exception : public std::exception {
protected:
    std::string message;
public:
    explicit Exception(std::string msg) : message(std::move(msg)) {}
    ~Exception() override = default;
    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};

class CombatException : public Exception {
public:
    explicit CombatException(std::string msg)
        : Exception("Combat Error: " + std::move(msg)) {}
};

class InventoryException : public Exception {
public:
    explicit InventoryException(std::string msg)
        : Exception("Inventory Error: " + std::move(msg)) {}
};

class PlayerDeathException : public CombatException {
public:
    PlayerDeathException()
        : CombatException("Player has been defeated!") {}
};
                                                            /// !!!!! DESIGN PATTERNS !!!!! ///
                                                            /// SINGLETON - SHOP ///
class Shop {
private:
    static Shop* instance;
    std::map<std::string, int> items; // item name -> pret

    //Constructor private pentru Singleton
    Shop() {
        //Initializam cateva iteme
        items["Health Potion"] = 50;
        items["Attack Boost"] = 100;
        items["Defense Boost"] = 100;
        items["Speed Boost"] = 75;
    }
public:
    // copy constructor si atribuire delete
    Shop(const Shop& other) = delete;
    Shop& operator=(const Shop&) = delete;

    // get singleton instance
    static Shop& getInstance() {
        if (instance == nullptr) {
            instance = new Shop();
        }
        return *instance;
    }
    void displayItems() const {
        std::cout<<"\n=== SHOP ===\n";
        for (const auto& item : items) {
            std::cout<<" - " << item.first << ": " << item.second << " gold\n";
        }
    }

    bool buyItem(Player& player, const std::string& itemName) {
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
private:
    static void applyItemEffect(Player& player, const std::string& itemName) {
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
};
Shop* Shop::instance = nullptr;
                                                            /// FACTORY - ENEMYFACTORY ///
class EnemyFactory {
public:
    static std::unique_ptr<Enemy> createEnemy(
        Enemy::EnemyType type,
        const std::string& name,
        int level,
        Enemy::Difficulty diff = Enemy::Difficulty::MEDIUM) {
        return std::make_unique<Enemy>(name,type,diff,level);
    }
    static std::unique_ptr<Boss> createBoss(
        const std::string&name,
        const std::string& title = "Boss") {
        return std::make_unique<Boss>(
            name,
            Enemy::EnemyType::ORC, //bossi vor fi orci
            Enemy::Difficulty::BOSS,
            15,
            title);
    }
};
                                                            /// !!!!! GAME MANAGER (GAME LOOP) !!!!! ///
class GameManager {};

int main() {
};