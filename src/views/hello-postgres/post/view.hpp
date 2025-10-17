#pragma once

#include <string>
#include <string_view>

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace handlers::hello_postgres::post {

class Handler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-hello_postgres-post";

    Handler(const userver::components::ComponentConfig&, const userver::components::ComponentContext&);

    std::string HandleRequest(userver::server::http::HttpRequest&, userver::server::request::RequestContext&)
        const override;

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace handlers::hello_postgres::post
