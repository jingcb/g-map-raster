#ifndef __debug__utility__hh__
#define __debug__utility__hh__

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

enum class Severity {
    fatal,
    error,
    warning,
    info,
    debug,
    trace
};

class MappingLog {
public:
    MappingLog() = delete;
    ~MappingLog() = delete;
    
    static void Init(Severity severity, std::string log_folder = "") {
        if (log_folder != "") {
            logging::add_file_log (
                                   // file name pattern
                                   keywords::file_name = log_folder + "/g-map_%N.log",
                                   // rotate files every 10 MiB...
                                   keywords::rotation_size = 10 * 1024 * 1024,
                                   // ...or at midnight
                                   keywords::time_based_rotation = sinks::file::rotation_at_time_interval(boost::posix_time::minutes(1)),
                                   // keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                                   // keywords::time_based_rotation =
                                   // log record format
                                   keywords::format = "[%TimeStamp%]: %Message%");
        }
        
        switch (severity) {
            case Severity::fatal:
                logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::fatal);
                break;
            case Severity::error:
                logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::error);
                break;
            case Severity::warning:
                logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::warning);
                break;
            case Severity::info:
                logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
                break;
            case Severity::debug:
                logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::debug);
                break;
            case Severity::trace:
                logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::trace);
                break;
        }
    }
};



#endif
