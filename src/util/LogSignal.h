
#ifndef FSM_SRC_UTIL_LOG_SIGNAL_H
#define FSM_SRC_UTIL_LOG_SIGNAL_H

#include <cstdint>
#include <sstream>
#include <functional>
#include <cassert>
#include <iostream>
#include <tuple>

#include "common.h"

namespace fsm::util::log {

enum class LogLevel : std::uint8_t { Off   = 0
                                   , Fatal = 1 << 1
                                   , Error = 1 << 2
                                   , Warn  = 1 << 3
                                   , Info  = 1 << 4
                                   , Debug = 1 << 5
                                   , Trace = 1 << 6
                                   , All   = 1 << 7 };

constexpr char const* const tag(LogLevel type) {
    if (LogLevel::Off == type) return "";
    if (LogLevel::Fatal == type) return "FATAL";
    if (LogLevel::Error == type) return "ERROR";
    if (LogLevel::Warn == type) return "WARN";
    if (LogLevel::Info == type) return "INFO";
    if (LogLevel::Debug == type) return "DEBUG";
    if (LogLevel::Trace == type) return "TRACE";
    return "UNAMED";
}

/**
 * @brief The LogSignal struct
 */
struct LogSignal final {
    LogLevel      type{LogLevel::Info};
    std::uint32_t line{0u};
    std::uint32_t thread_id{0u};
    char const*   emitter{nullptr};
    std::string   funcname{};
    std::string   payload{};
    std::string   timestamp{};
    std::string   type_str{};
    std::string   msg_ready_to_print{};
};

inline std::string signal_to_log_msg(LogSignal const& p_signal) {
    std::ostringstream oss;
    oss << "[" << p_signal.timestamp << "] "
        << p_signal.type_str << " <"
        << p_signal.emitter << "::"
        << p_signal.funcname << ":"
        << p_signal.line << "> "
        << p_signal.payload << std::endl;
    return oss.str();
}


using log_func_t = std::function<void(LogSignal)>;
using types::literals::operator""_timestamp;

/**
 * @brief The LogSignalDispatcher struct
 */
struct LogSignalDispatcher final {
    LogLevel level;
    log_func_t log = nullptr;


    template <typename... Args>
    void emit_log(LogLevel p_type
                 , char const* p_emitter
                 , char const* pm_funcname
                 , std::uint32_t p_line
                 , Args&&... p_args) const & /* can only be called by a lvalue */ {
        assert(log != nullptr);
        LogSignal signal;
        signal.emitter              = p_emitter;
        signal.type                 = p_type;
        signal.line                 = p_line;
        signal.funcname             = pm_funcname;
        signal.timestamp            = "%F %T"_timestamp;
        signal.type_str             = tag(p_type);
        std::ostringstream oss;
        ((oss << " " << p_args), ...);
        signal.payload              = oss.str();
        signal.msg_ready_to_print   = signal_to_log_msg(signal);
        log(std::move(signal));
    }

    void emit_log_scope(char const* p_emitter
                 , char const* pm_funcname
                 , std::uint32_t p_line
                 , std::string const& p_payload) const & /* can only be called by a lvalue */ {
        assert(log != nullptr);
        LogSignal signal;
        signal.emitter              = p_emitter;
        signal.type                 = LogLevel::Trace;
        signal.line                 = p_line;
        signal.funcname             = pm_funcname;
        signal.timestamp            = "%F %T"_timestamp;
        signal.type_str             = tag(LogLevel::Trace);
        signal.payload              = p_payload;
        signal.msg_ready_to_print   = signal_to_log_msg(signal);
        log(std::move(signal));
    }
};

struct ScopeLog final {
    LogSignalDispatcher const* m_dispatcher{nullptr};
    char const*          m_emitter;
    char const*          m_funcname;
    std::uint32_t        m_line;
    std::string          m_msg_enter;
    std::string          m_msg_leave;
    
    template <typename... Args>
    explicit ScopeLog(LogSignalDispatcher const* const p_dispatcher
                     , char const* p_emitter
                     , char const* p_funcname
                     , std::uint32_t p_line
                     , Args&&... p_args) 
        : m_dispatcher{p_dispatcher}
        , m_emitter{p_emitter}
        , m_funcname{p_funcname}
        , m_line{p_line} {
        std::ostringstream oss;
        ((oss << " " << p_args), ...);
        m_msg_enter = "----> " + oss.str();
        m_msg_leave = "<---- " + oss.str();
        m_dispatcher->emit_log_scope(m_emitter, m_funcname, m_line, m_msg_enter);
    }

    ~ScopeLog() {
        m_dispatcher->emit_log_scope(m_emitter, m_funcname, m_line, m_msg_leave);
    }

};

}  // namespace fsm::util::log 

#endif  //FSM_SRC_UTIL_LOG_SIGNAL_H
