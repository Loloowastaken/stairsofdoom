#pragma once
#include <Character.h>
#include <ItemContainer.h>
#include <string>
/// CLASA 'PLAYER' - DERIVATA LUI 'CHARACTER' ///
class Player : public Character {
    protected:
    int gold;
    int experience;
    int experienceToNext;
    ItemContainer<std::string> inventory;
    int heroicStrikeCooldown;
    int shieldBashCooldown;
    int secondWindUses;
public:
                                                            // CONSTRUCTORI (apeland clasa de baza) //
    //Constructor default
    explicit Player(const std::string &name);
    //Constructor supraincarcat
    Player(const std::string &name, int level);
    //Constructor copy
    Player (const Player& other) = default;
                                                            // OPERATORI //
    // Operator copy
    Player &operator=(const Player& other);
                                                            // DESTRUCTORI //
    ~Player() override;
                                                            // METODE VIRTUALE SUPRASCRISE //
    void attack(Character &target) override;
    void specialAbility() override;
                                                            // METODE SPECIFICE PLAYER //
    void gainExperience(int xp);
    void levelUp();
    void addGold(int amount);
    void spendGold(int amount);
    void addItem(const std::string& item);
    void removeItem(const std::string& item);
    void showInventory() const;

    [[nodiscard]] bool hasItem(const std::string & string) const;

    // ABILITATI //
private:
    bool heroicStrike();
    bool shieldBash();
    void secondWind();

                                                            // GETTERI //
public:
    [[nodiscard]] int getGold() const { return gold; }
    [[nodiscard]] int getExp() const { return experience; }
    [[nodiscard]] int getExpToNext() const { return experienceToNext; }
    void setGold(const int setgold) { gold = setgold; }
    void setExp(const int setexp) { experience = setexp; }
    void setUsesRemaining(const int setuses) { secondWindUses = setuses; }
};