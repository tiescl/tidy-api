#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include <boost/uuid/uuid.hpp>

#include <userver/crypto/algorithm.hpp>
#include <userver/server/auth/user_auth_info.hpp>
#include <userver/storages/postgres/io/chrono.hpp>

#include <db/dto/auth/views.hpp>

#include <defs/users.hpp>

namespace caches::models::user_tokens {

using KeyType = server::auth::UserAuthInfo::Ticket;
using ValueType = db::dto::auth::UserTokenExt;

using ValuePtr = std::shared_ptr<const ValueType>;
using ValuePtrMap =
    std::unordered_map<KeyType, ValuePtr, std::hash<KeyType>, crypto::algorithm::StringsEqualConstTimeComparator>;

class Container {
public:
    Container() = default;

    void insert_or_assign(KeyType id, ValueType&& token);

    size_t size() const;

    int64_t GetLatestRevision() const;

    ValuePtr GetUserInfoByToken(KeyType id) const;

private:
    void Delete(KeyType id) noexcept;
    void Upsert(KeyType id, ValueType&& token);

    ValuePtrMap user_tokens_by_ticket_{};
    int64_t revision_{0};
};

}  // namespace caches::models::user_tokens
