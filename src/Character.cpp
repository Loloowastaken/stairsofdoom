#include "Character.h"

int Character::totalCharacters = 0;

Character::Character()
    : name("Unknown"), level(1), health(100), maxHealth(100),
      attackPower(10), defense(5), speed(10) {
    totalCharacters++;
}
Character::Character(std::string  name, int level, int health,
              int attackPower, int defense, int speed)
        : name(std::move(name)), level(level), health(health), maxHealth(health),
          attackPower(attackPower), defense(defense), speed(speed) {
    totalCharacters++;
}
Character::Character(const std::string& name, int level)
        : Character(name, level, 50+level*10, 5+level*2, 2+level, 5+level) {
    totalCharacters++;
}
Character::Character(const Character &other)
        : name(other.name),
          level(other.level),
          health(other.health),
          maxHealth(other.maxHealth),
          attackPower(other.attackPower),
          defense(other.defense),
          speed(other.speed) {
    totalCharacters++;
}
Character& Character::operator=(const Character &other) {
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
Character& Character::operator+=(const int healAmount) {
    health+=healAmount;
    if (health>maxHealth) health=maxHealth;
    return *this;
}
Character& Character::operator-=(int damage) {
    health-=damage;
    if (health<0) health=0;
    return *this;
}
void Character::attack(Character& target) {
    int damage = calculateDamage(target);
    std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n ";
    target.takeDamage(damage);
}
void Character::takeDamage(int damage) {
    health-=damage;
    if (health<0) health=0;
}
Character::~Character()= default;
void Character::displayStatus() const {
    std::cout << name << " [LVL " << level << "] "
              << "HP: " << health << "/" << maxHealth
              << " ATK: " << attackPower
              << " DEF: " << defense
              << " SPD: " << speed << std::endl;
}

int Character::calculateDamage(const Character &target) const {
    int damage = attackPower - (target.getDefense()/2);
    return (damage < 1) ? 1 : damage;
}
std::istream& operator>>(std::istream& is, Character& c) {
    std::string inputName;
    std::cout << "Enter character name: ";
    is >> inputName;
    c.setName(inputName);
    return is;
}
bool operator<(const Character& c1, const Character& c2) {
    return c1.getSpeed() < c2.getSpeed();
}
bool operator==(const Character& c1, const Character& c2) {
    return c1.getLevel() == c2.getLevel();
}

