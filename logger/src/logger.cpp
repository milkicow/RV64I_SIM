#include "logger.hpp"

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <fstream>
#include <string_view>

namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", Logger::severity_level)

void Logger::init(severity_level log_level) {
    boost::log::formatter fmt = expr::stream << expr::smessage;

    typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
    boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
    sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>("full.log"));
    sink->locked_backend()->auto_flush(true);
    sink->set_formatter(fmt);
    sink->set_filter(severity <= log_level);
    boost::log::core::get()->add_sink(sink);

    boost::log::add_common_attributes();
}

void Logger::message(severity_level level, const std::string_view& module_name,
                     const std::string_view& message) {
    std::string module_n{};

    if (module_name.size() != 0) {
        module_n = std::string{module_name} + ": ";
    }

    BOOST_LOG_SEV(Logger::getLogger(), level)
        << boost::log::add_value("log_level", severity_levels[level].name) << module_n << message;
}
