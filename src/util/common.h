#ifndef FSM_SRC_UTIL_COMMON_H
#define FSM_SRC_UTIL_COMMON_H

#include <string>
#include <chrono>
#include <iomanip>  // std::put_time
#include <sstream> 
#include <cstdlib>  // std::getenv()
#include <tuple>

namespace fsm::util {

/**
 * @param T
 * @param Parameter to avoid a interchangebility between to types having the same T, in other
 *        words it will make an instantiation in a unique type. It is not used though, this
 *        is called Phantom type.
 * @see <a href="https://www.fluentcpp.com/2016/12/08/strong-types-for-strong-interfaces/">Fluent C++ - Strong types for interfaces</a>
 */
template <typename T, typename Parameter>
class NamedType {
public:
    explicit NamedType(T const& value) : value_(value) {}
    explicit NamedType(T&& value) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const { return value_; }
private:
    T value_;
};

/**
 * @brief Returns a environment variable value
 * @param[in] name environment variable name. Must not be empty.
 * @return A tuple having the variable value in the first parameter. In case of variable
 *         not found the second tuple's value will contain error message
 */
inline std::tuple<std::string, std::string>
get_environment_variable(std::string const& name) noexcept {
    if (name.size() == 0) {
        return std::make_tuple(std::string{}, "invalid environment variable - empty!");
    }
    auto value = std::getenv(name.data()); 
    if (value != nullptr) {
        return std::make_tuple(std::string{value}, "");
    }
    std::string error = "environment variable[" + std::string{name.data()} + "] not found";
    return std::make_tuple(std::string{}, error);
}


//--------------------------------------------------------------------------------
namespace literals {

/**
 * @brief
 */
constexpr std::chrono::milliseconds operator ""_ms(unsigned long long millisecs) {
    return std::chrono::milliseconds{millisecs};
}

/**
 * Given a date and time format returns the current timestamp
 * @param lit - The time format for the timestamp
 *              %F ==> %Y-%m-%d
 *              %T ==> %H:%M:%S
 * @see  https://en.cppreference.com/w/cpp/io/manip/put_time
 */
inline std::string operator ""_timestamp(const char * lit, size_t) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, lit);
    return oss.str();
}

} // namespace literals


}   // namespace fsm::util 

#endif // FSM_SRC_UTIL_COMMON_H
