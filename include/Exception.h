#pragma once
#include <string>
#include <utility>

                                                            /// !!! IERARHIE DE EXCEPTII !!! ///
class Exception : public std::exception {
protected:
    std::string message;
public:
    explicit Exception(std::string msg) : message(std::move(msg)) {}
    ~Exception() override = default;
    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};

//Combat exceptions


class CombatException : public Exception {
public:
    explicit CombatException(std::string msg) : Exception("[COMBAT] "+std::move(msg)) {}
};

class InvalidTargetException : public CombatException {
public:
    InvalidTargetException(const int targetIndex, const int maxTargets)
        : CombatException("Invalid target#" + std::to_string(targetIndex) + " (max: " + std::to_string(maxTargets) + ")") {}
};

class PlayerDeathException : public CombatException {
public:
    PlayerDeathException() : CombatException("Player has died!") {}
};

class FleeFailException : public CombatException {
public:
    explicit FleeFailException(const int chance) : CombatException("Flee failed! (" + std::to_string(chance) + "% chance)") {}
};

//Inventory exceptions
class InventoryException : public Exception {
public:
    explicit InventoryException(std::string msg)
        : Exception("[INVENTORY] "+std::move(msg)) {}
};

class ItemNotFoundException : public InventoryException {
public:
    explicit ItemNotFoundException(const std::string &itemName)
        : InventoryException("Item not found: " + itemName) {}
};

class InsufficientFundsException : public InventoryException {
public:
    explicit InsufficientFundsException(const int needed, const int have)
        : InventoryException("Need " + std::to_string(needed) + " gold, but only have " + std::to_string(have)) {}
};

class InventoryFullException : public InventoryException {
public:
    explicit InventoryFullException(const int maxCapacity)
        : InventoryException("Inventory full! Max capacity: " + std::to_string(maxCapacity)) {}
};

//Game state exception
class GameStateException : public Exception {
public:
    explicit GameStateException(std::string msg)
        : Exception("[GAME STATE] "+std::move(msg)) {}
};