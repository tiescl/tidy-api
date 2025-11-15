#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/congestion_control/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>

#include <userver/storages/postgres/component.hpp>

#include <userver/utils/daemon_run.hpp>

#include <views/v1/auth/register/post/view.hpp>

namespace {

components::ComponentList GenerateServiceHandlersList() {
    return components::ComponentList().Append<handlers::v1_auth_register::post::View>();
}

}  // namespace

int main(int argc, char* argv[]) {
    auto component_list = components::MinimalServerComponentList()
                              .Append<server::handlers::Ping>()
                              .Append<components::HttpClient>()
                              .Append<clients::dns::Component>()
                              .Append<components::TestsuiteSupport>()
                              .Append<congestion_control::Component>()
                              .Append<server::handlers::TestsControl>()
                              .Append<components::Postgres>("tidy_pg")
                              .AppendComponentList(GenerateServiceHandlersList());

    return utils::DaemonMain(argc, argv, component_list);
}
