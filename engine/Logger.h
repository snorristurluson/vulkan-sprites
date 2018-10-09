//
// Created by snorri on 19.9.2018.
//

#ifndef VULKAN_SPRITES_LOGGER_H
#define VULKAN_SPRITES_LOGGER_H

#include <string>
#include <sys/socket.h>

class Logger
{
public:
    static Logger* instance();

    enum LogSeverity
    {
        SEVERITY_INFO,
        SEVERITY_NOTICE,
        SEVERITY_WARN,
        SEVERITY_ERR,

        SEVERITY_COUNT,
    };

    void connectToHost();
    void connectToHost(const std::string& server, int64_t pid, const std::string& machineName, const std::string& executablePath);
    void disconnnect();

    void log(LogSeverity severity, const std::string& module, const std::string& channel, const std::string& message);

private:

    struct ConnectionMessage
    {
        static const int MESSAGE_MAX_PATH = 260;

        uint32_t version;
        int64_t pid;
        char machineName[32];
        char executablePath[MESSAGE_MAX_PATH];
    };

    struct TextMessage
    {
        static const int TEXT_SIZE = 256;

        uint64_t timestamp;
        uint32_t severity;
        char module[32];
        char channel[32];
        char message[TEXT_SIZE];
    };

    struct RawLogMessage
    {
        uint32_t type;
        union
        {
            ConnectionMessage connection;
            TextMessage text;
        };
    };


    int m_socket;
    int64_t m_pid;
    std::string m_machineName;
    std::string m_executablePath;

    enum State
    {
        Disconnected,
        Connecting,
        Connected,
    };

    State m_state;
};

#include "spdlog/sinks/base_sink.h"
#include "spdlog/fmt/fmt.h"

template<typename Mutex>
class LogLiteSink : public spdlog::sinks::base_sink <Mutex>
{
public:
    LogLiteSink()
    {
        m_logger = Logger::instance();
    }

protected:
    Logger* m_logger;

    void _sink_it(const spdlog::details::log_msg& msg) override
    {
        Logger::LogSeverity severity;
        switch(msg.level) {
            case spdlog::level::trace:
            case spdlog::level::debug:
                severity = Logger::SEVERITY_INFO;
                break;
            case spdlog::level::info:
                severity = Logger::SEVERITY_NOTICE;
                break;
            case spdlog::level::warn:
                severity = Logger::SEVERITY_WARN;
                break;
            case spdlog::level::err:
                severity = Logger::SEVERITY_ERR;
                break;
            default:
                return;
        }
        m_logger->log(severity, "module", *msg.logger_name, msg.raw.c_str());
    }
    void _flush() override
    {
    }
};

#include "spdlog/details/null_mutex.h"
#include <mutex>
using LogLiteSink_mt = LogLiteSink<std::mutex>;
using LogLiteSink_st = LogLiteSink<spdlog::details::null_mutex>;

#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> GetLogger(const char *name);

#endif //VULKAN_SPRITES_LOGGER_H
