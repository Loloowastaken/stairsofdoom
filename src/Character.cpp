#include "Character.h"
#include "Exception.h"

int Character::totalCharacters = 0;

Character::Character()
    : name("Unknown"), level(1), health(100), maxHealth(100),
      attackPower(10), defense(5), speed(10) {
    totalCharacters++;
}
Character::Character(std::string name, const int level, const int health,
              const int attackPower, const int defense, const int speed)
        : name(std::move(name)), level(level), health(health), maxHealth(health),
          attackPower(attackPower), defense(defense), speed(speed) {
    totalCharacters++;
}
Character::Character(const std::string& name, const int level)
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
Character& Character::operator-=(const int damage) {
    health-=damage;
    if (health<0) health=0;
    return *this;
}
void Character::attack(Character& target) {
    const int damage = calculateDamage(target);
    std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n ";
    target.takeDamage(damage);
}
void Character::takeDamage(const int damage) {
    health-=damage;
    if (health<0) health=0;
}
Character::~Character()= default;
int Character::calculateDamage(const Character &target) const {
    const int damage = attackPower - (target.getDefense()/2);
    return (damage < 1) ? 1 : damage;
}
std::istream& operator>>(std::istream& is, Character& c) {
    std::string inputName;
    std::cout << "Enter character name: ";
    is >> inputName;
    c.setName(inputName);
    return is;
}
std::ostream& operator<<(std::ostream& os, Character& c) {
    os << c.getName() << " [LVL " << c.getLevel() << "] "
              << "HP: " << c.getHealth() << "/" << c.getMaxHealth()
              << " ATK: " << c.getAttackPower()
              << " DEF: " << c.getDefense()
              << " SPD: " << c.getSpeed() << std::endl;
    return os;
}
bool operator<(const Character& c1, const Character& c2) {
    return c1.getSpeed() < c2.getSpeed();
}
bool operator==(const Character& c1, const Character& c2) {
    return c1.getLevel() == c2.getLevel();
}

