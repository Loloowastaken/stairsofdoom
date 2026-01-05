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

class InventoryException : public Exception {
public:
    explicit InventoryException(std::string msg)
        : Exception("[INVENTORY] "+std::move(msg)) {}
};

