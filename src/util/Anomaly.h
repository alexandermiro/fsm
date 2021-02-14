#ifndef FSM_SRC_UTIL_ANOMALY_COMMON_H
#define FSM_SRC_UTIL_ANOMALY_COMMON_H

#include <stdexcept>

#define THROW_ANOMALY(...) throw fsm::except::create_anomaly("[ANOMALY]", __FILE__, ":", __LINE__, __VA_ARGS__);

namespace fsm::util::except { 

/**
 * @brief The Anomaly class
 */
class Anomaly final : public std::runtime_error {
public:
    explicit Anomaly(char const* msg) : std::runtime_error(msg) {}
    explicit Anomaly(std::string const& msg) : std::runtime_error(msg) {}
    ~Anomaly() noexcept {}
};

/**
 *
 */
template<typename Arg, typename... Args>
void join_anomaly_args(std::ostringstream& o, Arg&& arg, Args&&... args) {
    o << std::forward<Arg>(arg);
    ((o << ' ' << std::forward<Args>(args)), ...);
}

/**
 * @brief Join the anomaly arguments
 */
template<typename... Args>
Anomaly create_anomaly(Args&&... args) {
    std::ostringstream oss;
    join_anomaly_args(oss, args...);
    return Anomaly(oss.str());
}

}   // namespace fsm::util::except 

#endif // FSM_SRC_UTIL_ANOMALY_COMMON_H
