#include "password_hasher.hpp"

#include <vector>

#include <openssl/err.h>
#include <openssl/evp.h>

#include <userver/crypto/algorithm.hpp>
#include <userver/crypto/random.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/utils/encoding/hex.hpp>
#include <userver/utils/text.hpp>

namespace auth {

namespace {

static constexpr int kSaltLength{16};
static constexpr int kHashLength{32};
static constexpr int kIterations{600000};
static const std::string kSeparator{"$"};

}  // namespace

std::string PasswordHasher::Hash(const std::string& password) {
    unsigned char salt[kSaltLength];
    crypto::GenerateRandomBlock(utils::span(salt));

    unsigned char hash[kHashLength];
    if (!PKCS5_PBKDF2_HMAC(
            password.c_str(), password.length(), salt, kSaltLength, kIterations, EVP_sha256(), kHashLength, hash
        )) {
        throw server::handlers::InternalServerError();
    }

    return utils::text::Join(
        {utils::encoding::ToHex(salt, kSaltLength), utils::encoding::ToHex(hash, kHashLength)}, kSeparator
    );
}

bool PasswordHasher::Verify(const std::string& password, const std::string& stored) {
    std::vector<std::string> split_result = utils::text::Split(stored, kSeparator);
    if (split_result.size() != 2) return false;

    const std::string salt = utils::encoding::FromHex(split_result.at(0));
    const std::string expected_hash = utils::encoding::FromHex(split_result.at(1));

    if (salt.length() != kSaltLength || expected_hash.length() != kHashLength) {
        LOG_ERROR("broken salt/hash detected");
        return false;
    }

    unsigned char hash[kHashLength];
    if (!PKCS5_PBKDF2_HMAC(
            password.c_str(),
            password.length(),
            reinterpret_cast<const unsigned char*>(salt.c_str()),
            salt.length(),
            kIterations,
            EVP_sha256(),
            kHashLength,
            hash
        )) {
        throw server::handlers::InternalServerError();
    }

    return crypto::algorithm::AreStringsEqualConstTime(
        expected_hash, std::string_view(reinterpret_cast<const char*>(hash), kHashLength)
    );
}

}  // namespace auth
