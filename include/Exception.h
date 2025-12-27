#pragma once
#include <string>

                                                            /// !!! IERARHIE DE EXCEPTII !!! ///
class Exception : public std::exception {
protected:
    std::string message;
public:
    explicit Exception(std::string msg);
    ~Exception() override = default;
    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};

class InventoryException : public Exception {
public:
    explicit InventoryException(std::string msg);
};
