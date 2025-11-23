#include "text.hpp"

#include <format>

namespace utils {

std::string WrapWithWildcards(const std::string& search_term) { return std::format("%{}%", search_term); }

}  // namespace utils
