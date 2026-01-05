#pragma once
#include <memory>
#include <random>
#include <vector>
#include <Character.h>
#include <Player.h>

/// CLASA GAMEMANAGER = GAMEPLAY LOOP ///
class GameManager {
private:
    std::unique_ptr<Player> player;
    int currentFloor;
    int totalFloors;
    std::vector<std::unique_ptr<Character>> enemies;
    bool gameRunning;

    //game state
    int enemiesDefeated;
    int goldCollected;
    int floorsCleared;
    bool hasFought; // if lebron was a boolean
    bool playerFled; // catering for useless mechanics award

    //evil rng
    std::mt19937 rng;

public:
    GameManager();
    ~GameManager();

    void startGame();
    void mainMenu();
    void gameLoop();
    void gameOver(bool won);

private:
    void generateEnemies();
    void displayFloorInfo() const;
    void displayPlayerStatus() const;

    static void displayCombatMenu();
    void processPlayerDeath();
    void exploreFloor();

    void combatPhase();
    void shopPhase() const;
    void inventoryPhase() const;

    //Combat mechanics
    void playerTurn();
    void enemyTurn(Character& enemy);
    void useItem() const;
    [[nodiscard]] bool isStalemate() const; // just trust me
    [[nodiscard]] int calculateFleeChance() const;
    bool fleeCombat();

    //Event handlers
    void handleLevelUp() const;
    void handleVictory();
    void handleFloorCompletion();
    void handleCombatVictory();

    //Random utilities
    int randomInt(int min, int max);
    bool randomChance(int percentage);

};