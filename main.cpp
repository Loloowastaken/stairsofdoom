#include <iostream>
#include <string>
#include <memory>

class Character {
private:
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
    Character(const std::string& name, int level, int health,
              int attackPower = 10, int defense = 5, int speed = 10)
        : name(name), level(level), health(health), maxHealth(health),
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
        int damage= calculateDamage(target);
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
    //metoda virtuala pura
    virtual void specialAbility() = 0;
                                                            // DESTRUCTORI //
    virtual ~Character() {
        std::cout<< "Caracterul " << name << " distrus\n";
    }
                                                            // GETTERS/SETTERS //
    [[nodiscard]] const std::string& getName() const {
        return name;
    }

    [[nodiscard]] int getLevel() const {
        return level;
    }

    [[nodiscard]] int getHealth() const {
        return health;
    }

    [[nodiscard]] int getMaxHealth() const {
        return maxHealth;
    }

    [[nodiscard]] int getAttackPower() const {
        return attackPower;
    }

    [[nodiscard]] int getDefense() const {
        return defense;
    }

    [[nodiscard]] int getSpeed() const {
        return speed;
    }

    void setName(const std::string &setname) {
        this->name = setname;
    }

    void setLevel(int setlevel) {
        this->level = (setlevel > 0) ? setlevel : 1;
    }

    void setHealth(int sethealth) {
        this->health = sethealth;
    }

    void setMaxHealth(int setmax_health) {
        this->maxHealth = setmax_health;
    }

    void setAttackPower(int setattack_power) {
        this->attackPower = setattack_power;
    }

    void setDefense(int setdefense) {
        this->defense = setdefense;
    }

    void setSpeed(int setspeed) {
        this->speed = setspeed;
    }
                                                            // ALTE METODE //
    void LevelUp() {
        level++;
        maxHealth += 20;
        health = maxHealth; // restore viata cand levelup
        attackPower += 5;
        defense += 3;
        speed += 1;
    }
    void displayStatus() const {
        std::cout << name << " [LVL " << level << "] "
                  << "HP: " << health << "/" << maxHealth
                  << " ATK: " << attackPower
                  << " DEF: " << defense
                  << " SPD: " << speed << std::endl;
    }
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
    int inputLevel, inputAttack , inputDefense, inputSpeed;
    std::cout << "Enter character name: ";
    is >> inputName;
    c.setName(inputName);

    std::cout << "Enter level: ";
    is >> inputLevel;
    c.setLevel(inputLevel);

    std::cout << "Enter attack power: ";
    is >> inputAttack;
    c.setAttackPower(inputAttack);

    std::cout << "Enter defense: ";
    is >> inputDefense;
    c.setDefense(inputDefense);

    std::cout << "Enter speed: ";
    is >> inputSpeed;
    c.setSpeed(inputSpeed);

    c.setHealth(100+c.getLevel()*20);
    c.setMaxHealth(c.getHealth());

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


int main() {
    //Test class so CPPCheck will stfu
    class TestCharacter : public Character {
        public:
        TestCharacter(const std::string& name, int level = 1)
            : Character(name, level) {}

        void specialAbility() override {
            std::cout << "Special Ability\n";
        }
    };

    TestCharacter hero("A",5);
    TestCharacter enemy("B", 3);

    std::cout<< "Hero health before: " << hero.getHealth() << std::endl;
    hero.takeDamage(20);
    std::cout<< "Hero health after taking 20 damage: " << hero.getHealth() << std::endl;

    std::cout << "\nCombat test:\n";
    hero.attack(enemy);

    hero.displayStatus();
    enemy.displayStatus();

    std::cout<<"Operator tests:\n";
    std::cout<< "Hero: " << hero << std::endl;
    std::cout<< "Enemy: " << enemy << std::endl;

    std::cout<<"\nTurn order (speed comparison)\n";
    if (hero<enemy) {
        std::cout << enemy.getName() << " goes first\n";
    }
    else {
        std::cout << hero.getName() << " goes first\n";
    }

    std::cout<<"Total characters created: " << Character::getTotalCharacters()<<std::endl;


}