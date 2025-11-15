#pragma once

#include <utility>

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/to.hpp>
#include <userver/formats/serialize/to.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>

namespace utils {

template <
    typename Derived,
    typename Request,
    typename Response,
    Request (*ParseFn)(formats::json::Value, formats::parse::To<Request>),
    formats::json::Value (*SerializeFn)(const Response&, formats::serialize::To<formats::json::Value>)>
class TypedJsonHandler : public server::handlers::HttpHandlerJsonBase {
public:
    using HttpHandlerJsonBase::HttpHandlerJsonBase;

    formats::json::Value HandleRequestJsonThrow(
        const server::http::HttpRequest& request,
        const formats::json::Value& request_body,
        server::request::RequestContext& context
    ) const override {
        Request body = ParseFn(request_body, formats::parse::To<Request>());

        Response res = static_cast<const Derived*>(this)->Handle(std::move(body), request, context);

        return SerializeFn(res, formats::serialize::To<formats::json::Value>());
    }
};

}  // namespace utils
