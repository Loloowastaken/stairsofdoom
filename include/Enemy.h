#pragma once
#include <Character.h>
#include <string>
/// CLASA 'ENEMY' - DERIVATA LUI 'CHARACTER' ///
class Enemy : public Character {
public:
    enum class EnemyType {GOBLIN, SKELETON, ORC, SLIME}; // Vom avea mai multe feluri de inamici
    enum class Difficulty {EASY, MEDIUM, HARD, BOSS}; // Pentru a calcula dinamic atributele inamicului si rewardul
protected:
    EnemyType type;
    Difficulty difficulty;
    int baseReward;
    std::string description;
    bool abilityUsed;
public:
                                                            // CONSTRUCTORI //
    //Constructor default/supraincarcat
    Enemy(const std::string& name, EnemyType type, Difficulty diff, int level);
    // Constructor copy
    Enemy(const Enemy& other) = default;

    // Operator copy
    Enemy& operator=(const Enemy& other);

                                                            // DESTRUCTORI //
    ~Enemy() override;
                                                            // METODE VIRTUALE SUPRASCRISE //
    void attack(Character& target) override;
    void specialAbility() override;
                                                            // METODE HELPER //
    //Calcularea atributelor
    static int calculateHealth (EnemyType type, Difficulty diff, int level);
    static int calculateAttack (EnemyType type, Difficulty diff, int level);
    static int calculateDefense (EnemyType type, Difficulty diff, int level);
    static int calculateSpeed (EnemyType type, Difficulty diff, int level);
    static int calculateReward(EnemyType type, Difficulty diff, int level);
                                                            // GETTERI SI SETTERI //
    [[nodiscard]] EnemyType getType() const { return type; }
    [[nodiscard]] const std::string & getDescription() const { return description; }
    [[nodiscard]] Difficulty getDiff() const { return difficulty; }
    void setDescription();                                                           // ABILITATI INDIVIDUALE //
private:
    void goblinSpecial();
    void skeletonSpecial();
    void orcSpecial();
    void slimeSpecial();
};