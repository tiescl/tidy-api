#pragma once

#include <concepts>
#include <type_traits>
#include <utility>
#include <variant>

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/to.hpp>
#include <userver/formats/serialize/to.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>

namespace utils {

template <
    class Derived,
    typename Request,
    typename Response,
    typename SuccessResponse,
    Request (*ParseFn)(formats::json::Value, formats::parse::To<Request>),
    formats::json::Value (*SerializeFn)(const SuccessResponse&, formats::serialize::To<formats::json::Value>)>
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

        return std::visit(
            [](auto&& r) -> formats::json::Value {
                using T = std::decay_t<decltype(r)>;
                if constexpr (std::same_as<T, SuccessResponse>) {
                    return SerializeFn(r, formats::serialize::To<formats::json::Value>());
                } else {
                    return formats::json::MakeObject();
                }
            },
            res
        );
    }
};

}  // namespace utils
