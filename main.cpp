#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <typeinfo>

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
        : Character(name, level, 50+level*10, 5+level*2, 2+level, 5+level){}
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
        std::cout << name << " ataca " << target.getName() << " pentru " << damage << " damage!\n ";
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
        std::cout<< name << " distrus\n";
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
    void setLevel(int setlevel) { this->level = setlevel; }
    void setHealth(int sethealth) {
        this->health = (sethealth>maxHealth) ? maxHealth : sethealth;
        if (health < 0) health=0;
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
    //Constructor parametrizat
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
        std::cout << "Player " << getName() << " ataca " << target.getName() << "!\n";
        Character::attack(target); // apelarea implementarii de baza

        //specific pentru player: obtinerea experientei
        gainExperience(25);
    }

    void specialAbility() override {
        std::cout << getName() << " foloseste abilitatea speciala! Dublu damage pentru urmatorul atac!\n";
        setAttackPower(getAttackPower()*2);
    }
                                                            // METODE SPECIFICE PLAYER //
    void gainExperience(int xp) {
        experience += xp;
        std::cout << "Obtinut " << xp << " experienta. Total: "
                  << experience << "/" << experienceToNext << std::endl;
        while (experience>=experienceToNext) {
            experience-=experienceToNext;
            levelUp();
            experienceToNext += getLevel()*100;
        }
    }

    void levelUp() {
        setLevel(getLevel()+1);
        setMaxHealth(getMaxHealth()+30);
        setHealth(getMaxHealth()); // heal la levelup
        setAttackPower(getAttackPower()+8);
        setDefense(getDefense()+4);
        setSpeed(getSpeed()+2);
        std::cout << getName() << " a facut level up la " << getLevel() << "!\n";
    }

    void addGold(int amount) {
        gold += amount;
        std::cout << "Obtinut" << amount << " aur. Total: " << gold << "\n";
    }

    bool spendGold(int amount) {
        if (gold >= amount) {
            gold -= amount;
            std::cout << "Platit " << amount << " aur. Total ramas: " << gold << "\n";
            return true;
        }
        std::cout << "Prea putin aur! Ai nevoie de " << amount << ", dar ai " << gold << "\n";
        return false;
    }

    void addItem(const std::string& item) {
        inventory.push_back(item);
        std::cout << "Adaugat " << item << " la inventar\n";
    }

    void showInventory() const {
        std::cout << "Inventarul lui " << getName() << " (" << inventory.size() << " iteme):\n";
        for (const auto& item : inventory) {
            std::cout << " - " << item << std::endl;
        }
        std::cout << "Aur: " << gold << "\n";
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
            std::cout << "Cumparat: " << item << "\n";
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
        std::cout << ") ataca !\n";

        Character::attack(target);
    }

    void specialAbility() override {
        std::cout << getName() << " a folosit ";
        switch(type) {
            case EnemyType::GOBLIN:
                std::cout << "TRICK! Defense scazut pentru jucator!\n";
                break;
            case EnemyType::SKELETON:
                std::cout << "FORTIFY! Defense ridicat pentru inamic!\n";
                setDefense(getDefense()+5);
                break;
            case EnemyType::ORC:
                std::cout << "RAGE! Atac ridicat pentru inamic!\n";
                setAttackPower(getAttackPower()+10);
                break;
            case EnemyType::SLIME:
                std::cout << "ACID! Otrava peste timp!\n";
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
                description = "Creatura mica si sireata";
                break;
            case EnemyType::SKELETON:
                description = "Soldat nemuritor";
                break;
            case EnemyType::ORC:
                description = "Barbar si puternic";
                break;
            case EnemyType::SLIME:
                description = "Monstru gelatinos";
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
        setMaxHealth(getMaxHealth()*2);
        setHealth(getMaxHealth());
        setAttackPower(getAttackPower()+10);
        setDefense(getDefense()+5);
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
        std::cout << "Bossul " << getName() << " distrus\n";
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
                      << " foloseste ATAC SUPREM! Damage devastator!\n";
        } else {
            Enemy::specialAbility(); // apelam abilitatea speciala normala
        }
    }
                                                            // METODE SPECIFICE BOSSULUI //
    void enterSpecialPhase() {
        hasSpecialPhase = true;
        std::cout << "\n!!! PHASE 2 !!!\n";
        std::cout << title << " " << getName() << " este infuriat!\n";
        setAttackPower(getAttackPower()*2);
        setSpeed(getSpeed()*2);
    }
                                                            // GETTERI //
    [[nodiscard]] const std::string& getTitle() const { return title; }
    [[nodiscard]] bool isInSpecialPhase() const { return hasSpecialPhase; }

    void displayBossInfo() const {
        displayStatus();
        std::cout<< " - " << title << "\n";
        if (hasSpecialPhase) {
            std::cout<<" (INFURIAT)\n";
        }
    }
};

