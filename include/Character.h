#pragma once
#include <iostream>
#include <string>
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
    Character();
    // Constructor parametrizat
    Character(std::string  name, int level, int health,
              int attackPower, int defense, int speed);
    // Constructor supraincarcat
    Character(const std::string& name, int level);
    // Constructor copy
    Character(const Character &other);
                                                        // OPERATORI MEMBRI //
    //Operator de copiere
    Character& operator=(const Character &other);

    //Operator membru +=, folosit pt healing
    Character& operator+=(int healAmount);

    //Operator membru -=, folosit pt damage
    Character& operator-=(int damage);
                                                            // METODE VIRTUALE //

    //metoda pentru atac
    virtual void attack(Character& target);
    //metoda pentru a lua damage
    virtual void takeDamage(int damage);
    //metoda virtuala pura, abilitati folosite si de player, si de inamic
    virtual void specialAbility() = 0; //face ca 'Character' sa fie clasa abstracta

                                                            // DESTRUCTORI //
    virtual ~Character(); // destructor virtual
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
    void displayStatus() const;
    [[nodiscard]] bool isAlive() const { return health > 0; }
    //Metoda statica
    static int getTotalCharacters() { return totalCharacters; };
    //Metoda helper pentru a calcula damage
    [[nodiscard]] int calculateDamage(const Character& target) const;
};

// OPERATORI NON-MEMBRI //
//Operator output
std::ostream& operator<<(std::ostream& os, const Character& c);
//Operator input
std::istream& operator>>(std::istream& is, Character& c);
//Operator <, folosit pentru a determina cine face prima miscare, bazata pe speed
bool operator<(const Character& c1, const Character& c2);
//Operator ==, folosit pentru a verifica daca sunt acelasi nivel/putere
bool operator==(const Character& c1, const Character& c2);