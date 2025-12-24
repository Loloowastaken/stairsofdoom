#include <Exception.h>
Exception::Exception(std::string msg) : message(std::move(msg)) {}
CombatException::CombatException(std::string msg)
        : Exception("Combat Error: " + std::move(msg)) {}
InventoryException::InventoryException(std::string msg)
        : Exception("Inventory Error: " + std::move(msg)) {}
PlayerDeathException::PlayerDeathException()
        : CombatException("Player has been defeated!") {}
