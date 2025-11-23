#pragma once

#include <optional>

#include <boost/uuid/uuid.hpp>

#include <db/dto/queues/views.hpp>
#include <db/pg_ctx.hpp>

namespace db::api::queues {

std::optional<boost::uuids::uuid> CreateQueue(const db::PgCtx& pg, const dto::queues::Queue& queue);

}  // namespace db::api::queues
