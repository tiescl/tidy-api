#pragma once

#include <userver/components/component.hpp>
#include <userver/logging/log.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/options.hpp>
#include <userver/storages/postgres/result_set.hpp>
#include <userver/storages/postgres/transaction.hpp>
#include <userver/tracing/span.hpp>

namespace db {

class PgCtx {
public:
    PgCtx(const components::ComponentContext& context);
    ~PgCtx();

    storages::postgres::ClusterPtr GetCluster() const;

    storages::postgres::Transaction GetRwTrx() const;
    storages::postgres::Transaction GetRoTrx() const;

    template <typename... Args>
    storages::postgres::ResultSet MakeRequest(
        const storages::postgres::ClusterHostType host_type,
        const storages::postgres::Query& request,
        const Args&... args
    ) const {
        const auto name = request.GetOptionalName();
        const auto label = name ? name.value().GetUnderlying() : "pg_statement";
        [[maybe_unused]] auto scope_time = tracing::Span::CurrentSpan().CreateScopeTime(label);

        try {
            return cluster_->Execute(host_type, request, args...);
        } catch (const storages::postgres::Error& err) {
            LOG_ERROR() << "Got exception from postgres (" << label << "):" << err;
            throw;
        }
    }

    template <typename... Args>
    storages::postgres::ResultSet MakeRequest(
        storages::postgres::Transaction& trx,
        const storages::postgres::Query& request,
        const Args&... args
    ) const {
        const auto name = request.GetOptionalName();
        const auto label = name ? name.value().GetUnderlying() : "pg_statement";
        [[maybe_unused]] auto scope_time = tracing::Span::CurrentSpan().CreateScopeTime(label);

        try {
            return trx.Execute(request, args...);
        } catch (const storages::postgres::Error& err) {
            LOG_ERROR() << "Got exception from postgres (" << label << "):" << err;
            throw;
        }
    }

    template <typename... Args>
    storages::postgres::ResultSet MakeRoRequest(const storages::postgres::Query& request, const Args&... args) const {
        return MakeRequest(storages::postgres::ClusterHostType::kSlave, request, args...);
    }

    template <typename... Args>
    storages::postgres::ResultSet MakeRwRequest(const storages::postgres::Query& request, const Args&... args) const {
        return MakeRequest(storages::postgres::ClusterHostType::kMaster, request, args...);
    }

private:
    const storages::postgres::ClusterPtr cluster_;
};

}  // namespace db
