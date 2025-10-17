#pragma once

#include <string>
#include <string_view>

#include <userver/server/handlers/http_handler_base.hpp>

namespace handlers::hello::get {

class Handler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-hello-get";

    using HttpHandlerBase::HttpHandlerBase;

    std::string HandleRequest(userver::server::http::HttpRequest&, userver::server::request::RequestContext&)
        const override;
};

}  // namespace handlers::hello::get
