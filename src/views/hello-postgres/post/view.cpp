#include "view.hpp"

#include <utils/greeting.hpp>

#include <userver/storages/postgres/component.hpp>

namespace handlers::hello_postgres::post {

Handler::Handler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& component_context
)
    : HttpHandlerBase(config, component_context),
      pg_cluster_(component_context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

std::string Handler::HandleRequest(
    userver::server::http::HttpRequest& request,
    [[maybe_unused]] userver::server::request::RequestContext& context
) const {
    const auto& name = request.GetArg("name");
    auto user_type = utils::UserType::kFirstTime;

    if (!name.empty()) {
        const auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO hello_schema.users(name, count) VALUES($1, 1) "
            "ON CONFLICT (name) "
            "DO UPDATE SET count = users.count + 1 "
            "RETURNING users.count",
            name
        );

        if (result.AsSingleRow<int>() > 1) {
            user_type = utils::UserType::kKnown;
        }
    }

    return utils::SayHelloTo(name, user_type);
}

}  // namespace handlers::hello_postgres::post
