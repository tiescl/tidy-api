#pragma once

#include <string_view>

#include <userver/cache/base_postgres_cache.hpp>
#include <userver/server/auth/user_auth_info.hpp>
#include <userver/storages/postgres/query.hpp>

#include <caches/models/user_tokens/container.hpp>

#include <db/dto/auth/views.hpp>
#include <tidy_api/sql_queries.hpp>

namespace caches {

namespace detail {

struct UserTokensPolicy {
    static constexpr std::string_view kName = "user-tokens-pg-cache";

    using ValueType = db::dto::auth::UserTokenExt;

    using CacheContainer = caches::models::user_tokens::Container;

    static constexpr auto kKeyMember = &ValueType::token;

    static constexpr const char* kUpdatedField = "increment";
    using UpdatedFieldType = int64_t;

    static storages::postgres::Query GetQuery() { return tidy_api::sql::kSelectUserTokens; }

    static int64_t GetLastKnownUpdated(const CacheContainer& container) { return container.GetLatestRevision(); }
};

}  // namespace detail

using UserTokensCache = components::PostgreCache<detail::UserTokensPolicy>;

}  // namespace caches
