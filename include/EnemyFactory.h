#pragma once
#include "Enemy.h"
#include "Boss.h"
#include <memory>
#include <string>
#include <random>
#include <vector>

class EnemyFactory {
private:
    static std::mt19937 rng;

    // Name generators
    static std::string generateName();

    // Description generators
    static std::string generateDescription(Enemy::EnemyType type, Enemy::Difficulty diff, const std::string& name, int level);

public:
    EnemyFactory() = delete;
    // Factory methods
    static std::unique_ptr<Character> createEnemy(Enemy::EnemyType type,
                                             Enemy::Difficulty diff,
                                             int floorLevel);

    static std::unique_ptr<Character> createRandomEnemy(int floorLevel);

    static std::unique_ptr<Character> createBoss(int floorLevel, const std::string& title = "");

    static std::vector<std::unique_ptr<Character>> createEnemyGroup(int floorLevel, int minCount = 1, int maxCount = 3);

    // Helper methods
    static Enemy::EnemyType getRandomTypeForFloor(int floorLevel);
    static Enemy::Difficulty calculateDifficulty(int floorLevel);
    static int calculateEnemyLevel(int floorLevel, Enemy::Difficulty diff);

    // Static random utilities
    static int randomInt(int min, int max);
    static bool randomChance(int percentage);
    static Enemy::EnemyType randomType();
};