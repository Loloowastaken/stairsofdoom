#include <Boss.h>
    Boss::Boss(const std::string& name, EnemyType type, Difficulty diff, int level,
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
    Boss& Boss::operator=(const Boss& other) {
        if (this != &other) {
            Enemy::operator=(other);  // Apeleaza operatorul copy de la Enemy
            title = other.title;
            hasSpecialPhase = other.hasSpecialPhase;
            specialPhaseThreshold = other.specialPhaseThreshold;
        }
        return *this;
    }
    Boss::~Boss() {
        std::cout << "Boss " << getName() << " destroyed\n";
    }
    void Boss::takeDamage(int damage)  {
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
    void Boss::specialAbility()  {
        if (hasSpecialPhase) {
            enragedSpecial();
        } else {
            Enemy::specialAbility(); // apelam abilitatea speciala normala
        }
    }
    void Boss::enragedSpecial() {
        std::cout<< "\n!!!" << title << " " << name << " ULTIMATE ATTACK !!!\n";
        attackPower*=4;
    }
    void Boss::enterSpecialPhase() {
        hasSpecialPhase = true;
        std::cout << "\n!!! PHASE 2 !!!\n";
        std::cout << title << " " << getName() << " is enraged!\n";
        attackPower*=2;
        speed*=2;
    }
    void Boss::displayBossInfo() const {
        displayStatus();
        std::cout<< " - " << title << "\n";
        if (hasSpecialPhase) {
            std::cout<<" (ENRAGED)\n";
        }
    }