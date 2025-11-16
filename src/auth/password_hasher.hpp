#pragma once

#include <string>

namespace auth {

class PasswordHasher {
public:
    static std::string Hash(const std::string& password);

    static bool Verify(const std::string& password, const std::string& stored);
};

}  // namespace auth
