#include "EnemyFactory.h"
#include <algorithm>
#include <array>

// Initialize static random generator
std::mt19937 EnemyFactory::rng(std::random_device{}());

// Random utilities
int EnemyFactory::randomInt(const int min, const int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

bool EnemyFactory::randomChance(const int percentage) {
    return randomInt(1, 100) <= percentage;
}

Enemy::EnemyType EnemyFactory::randomType() {
    switch(randomInt(0, 3)) {
        case 0: return Enemy::EnemyType::GOBLIN;
        case 1: return Enemy::EnemyType::SKELETON;
        case 2: return Enemy::EnemyType::ORC;
        case 3: return Enemy::EnemyType::SLIME;
        default: return Enemy::EnemyType::GOBLIN;
    }
}

// Name generators
std::string EnemyFactory::generateName() {
    static const std::array<std::string, 31> names = {
        "Affrath", "Belzagoth", "Charflah", "Dezmaziff", "Egloheim",
        "Fiffirth", "Gigalomaniak", "Hihihirim", "Innora", "Jajjaggar",
        "Kilminos", "Lorgrehth", "Momarnonion", "Nonisth", "Offorohoth",
        "Piplainan", "Quaqua", "Rutabaga", "Stintrith", "Titaniacaragorossh",
        "Uu", "Vivivviv", "Whaleworrn", "Xaxxaffras", "Yorgaloth", "Zumba",
        "Lospollos", "Lasplagas", "Deflaunth", "Jim", "Goggorogorogorogogggorogoggorogoroth"
    };
    return names[randomInt(0, names.size() - 1)];
}
// Description generator
std::string EnemyFactory::generateDescription(const Enemy::EnemyType type, const Enemy::Difficulty diff,const std::string& name, const int level)
    {
    std::string description;

    //Varied description for type
    switch(type) {
        case Enemy::EnemyType::GOBLIN:
            description = "A sneaky goblin named " + name + ". ";
            break;
        case Enemy::EnemyType::SKELETON:
            description = "A sturdy looking skeleton called " + name + ". ";
            break;
        case Enemy::EnemyType::ORC:
            description = "A brutish orc known as " + name + ". ";
            break;
        case Enemy::EnemyType::SLIME:
            description = "A gelatinous slime named " + name + ". ";
            break;
    }

    //Varied description for difficulty
    switch(diff) {
        case Enemy::Difficulty::EASY:
            description += "Looks inexperienced and nervous. ";
            break;
        case Enemy::Difficulty::MEDIUM:
            description += "Seems confident and experienced. ";
            break;
        case Enemy::Difficulty::HARD:
            description += "Appears bloodthirsty and dangerous. ";
            break;
        case Enemy::Difficulty::BOSS:
            description += "The pinnacle of might. ";
            break;
    }
    // Add level info
    description += " [Level " + std::to_string(level) + "]\n";
    return description;
}

// Factory methods
Enemy::EnemyType EnemyFactory::getRandomTypeForFloor(const int floorLevel) {
    // More varied enemies on higher floors
    if (floorLevel <= 5) {
        return Enemy::EnemyType::GOBLIN;  // Only goblins early
    }
    if (floorLevel <= 15) {
        return randomInt(0, 1) == 0 ? Enemy::EnemyType::GOBLIN : Enemy::EnemyType::SKELETON;
    }
    if (floorLevel <= 20) {
        switch(randomInt(0, 2)) {
            case 0: return Enemy::EnemyType::GOBLIN;
            case 1: return Enemy::EnemyType::SKELETON;
            case 2: return Enemy::EnemyType::SLIME;
            default: return Enemy::EnemyType::GOBLIN;
        }
    }
    return randomType();  // All types available
}

Enemy::Difficulty EnemyFactory::calculateDifficulty(const int floorLevel) {
    if (floorLevel <= 10) return Enemy::Difficulty::EASY;
    if (floorLevel <= 30) return Enemy::Difficulty::MEDIUM;
    if (floorLevel <= 80) return Enemy::Difficulty::HARD;
    return Enemy::Difficulty::BOSS;
}

int EnemyFactory::calculateEnemyLevel(const int floorLevel, const Enemy::Difficulty diff) {
    const int baseLevel = floorLevel;
    
    // Adjust based on difficulty
    switch(diff) {
        case Enemy::Difficulty::EASY:
            return std::max(1, baseLevel - 1);
        case Enemy::Difficulty::MEDIUM:
            return baseLevel;
        case Enemy::Difficulty::HARD:
            return baseLevel + 1;
        case Enemy::Difficulty::BOSS:
            return baseLevel + 3;
    }
    return baseLevel;
}

std::unique_ptr<Character> EnemyFactory::createEnemy(Enemy::EnemyType type, Enemy::Difficulty diff, const int floorLevel) {
    // Generate name
    std::string name = generateName();
    // Calculate level
    int level = calculateEnemyLevel(floorLevel, diff);
    // Create enemy
    auto enemy = std::make_unique<Enemy>(name, type, diff, level);
    // Generate and set description
    const std::string description = generateDescription(type, diff, name, level);
    enemy->setDescription(description);
    return enemy;
}

std::unique_ptr<Character> EnemyFactory::createRandomEnemy(const int floorLevel) {
    const Enemy::EnemyType type = getRandomTypeForFloor(floorLevel);
    const Enemy::Difficulty diff = calculateDifficulty(floorLevel);
    
    return createEnemy(type, diff, floorLevel);
}

std::unique_ptr<Character> EnemyFactory::createBoss(const int floorLevel, const std::string& title) {
    std::string bossName = generateName();
    std::string bossTitle = title.empty() ? 
        "Guardian of Floor " + std::to_string(floorLevel) : title;
    
    // Bosses are always ORC type and BOSS difficulty
    auto boss = std::make_unique<Boss>(
        bossName,
        Enemy::EnemyType::ORC,
        Enemy::Difficulty::BOSS,
        floorLevel + 5,  // Bosses are higher level
        bossTitle
    );
    
    // Generate boss description
    const std::string description = bossTitle + " " + bossName + ".";
    boss->setDescription(description);
    
    return boss;
}

std::vector<std::unique_ptr<Character>> EnemyFactory::createEnemyGroup(const int floorLevel, const int minCount, const int maxCount) {
    std::vector<std::unique_ptr<Character>> enemies;
    const int enemyCount = randomInt(minCount, maxCount);
    for (int i = 0; i < enemyCount; ++i) {
        enemies.push_back(createRandomEnemy(floorLevel));
    }
    return enemies;
}