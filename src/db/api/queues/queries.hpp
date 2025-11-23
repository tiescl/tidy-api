#pragma once

#include <optional>
#include <string>

#include <boost/uuid/uuid.hpp>

#include <db/dto/queues/views.hpp>
#include <db/pg_ctx.hpp>

namespace db::api::queues {

std::optional<boost::uuids::uuid> CreateQueue(const db::PgCtx& pg, const dto::queues::Queue& queue);

std::string DeleteQueue(const db::PgCtx& pg, const boost::uuids::uuid& queue_id, const boost::uuids::uuid& owner_id);

}  // namespace db::api::queues
