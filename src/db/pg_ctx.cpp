#include "pg_ctx.hpp"

#include <string>

#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace db {

namespace {

static const std::string kDatabaseName{"tidy_pg"};

}  // namespace

PgCtx::PgCtx(const components::ComponentContext& context)
    : cluster_(context.FindComponent<components::Postgres>(kDatabaseName).GetCluster()) {}

PgCtx::~PgCtx() = default;

storages::postgres::Transaction PgCtx::GetRwTrx() const {
    return cluster_->Begin(
        storages::postgres::ClusterHostType::kMaster,
        storages::postgres::TransactionOptions{storages::postgres::TransactionOptions::kReadWrite}
    );
}

storages::postgres::Transaction PgCtx::GetRoTrx() const {
    return cluster_->Begin(
        storages::postgres::ClusterHostType::kSlave,
        storages::postgres::TransactionOptions{storages::postgres::TransactionOptions::kReadOnly}
    );
}

}  // namespace db
