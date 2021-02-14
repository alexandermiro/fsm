#ifndef FSM_SRC_UTIL_LOGGER_H
#define FSM_SRC_UTIL_LOGGER_H

#include <functional>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>       // std::packaged_task


#include "LogSignal.h"
#include "SyncStaticQueue.h"
#include "common.h"
#include "FileHelper.h"

namespace fsm::util::log {

using dispatcher_umap_t = std::unordered_map<std::string, LogSignalDispatcher*>;

using types::literals::operator""_ms;

/**
 * @brief The LoggerManager class
 */
class Logger final {

    /**
     * @brief The Persister class
     */
    class Persister final {
    public:
        /**
         * @brief Persister
         * @param path
         */
        explicit Persister(io::Path&& path) noexcept(false)
            : file_helper_(new io::FileHelper{std::move(path)}) {

            if (!initialize()) {
                THROW_ANOMALY("Unable to open the log file", file_helper_->filepath());
            }
        }

        ~Persister() {
            shutdown();
        }

        /**
         * @brief log
         * @param signal
         */
        void log(LogSignal&& signal) {
            if (msg_q_.push(std::move(signal))) {
                std::scoped_lock<std::mutex> lock{new_msg_mtx_};
                cv_wait_for_msg_.notify_one();
            } else {
                std::cerr << "ERROR: lost log message due to full queue\n";
            }
        }

        /**
         * @brief shutdown
         */
        void shutdown() {
            keep_running_ = false;
            if (thr_persist_msg_.joinable()) thr_persist_msg_.join();
        }

    private:
        /**
         * @brief initialize
         * @return
         */
        bool initialize() {
            if (!file_helper_->open_from_scratch()) return false;

            std::packaged_task<void()> task([this]() { persist(); });

            thr_persist_msg_ = std::thread(std::move(task));
            return true;
        }

        /**
         * @brief persist
         */
        void persist() {
            while (keep_running_) {
                std::unique_lock<std::mutex> lock{new_msg_mtx_};
                cv_wait_for_msg_.wait(lock, [&]() { return msg_q_.size() || keep_running_ == false; });
                lock.unlock();

                while (msg_q_.size()) {
                    if (auto const signal = msg_q_.pop(); !signal.msg_ready_to_print.empty()) {
                        file_helper_->write(signal.msg_ready_to_print);
                        file_helper_->flush();
                        continue;
                    }
                }
            }
        }


        std::atomic_bool                        keep_running_{true};
        std::condition_variable                 cv_wait_for_msg_;
        std::thread                             thr_persist_msg_;
        std::mutex                              new_msg_mtx_;
        std::unique_ptr<io::FileHelper>         file_helper_{nullptr};
        core::SyncStaticQueue<LogSignal, 1000>  msg_q_;
    };
public:
    /**
     * @brief instance
     * @return
     */
    static Logger* instance(std::string_view filepath = "") noexcept {
        std::call_once(Logger::instantiated_flag_, [&filepath]() {
            instance_.reset(new Logger{filepath});
        });
        return instance_.get();
    }

    /**
     * @brief Dctor
     */
    ~Logger() {
        if (persister_) {
            persister_->shutdown();
        }
    }

    /**
     * @brief is_ready
     * @return
     */
    bool is_ready() const {
        return is_ready_;
    }

    /**
     * @brief
     */
    template <typename T>
    void connect(T* inst) noexcept {
        std::string const e = inst->_log_emitter_name;
        if (dispatchers_.count(e) == 0) {
            inst->_log_signal_dispatcher.log = [this](LogSignal&& signal) { persister_->log(std::move(signal)); };
            dispatchers_.insert(std::make_pair(e, &inst->_log_signal_dispatcher));
        }
    }

    LogSignalDispatcher& dispatcher() {
        return internal_dispatcher_;
    }

    void shutdown() {
        persister_->shutdown();
    }

private:
    /**
     * @brief Ctor
     */
    explicit Logger(std::string_view filepath) noexcept {
        try {
            io::Path path{filepath};
            persister_ = std::make_unique<Persister>(std::move(path));
            is_ready_ = true;
            connect_me();
        } catch(except::Anomaly& e) {
            std::cerr << "\n" << e.what() << std::endl;
        }
    }
    Logger()                            = delete;
    Logger(Logger const&)               = delete;
    Logger& operator=(Logger const&)    = delete;
    Logger(Logger&&)                    = delete;
    Logger& operator=(Logger&&)         = delete;

    void connect_me() {
        internal_dispatcher_.log = [this](LogSignal&& signal) { persister_->log(std::move(signal)); };
        dispatchers_.insert(std::make_pair("Logger", &internal_dispatcher_));
    }

    //--------------------------------------------------------------------------------
    inline static std::once_flag           instantiated_flag_{};
    inline static std::unique_ptr<Logger>  instance_{nullptr};
    inline static LogSignalDispatcher      internal_dispatcher_{};
    std::unique_ptr<Persister>             persister_{nullptr};
    dispatcher_umap_t                      dispatchers_;
    bool                                   is_ready_{false};
};


}  // namespace fsm::util::log 


using logger_ptr_t = fsm::util::log::Logger*;

#ifdef DISABLE_LOG_SYSTEM 
    #define DECLARE_LOGGER_SUITE(emitter_name) 
    #define CONNECT_LOGGER_TO_THIS()
    #define LOG_HOT(...)
    #define LOG_DEBUG(...)
    #define LOG_INFO(...) 
    #define LOG_WARN(...) 
    #define LOG_FATAL(...) 
    #define LOG_SCOPE(...) 
#else // DEBUG
    #define DECLARE_LOGGER_SUITE(emitter_name)                                          \
        public:                                                                         \
            inline static constexpr char const * _log_emitter_name = #emitter_name;     \
            fsm::util::log::LogSignalDispatcher _log_signal_dispatcher;

    #define CONNECT_LOGGER_TO_THIS() fsm::util::log::Logger::instance()->connect(this);

    #define LOG_HOT(...) \
        fsm::util::log::Logger::instance()->dispatcher().emit_log(fsm::util::log::LogLevel::Debug, "Logger", __FUNCTION__, __LINE__, __VA_ARGS__);

    #define LOG_DEBUG(...) \
        _log_signal_dispatcher.emit_log(fsm::util::log::LogLevel::Debug, _log_emitter_name, __FUNCTION__, __LINE__, __VA_ARGS__);

    #define LOG_INFO(...) \
        _log_signal_dispatcher.emit_log(fsm::util::log::LogLevel::Info, _log_emitter_name, __FUNCTION__, __LINE__, __VA_ARGS__);

    #define LOG_WARN(...) \
        _log_signal_dispatcher.emit_log(fsm::util::log::LogLevel::Warn, _log_emitter_name, __FUNCTION__, __LINE__, __VA_ARGS__);

    #define LOG_FATAL(...) \
        _log_signal_dispatcher.emit_log(fsm::util::log::LogLevel::Fatal, _log_emitter_name, __FUNCTION__, __LINE__, __VA_ARGS__);

    #define LOG_SCOPE(...) \
        fsm::util::log::ScopeLog _scope_log_(&_log_signal_dispatcher, _log_emitter_name, __FUNCTION__, __LINE__, __VA_ARGS__); 

#endif // DISABLE_LOG_SYSTEM

#endif // FSM_SRC_UTIL_LOGGER_H
