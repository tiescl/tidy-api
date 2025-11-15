#pragma once

#include <userver/storages/postgres/io/enum_types.hpp>

#include <defs/auth.hpp>

namespace storages::postgres::io {

template <>
struct CppToUserPg<defs::auth::UserRole> : EnumMappingBase<defs::auth::UserRole> {
    static constexpr DBTypeName postgres_name{"tidy", "user_role"};
    static constexpr EnumeratorList enumerators{
        {EnumType::kPending, "pending"},
        {EnumType::kAdmin, "admin"},
        {EnumType::kManager, "manager"},
        {EnumType::kRecruiter, "recruiter"},
        {EnumType::kDeveloper, "developer"},
    };
};

}  // namespace storages::postgres::io
