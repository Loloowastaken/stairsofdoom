#pragma once
#include <Enemy.h>
#include <string>
/// CLASA 'BOSS' - DERIVATA LUI 'ENEMY' ///
class Boss : public Enemy {
protected:
    std::string title;
    bool hasSpecialPhase; // la un anumit punct, bossul intra in phase 2 pentru a il distinge mai bine de un inamic normal
    int specialPhaseThreshold;
    int turnCounter; // abilitati phase-specific
public:
    // CONSTRUCTORI //
    Boss(const std::string& name, EnemyType type, Difficulty diff, int level,
         std::string title);
    // Constructor copy
    Boss(const Boss& other) = default;
    // Operatorul copy
    Boss& operator=(const Boss& other);
    // DESTRUCTOR //
    ~Boss() override;
    // METODE SUPRASCRISE //
    void takeDamage(int damage) override;
    void specialAbility() override;
    // METODE SPECIFICE BOSSULUI //
private:
    void enragedSpecial();
    void enterSpecialPhase();
public:
    // GETTERI //
    [[nodiscard]] const std::string& getTitle() const { return title; }
    [[nodiscard]] bool isInSpecialPhase() const { return hasSpecialPhase; }
};