int main() {
    //Demonstrare upcasting, downcasting

    //UPCASTING
    Player player("Hero", 5);
    Character* characterPtr = &player; //Upcast
    std::cout << "1. Upcast Player la Character*\n";
    characterPtr->displayStatus();
    std::cout<<"\n";

    //Folosirea metodelor virtuale prin pointer de baza
    Enemy goblin("Gubby", Enemy::EnemyType::GOBLIN, 3);
    Character* enemyPtr = &goblin; //Upcast

    std::cout<< "2. Apelarea metodei virtuale prin pointer de baza:\n";
    characterPtr->attack(goblin); // Apeleaza Player::attack()
    enemyPtr->attack(player); // Apeleaze Enemy::attack()

    //DOWNCASTING
    std::cout<< "\n3. Downcast cu dynamic_cast (RTTI):\n";

    std::vector<Character*> characters;
    characters.push_back(&player);
    characters.push_back(&goblin);

    Boss orc("Ziggurat", Enemy::EnemyType::ORC, 15, "cel Flamand");
    characters.push_back(&orc);

    for (Character* ptr : characters) {
        // Incercam downcast la Player
        if (auto* playerPtr = dynamic_cast<Player*>(ptr)) {
            std::cout << " - Gasit Player: " << playerPtr->getName()
                      << " (Aur: " << playerPtr->getGold() << ")\n";
            playerPtr->showInventory();
        }
        // Incercam downcast la Enemy
        else if (auto* enemy_Ptr = dynamic_cast<Enemy*>(ptr)) {
            std::cout<< " - Gasit Enemy: " << enemy_Ptr->getName()
                    << " (Reward: " << enemy_Ptr->calculateReward() << " gold)\n";
        }
        // Incercam downcast la Boss
        else if (auto* bossPtr = dynamic_cast<Boss*>(ptr)) {
            std::cout<< " Este un BOSS: " << bossPtr->getTitle() << "\n";
        }
    }

    //Verificam tipul cu typeid
    std::cout<<"\n4. Verificarea tipului cu typeid:\n";
    std::cout<<"player type: " << typeid(player).name() << "\n";
    std::cout<<"goblin type: " << typeid(goblin).name() << "\n";
    std::cout<<"orc type: " << typeid(orc).name() << "\n";

    //Demonstrarea polimorfismului in lupta
    std::cout<< "\n5. Simulare de lupta\n";

    //Facem o echipa mixta (asta e DOAR pentru demonstrare, nu vom avea niciodata inamici cu player)
    std::vector<std::unique_ptr<Character>> party;
    party.push_back(std::make_unique<Player>("Razboinic",8));
    party.push_back(std::make_unique<Enemy>("Schelet",Enemy::EnemyType::SKELETON,6));

    for (auto&member:party) {
        member->specialAbility();
    }
    std::cout<<"\nNumarul total de personaje este: \n";
    std::cout<<Character::getTotalCharacters()<<"\n";

};