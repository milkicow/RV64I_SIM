#pragma once

#include <boost/log/sources/severity_logger.hpp>
#include <string>
#include <string_view>

class Logger {
   public:
    enum severity_level { release, trace, standard, verbose };

   private:
    struct SeverityType {
        severity_level level;
        std::string name;
    };

    const std::array<Logger::SeverityType, 4> severity_levels{{
        {release, "release"},
        {trace, "trace"},
        {standard, "standard"},
        {verbose, "verbose"},
    }};

    using logger_t = boost::log::sources::severity_logger<Logger::severity_level>;
    logger_t& getLogger() { return m_logger; };

    logger_t m_logger;
    Logger() {};

   public:
    static Logger& getInstance() {
        static Logger logger;
        return logger;
    }

    void init(severity_level log_level);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void message(severity_level level, const std::string_view& module_name,
                 const std::string_view& message);
};
