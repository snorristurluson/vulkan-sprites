//
// Created by snorri on 19.9.2018.
//

#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "Logger.h"

namespace
{

    enum MessageType
    {
        CONNECTION_MESSAGE,
        SIMPLE_MESSAGE,
        LARGE_MESSAGE,
        CONTINUATION_MESSAGE,
        CONTINUATION_END_MESSAGE,
    };

    template <size_t size>
    void fillString(char (&destination)[size], const std::string& src)
    {
        strncpy(destination, src.data(), src.size());
        if (src.size() < size)
        {
            destination[src.size()] = 0;
        }
        else
        {
            destination[size - 1] = 0;
        }
    }

}

void Logger::connectToHost() {
    char hostname[1024];
    gethostname(hostname, 1024);

    char exename[1024];
    auto result = readlink("/proc/self/exe", exename, 1024);
    exename[result + 1] = 0;

    connectToHost("127.0.0.1", getpid(), hostname, exename);
}

void Logger::connectToHost(
        const std::string &server,
        int64_t pid,
        const std::string &machineName,
        const std::string &executablePath)
{
    m_state = Disconnected;
    m_machineName = machineName;
    m_pid = pid;
    m_executablePath = executablePath;

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket < 0) {
        return;
    }

    struct sockaddr_in serv_addr{};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(0xCC9);

    if(inet_pton(AF_INET, server.c_str(), &serv_addr.sin_addr) <= 0)
    {
        return;
    }

    if( connect(m_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        return;
    }

    RawLogMessage msg{};
    msg.type = CONNECTION_MESSAGE;
    msg.connection.pid = m_pid;
    msg.connection.version = 2;
    fillString(msg.connection.machineName, m_machineName);
    fillString(msg.connection.executablePath, m_executablePath);

    send(m_socket, reinterpret_cast<const char*>(&msg), sizeof(msg), 0);

    m_state = Connected;
}

void Logger::log(
        Logger::LogSeverity severity,
        const std::string &module,
        const std::string &channel,
        const std::string &message)
{
    if (m_state == Disconnected)
    {
        return;
    }

    RawLogMessage msg{};

    struct timespec tp{};
    clockid_t clk_id = CLOCK_REALTIME;
    clock_gettime(clk_id, &tp);

    msg.text.timestamp = static_cast<uint64_t>(tp.tv_sec) * 1000 + tp.tv_nsec / 1000000;
    msg.text.severity = severity;
    fillString(msg.text.module, module);
    fillString(msg.text.channel, channel);

    if (message.size() >= TextMessage::TEXT_SIZE)
    {
        msg.type = LARGE_MESSAGE;
    }
    else
    {
        msg.type = SIMPLE_MESSAGE;
    }
    int offset = 0;
    do
    {
        strncpy(msg.text.message, message.data() + offset, TextMessage::TEXT_SIZE - 1);
        msg.text.message[TextMessage::TEXT_SIZE - 1] = 0;
        offset += TextMessage::TEXT_SIZE - 1;

        if (m_state == Connected)
        {
            send(m_socket, reinterpret_cast<const char*>(&msg), sizeof(msg), 0);
        }
        if (offset + TextMessage::TEXT_SIZE < message.size())
        {
            msg.type = CONTINUATION_MESSAGE;
        }
        else
        {
            msg.type = CONTINUATION_END_MESSAGE;
        }
    }
    while (offset < message.size());
}

void Logger::disconnnect() {
    close(m_socket);
    m_state = Disconnected;
    m_socket = 0;
}

Logger *Logger::instance() {
    static Logger* s_instance = nullptr;
    if(!s_instance) {
        s_instance = new Logger;
        s_instance->connectToHost();
    }
    return s_instance;
}

std::shared_ptr<spdlog::logger> GetLogger(const char *name) {
    auto logger = spdlog::get(name);
    if(logger) {
        return logger;
    }
    auto sink = std::make_shared<LogLiteSink_mt>();
    logger = std::make_shared<spdlog::logger>(name, sink);
    logger->set_level(spdlog::level::debug);
    spdlog::register_logger(logger);
    return logger;
}
