cmake_minimum_required(VERSION 3.12...3.31)
project(tidy-api CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(DownloadUserver)

find_package(
    userver
    COMPONENTS universal
               core
               postgresql
    QUIET
)
if(NOT userver_FOUND)
    download_userver()
endif()

userver_setup_environment()

# Common sources
include_directories(src)

userver_add_sql_library(
    ${PROJECT_NAME}_sql
    NAMESPACE
    tidy_api
    SOURCE_DIR
    src/db/sql
    OUTPUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}
    SQL_FILES
    *.sql
)

file(GLOB_RECURSE SCHEMAS ${CMAKE_CURRENT_SOURCE_DIR}/defs/*.yaml)
userver_target_generate_chaotic(
    ${PROJECT_NAME}_gen
    GENERATE_SERIALIZERS
    LAYOUT "/components/schemas/([^/]*)/=defs::{stem}::{0}"
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/src
    SCHEMAS ${SCHEMAS}
    RELATIVE_TO ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
    ${PROJECT_NAME}_objs OBJECT
    src/auth/auth_checker.cpp
    src/auth/cookie.cpp
    src/auth/password_hasher.cpp
    src/caches/models/user_tokens/container.cpp
    src/db/pg_ctx.cpp
    src/db/api/admin/queries.cpp
    src/db/api/auth/queries.cpp
    src/db/api/queues/queries.cpp
    src/db/api/users/queries.cpp
    src/views/admin/v1/users/del/view.cpp
    src/views/admin/v1/users/get/view.cpp
    src/views/admin/v1/users/role/post/view.cpp
    src/views/v1/auth/login/post/view.cpp
    src/views/v1/auth/logout/post/view.cpp
    src/views/v1/auth/register/post/view.cpp
    src/views/v1/queues/post/view.cpp
    src/views/v1/users/me/del/view.cpp
    src/views/v1/users/me/get/view.cpp
    src/views/v1/users/me/put/view.cpp
    src/views/v1/users/me/password/put/view.cpp
)
target_link_libraries(
    ${PROJECT_NAME}_objs
    PUBLIC userver::universal
           userver::core
           userver::postgresql
           ${PROJECT_NAME}_sql
           ${PROJECT_NAME}_gen
)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

# Functional testing
userver_testsuite_add_simple()

# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX "${PREFIX_PATH}")
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${PROJECT_NAME}
)
install(
    FILES ${CONFIGS_FILES}
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
)
