#pragma once

#include <string>
#include <string_view>

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include <db/pg_ctx.hpp>

namespace handlers::hello::get {

class Handler final : public server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-hello-get";

    Handler(const components::ComponentConfig& config, const components::ComponentContext& context);

    std::string HandleRequest(server::http::HttpRequest&, server::request::RequestContext&) const override;

private:
    const db::PgCtx pg_;
};

}  // namespace handlers::hello::get
