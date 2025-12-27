#include <Enemy.h>
    Enemy::Enemy(const std::string& name, EnemyType type, Difficulty diff, int level)
        : Character(name, level,
                      calculateHealth(type,diff,level),
                      calculateAttack(type,diff,level),
               calculateDefense(type,diff,level),
                calculateSpeed(type,diff,level)),
          type(type), difficulty(diff), baseReward(calculateReward(type,diff,level)), abilityUsed(false) {
        setDescription();
    }
    Enemy& Enemy::operator=(const Enemy& other) {
        if (this != &other) {
            Character::operator=(other);  // Call base assignment
            type = other.type;
            baseReward = other.baseReward;
            description = other.description;
        }
        return *this;
    }
    Enemy::~Enemy() {
        std::cout << "Enemy " << getName() << " destroyed\n";
    }
    void Enemy::attack(Character& target) {
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

    void Enemy::specialAbility() {
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
    int Enemy::calculateHealth (EnemyType type, Difficulty diff, int level) {
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
            case Difficulty::MEDIUM: return static_cast<int>(base*1.5);
            case Difficulty::HARD: return base*2;
            case Difficulty::BOSS: return base*3;
        }
        return base;
    }
    int Enemy::calculateAttack (EnemyType type, Difficulty diff, int level) { // should be HIGH to avoid frequent slapfights
        int base = 5+level*2;
        switch (type) {
            case EnemyType::GOBLIN: base+=5+level; break;
            case EnemyType::SKELETON: base+=7+level*2; break;
            case EnemyType::ORC: base+=8+level*3; break;
            case EnemyType::SLIME: base+=2+level; break;
        }
        // Difficulty multiplier
        switch (diff) {
            case Difficulty::EASY: return base;
            case Difficulty::MEDIUM: return static_cast<int>(base*1.3);
            case Difficulty::HARD: return static_cast<int>(base*1.6);
            case Difficulty::BOSS: return base*2;
        }
        return base;
    }
    int Enemy::calculateDefense (EnemyType type, Difficulty diff, int level) {
        int base = 3+level; // this stat is DANGEROUS, makes combat insufferable
        switch (type) {
            case EnemyType::GOBLIN: base = 3+level; break;
            case EnemyType::SKELETON: base = 5+level; break;
            case EnemyType::ORC: base = 2+level; break;
            case EnemyType::SLIME: base = 10+level; break;
        }
        // Difficulty multiplier
        switch (diff) {
            case Difficulty::EASY: return base;
            case Difficulty::MEDIUM: return static_cast<int>(base*1.3);
            case Difficulty::HARD: return static_cast<int>(base*1.6);
            case Difficulty::BOSS: return base*2;
        }
        return base;
    }
    int Enemy::calculateSpeed (EnemyType type, Difficulty diff, int level) {
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
            case Difficulty::MEDIUM: return static_cast<int>(base*1.2);
            case Difficulty::HARD: return static_cast<int>(base*1.4);
            case Difficulty::BOSS: return static_cast<int>(base*1.6);
        }
        return base;
    }
    int Enemy::calculateReward(EnemyType type, Difficulty diff, int level) {
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
    void Enemy::setDescription() {
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
    void Enemy::goblinSpecial() {
        std::cout << "TRICK! Goblin increases SPD by 5!\n";
        speed+=5;
    }
    void Enemy::skeletonSpecial() {
        std::cout << "FORTIFY! Skeleton increases DEF by 10!\n";
        defense+=10;
    }
    void Enemy::orcSpecial() {
        std::cout << "RAGE! Orc increases ATK by 15, but loses 5 DEF!\n";
        attackPower+=15;
        defense-=5;
    }
    void Enemy::slimeSpecial() {
        std::cout << "BULWARK FORM! Slime increases DEF by 15, but loses 5 ATK and 5 SPD!\n";
        defense+=15;
        attackPower-=5;
        speed-=5;
    }