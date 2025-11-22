#include "container.hpp"

#include <algorithm>

#include <userver/logging/log.hpp>

namespace caches::models::user_tokens {

void Container::insert_or_assign(KeyType id, ValueType&& token) {
    if (token.expires_at < storages::postgres::Now()) {
        Delete(id);
    } else {
        try {
            Upsert(id, std::move(token));
        } catch (const std::exception& exc) {
            LOG_ERROR() << "Unable to process token for user " << token.user_id << ": " << exc;
            Delete(id);
        }
    }

    revision_ = std::max(revision_, token.increment);
}

void Container::Delete(KeyType id) noexcept {
    const auto it = user_tokens_by_ticket_.find(id);
    if (it == user_tokens_by_ticket_.end()) {
        return;
    }
    user_tokens_by_ticket_.erase(it);
}

void Container::Upsert(KeyType id, ValueType&& token) {
    user_tokens_by_ticket_.insert_or_assign(id, std::make_shared<const ValueType>(std::move(token)));
}

size_t Container::size() const { return user_tokens_by_ticket_.size(); }

int64_t Container::GetLatestRevision() const { return revision_; }

ValuePtr Container::GetUserInfoByToken(KeyType id) const {
    if (const auto it = user_tokens_by_ticket_.find(id); it != user_tokens_by_ticket_.end()) {
        return it->second;
    }
    return nullptr;
}

}  // namespace caches::models::user_tokens
