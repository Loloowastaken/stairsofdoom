#include <Exception.h>
Exception::Exception(std::string msg) : message(std::move(msg)) {}
InventoryException::InventoryException(std::string msg)
        : Exception("Inventory Error: " + std::move(msg)) {}
