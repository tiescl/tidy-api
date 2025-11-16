#pragma once

#include <string_view>
#include <unordered_map>

#include <userver/cache/base_postgres_cache.hpp>
#include <userver/crypto/algorithm.hpp>
#include <userver/server/auth/user_auth_info.hpp>
#include <userver/storages/postgres/io/chrono.hpp>

#include <db/dto/auth/views.hpp>
#include <tidy_api/sql_queries.hpp>

namespace caches {

namespace detail {

struct UserTokensPolicy {
    static constexpr std::string_view kName = "user-tokens-pg-cache";

    using KeyType = server::auth::UserAuthInfo::Ticket;
    using ValueType = db::dto::auth::UserToken;
    using CacheContainer =
        std::unordered_map<KeyType, ValueType, std::hash<KeyType>, crypto::algorithm::StringsEqualConstTimeComparator>;

    static constexpr auto kKeyMember = &ValueType::token;
    static constexpr const char* kUpdatedField = "updated_at";
    using UpdatedFieldType = storages::postgres::TimePointTz;

    static constexpr const char* kWhere = "expires_at >= NOW()";
    static storages::postgres::Query GetQuery() { return tidy_api::sql::kSelectUserTokens; }
};

}  // namespace detail

using UserTokensCache = components::PostgreCache<detail::UserTokensPolicy>;

}  // namespace caches
