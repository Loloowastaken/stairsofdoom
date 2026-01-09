#include <GameManager.h>
#include <Enemy.h>
#include <Boss.h>
#include <Exception.h>
#include <Shop.h>
#include <iostream>

#include "EnemyFactory.h"

GameManager::GameManager()
    : currentFloor(1), totalFloors(100), gameRunning(true), enemiesDefeated(0),
      goldCollected(0), floorsCleared(0), hasFought(false), playerFled(false), rng(std::random_device{}()) {
    std::cout << "=== STAIRS OF DOOM ===" << std::endl;
}
GameManager::~GameManager() {
    std::cout << "Game session ended.\n";
}
void GameManager::startGame() {
    mainMenu();
}
void GameManager::mainMenu() {
    int choice=0;
    while (choice!=2) {
        std::cout << "\n===== MAIN MENU =====\n";
        std::cout << "\n1. Start Game (1)\n";
        std::cout << "\n2. End Game (2)\n";
        std::cin >> choice;
        switch (choice) {
            case 1: {
                player=std::make_unique<Player>("");
                std::cin>>*player;
                std::cout << "\nBeginning journey for " << player->getName() << ".\n";
                std::cout << "Your journey begins on Floor 1.\n";
                //Give out some starting items
                player->addItem("HealthPotion");
                player->addItem("HealthPotion");
                player->addGold(100);
                gameLoop();
                return;
            }
            case 2: {
                std::cout<<"Farewell.\n";
                gameRunning=false;
                return;
            }
            default:
                std::cout<<"Invalid choice.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    }
}
void GameManager::gameLoop() {
    std::cout << "\n=== ADVENTURE START ===\n";
    while (gameRunning && player && player->isAlive()) {
        displayFloorInfo();
        int choice = 0;
        std::cout<<"\nWhat would you like to do on this floor?\n";
        std::cout<<"1. Explore floor\n";
        std::cout<<"2. Visit shop\n";
        std::cout<<"3. Check inventory\n";
        std::cout<<"4. Advance to next floor\n";
        std::cout<<"5. Exit game\n";
        std::cout<<"Choice: ";
        std::cin>>choice;
        switch (choice) {
            case 1: exploreFloor(); break;
            case 2: shopPhase(); break;
            case 3: inventoryPhase(); break;
            case 4: if (enemies.empty() && hasFought==true) { //enemies being empty is a weak condition, we need to have fought them as well
                handleFloorCompletion();
            } else std::cout<<"You must defeat all enemies on this floor before proceeding.\n"; break;
            case 5:
                std::cout<<"Exiting game.\n"; // removes recursive chain
                return;
            default:
                std::cout<<"Invalid choice!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }

        //Check for gameover
        if (!player->isAlive()) {
            processPlayerDeath();
            return;
        }

        //Check for victory
        if (currentFloor > totalFloors) {
            handleVictory();
            return;
        }
    }
}

void GameManager::exploreFloor() {
    std::cout << "\n=== EXPLORING FLOOR " << currentFloor << " ===\n";
    //Random encounter
    if ((randomChance(70) && hasFought==false) || (hasFought==true && playerFled==true)) { //no grinding, check for cowardice
        if (!playerFled) generateEnemies(); //first time encounter
        hasFought=true;
        playerFled=false;
        std::cout<<"Encountered " << enemies.size() << " enemy(ies)!\n";
        combatPhase();
    } else {
        // find treasure or nothing
        if (randomChance(50)) {
            const int goldFound = randomInt(10, 50)*currentFloor;
            player->addGold(goldFound);
            goldCollected+=goldFound;
            std::cout<< "You found " << goldFound << " gold!\n";
        } else if (randomChance(30)) {
            std::cout<<"You found a Health Potion!\n";
            player->addItem("HealthPotion");
        } else {
            std::cout<<"The floor is empty...for now...\n";
        }
    }
}
void GameManager::combatPhase() {
    std::cout << "\n=== COMBAT START ===\n";

    //Determine turn order
    std::vector<Character*> turnOrder;
    turnOrder.push_back(player.get());
    for (auto& enemy : enemies) {
        turnOrder.push_back(enemy.get());
    }
    //Sort by speed (descending)
    std::ranges::sort(turnOrder, [](const Character* a, const Character* b) {
        return a < b; // folosind operatorul nonmembru
    });
    //Combat loop
    while (!enemies.empty() && player->isAlive()) {
        if (playerFled==true) {break;}
        for (const auto character : turnOrder) {
            if (character->isAlive()) std::cout << *character ;
        }
        for (auto* character : turnOrder) {
            if (!character->isAlive()) continue;
            if (character == player.get()) {
                playerTurn();
                if (playerFled==true) {break;}
            } else {
                // Find which enemy this is
                auto it = std::ranges::find_if(enemies, [character](const auto& enemy) {
                    return enemy.get() == character;
                });
                if (it != enemies.end() && (*it)->isAlive()) {
                    enemyTurn(**it);
                }
            }
        }
        //Remove dead enemies at the end of the round
        std::erase_if(enemies,[](const auto& enemy) {
            return !enemy->isAlive();
        });

        //Check for victory
        if (player->isAlive() && enemies.empty()) {
            handleCombatVictory(); break;
        }
        //Check for stalemate (enemy does <=3 ATK, player obviously wins, but it takes centuries)
        if (isStalemate()) {
            std::cout<<"\nThese wretches are no match for you! They run away in fear.\n";
            handleCombatVictory();
            enemies.clear();
        }
    }
}

bool GameManager::isStalemate() const {
    bool allEnemiesWeak=true;
    for (const auto&enemy:enemies) {
        if (const int enemyDamage=enemy->getAttackPower() - (player->getDefense()/2); enemyDamage>=3) {
            allEnemiesWeak=false;
            break;
        }
    }
    return allEnemiesWeak;
}

void GameManager::playerTurn() {
    displayCombatMenu();
    int choice = 0;
    std::cin>>choice;
    switch (choice) {
        case 1: {
            if (enemies.empty()) return;
            std::cout<<"\n Choose target:\n";
            for (size_t i = 0; i < enemies.size(); ++i) {
                std::cout << (i+1) << ". " << enemies[i]->getName() << " (" << enemies[i]->getHealth()
                << "/" << enemies[i]->getMaxHealth() << " HP)\n";
            }
            int targetChoice;
            std::cin>>targetChoice;
            if (!(targetChoice < 1 || targetChoice > static_cast<int>(enemies.size()))) {
                player->attack(*enemies[targetChoice - 1]);
                //Check if the enemy has died
                if (!enemies[targetChoice - 1]->isAlive()) {
                    std::cout << enemies[targetChoice-1]->getName() << "was defeated!\n";
                    enemiesDefeated++;
                    //Reward
                    if (const auto* enemy = dynamic_cast<Enemy*>(enemies[targetChoice-1].get())){
                        const int reward = Enemy::calculateReward(enemy->getType(), enemy->getDiff(), enemy->getLevel());
                        player->addGold(reward);
                        goldCollected+=reward;
                        std::cout<<"You gained " << reward << " gold!\n";
                    }
                }
            }
        } break;
        case 2: player->specialAbility(); break;
        case 3: try { useItem(); } catch (const InventoryException &e) { std::cout<<e.what();} break;
        case 4: { // i implemented fleeing on a whim, and it was my worst mistake.
            if (!fleeCombat()) {
                const int fleeChance = calculateFleeChance();
                std::cout<<"You failed to escape! Escape chance: " << fleeChance << "%\n";
                break;
            }
            playerFled = true;
            std::cout << "Escaped from combat!\n";
        }
        break;
        case 5: {
            std::cout<<"Analyzing enemies...\n";
            for (const auto & enemie : enemies) {
                if (const auto* enemy = dynamic_cast<const Enemy *>(enemie.get())) {
                    std::cout<<enemy->getDescription();
                }
            }
        } break;
        default:
            std::cout<<"Invalid action!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
    }
}

void GameManager::enemyTurn(Character &enemy) {
    std::cout<< "\n" << enemy.getName() << "'s turn:\n";
    //extremely simple enemy AI
    if (randomChance(20) && dynamic_cast<Enemy*>(&enemy)) {
        enemy.specialAbility();
    } else {
        enemy.attack(*player);
    }

    //Check if the player died
    if (!player->isAlive()) {
        std::cout<<player->getName() << " has been defeated!\n";
    }
}

void GameManager::shopPhase() const {
    std::cout<< "\n=== SHOP ===\n";
    std::cout<< "You find a mysterious shopkeeper on this floor.\n";
    std::cout<< "???: 'Ello there, stranger. What're ya buyin'?\n";

    Shop& shop = Shop::getInstance();
    shop.displayItems();

    int choice=0;
    while (choice!=2) {
        std::cout<<"\n1. Buy item \n";
        std::cout<<"\n2. Leave shop \n";
        std::cout<<"Choice: ";
        std::cin>>choice;

        if (choice==1) {
            std::cout<< "Enter item name (or 'list' to see items, or 'exit' to exit item display): ";
            std::string itemName;
            std::cin>>itemName;

            if (itemName == "list") {
                shop.displayItems();
            } else if (itemName == "exit") {
                return;
            } else {
                try {
                    if (shop.buyItem(*player, itemName)) {
                        std::cout<<"???: Hehehe! Thank ye!\n";
                    }
                } catch (const InventoryException& e) {
                    std::cout<< e.what() << std::endl;
                }
            }
        } else {
            std::cout << "???: Come again another day, stranger!";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
}
void GameManager::inventoryPhase() const {
    player->showInventory();
    int choice = 0;
    while (choice!=2) {
        std::cout<<"\n1. Use item \n";
        std::cout<<"\n2. Close inventory \n";
        std::cout<<"Choice: ";
        std::cin>>choice;
        if (choice==1) {
           try{ useItem(); } catch (const InventoryException &e) { std::cout<<e.what() ;}
        }
        else break;
    }
}
void GameManager::useItem() const {
    //Very simplified, adding more items later maybe, project is alrd complicated enough lol
    std::cout<<"Enter item name to use: ";
    std::string itemName;
    std::cin>>itemName;
    if (!player->hasItem(itemName)) {
        std::cout << "You don't have '" << itemName << "' in your inventory!\n";
        return;
    }
    try {
        Shop::applyItemEffect(*player,itemName);
        player->removeItem(itemName);
    } catch (InventoryException &e ) {
        std::cout<<e.what()<<std::endl;
    }
}

void GameManager::generateEnemies() {
    enemies.clear();
    if (currentFloor%10==0) {
        std::cout<<"\nBeware...a terrible presence lurks on this floor.\n";
        auto boss = EnemyFactory::createBoss(currentFloor);
        enemies.push_back(std::move(boss));

        const int minionCount = randomInt(1,2);
        for (int i=0; i < minionCount; i++) {
            enemies.push_back(EnemyFactory::createRandomEnemy(currentFloor));
        }
    } else {
        auto newEnemies = EnemyFactory::createEnemyGroup(currentFloor,1,3);
        enemies=std::move(newEnemies);
    }
    std::cout<<"Encountered " << enemies.size() << " enemy(ies)!\n";
}

void GameManager::handleCombatVictory() {
    std::cout<<"\n=== VICTORY! ===\n";
    //Experience reward
    const int expGained = currentFloor*50;
    player->gainExperience(expGained);
    std::cout<<"Gained "<< expGained << " experience!\n";
    if (player->getExp()>=player->getExpToNext()) {
        handleLevelUp();
    }
    if (randomChance(30)) {
        //item drop
        switch (randomInt(1,4)) {
            case 1: std::cout<<"Found a health potion!\n";
                player->addItem("HealthPotion"); break;
            case 2: std::cout<<"Found an attack potion!\n";
                player->addItem("AttackPotion"); break;
            case 3: std::cout<<"Found a defense potion!\n";
                player->addItem("DefensePotion"); break;
            case 4: std::cout<<"Found a speed potion!\n";
                player->addItem("SpeedPotion"); break;
            default: std::cout<<"Found...nothing?\n"; break;
        }
    }
}

void GameManager::handleFloorCompletion() {
    std::cout<<"\n=== FLOOR " << currentFloor << " CLEARED! ===\n";
    floorsCleared++;
    currentFloor++;
    hasFought = false; // prevents skipping!
    playerFled = false; // cowardice is reset!
    if (currentFloor<=totalFloors) {
        std::cout<<"Advancing to Floor " << currentFloor << "...\n";
        //tiny heal between floors
        const int healAmount = player->getMaxHealth()*static_cast<int>(0.10);
        player->setHealth(player->getHealth() + healAmount);
        std::cout<<"Recovered " << healAmount << " health while moving.\n";

        //level up chance
        if (randomChance(25)) {
            std::cout<<"You feel stronger!\n";
            player->levelUp();
        }
    }
}

void GameManager::processPlayerDeath() {
    std::cout<< "\n=== GAME OVER ===\n";
    std::cout<< "You were defeated on Floor " << currentFloor << "\n";
    std::cout<<"\n=== FINAL STATS === \n";
    std::cout<<"Floors cleared: " << floorsCleared << "\n";
    std::cout<<"Enemies defeated: " << enemiesDefeated << "\n";
    std::cout<<"Gold collected: " << goldCollected << "\n";

    gameRunning = false;
    player.reset();
}
void GameManager::handleVictory() {
    std::cout<< "\n=== CONGRATULATIONS! ===\n";
    std::cout<<"You have conquered all " << totalFloors << " floors!\n";
    std::cout<<"\n=== FINAL STATS === \n";
    std::cout<<"Floors cleared: " << floorsCleared << "\n";
    std::cout<<"Enemies defeated: " << enemiesDefeated << "\n";
    std::cout<<"Gold collected: " << goldCollected << "\n";

    gameOver(true);
}

void GameManager::handleLevelUp() const {
    // used just in case we have multiple levelups
    while (player->getExp()>=player->getExpToNext()) {
        player->setExp(player->getExp()-player->getExpToNext());
        if (player->getExp()<0) player->setExp(0);
        player->levelUp();
    }
}

void GameManager::displayFloorInfo() const {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "FLOOR: " << currentFloor << "/" << totalFloors << "\n";
    std::cout << std::string(50, '=') << "\n";
    displayPlayerStatus();
}

void GameManager::displayPlayerStatus() const {
    if (player) {
        std::cout<<*player;
        std::cout << "Gold: " << player->getGold() << "\n";
    }
}

void GameManager::displayCombatMenu() {
    std::cout<< "\n=== YOUR TURN ===\n";
    std::cout<<"Choose your action: ";
    std::cout<<"1. Attack\n";
    std::cout<<"2. Special ability\n";
    std::cout<<"3. Use item\n";
    std::cout<<"4. Flee\n";
    std::cout<<"5. Analyze\n";
}

int GameManager::calculateFleeChance() const {
    //flee chance: 50% + (player speed - avg enemy speed)
    int fleeChance = 50;
    if (!enemies.empty()) {
        int totalEnemySpeed=0;
        for (const auto& enemy : enemies) {
            totalEnemySpeed+= enemy->getSpeed();
        }
        const int avgEnemySpeed=static_cast<int>(totalEnemySpeed / enemies.size());
        fleeChance+=(player->getSpeed()-avgEnemySpeed);
    }
    fleeChance = std::max(10,std::min(90,fleeChance)); // no guarantee, 10-90%
    return fleeChance;
}

bool GameManager::fleeCombat() {
    const int fleeChance = calculateFleeChance();
    return randomChance(fleeChance);
}
//Random utilities
int GameManager::randomInt(const int min, const int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(rng);
}
bool GameManager::randomChance(const int percentage) {
    return randomInt(1,100) <= percentage;
}

void GameManager::gameOver(const bool won) {
    if (won) {
        std::cout<<"Congratulations on your win. The game has ended.\n";
        gameRunning = false;
    } else {
        std::cout<<"You are welcome to try again. The game is over.\n";
        gameRunning = false;
    }
}