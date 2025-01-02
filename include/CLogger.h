/**
 * @file CLogger.h
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#ifndef STL_VIEWER_CLOGGER_H_INCLUDED
#define STL_VIEWER_CLOGGER_H_INCLUDED

// Logging module
//
// Define LOGGING_ENABLED during compilation to enable logging.
// For performance reasons it's recommended to enable logging in Debug build.
// Not defined LOGGING_ENABLED will not add any code in the application.

#include "common.h"
#include <ostream>

/**
 * @enum LogLevel
 * @brief Defines the different log levels for logging output.
 *
 * This enum defines the severity of log messages. It is used to filter
 * log messages based on their importance.
 */
enum LogLevel { None=0, Error, Warning, Info, Debug, Trace };

/**
 * @brief Conditional logging class to handle logging at various levels.
 *
 * The `CLogger` class provides a mechanism for logging messages with different log levels
 * (Error, Warning, Info, Debug, Trace). It is designed to only compile and include logging
 * functionality when `LOGGING_ENABLED` is defined during compilation, helping to optimize performance.
 *
 * When logging is enabled, messages are printed to a specified log file, and optionally echoed
 * to the standard output (`std::cout`).
 */
#ifdef LOGGING_ENABLED

#include <array>
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * @class CLogger
 * @brief Handles logging messages to a file and/or standard output based on log level.
 *
 * This class is used to log messages to a file with information about the log level, file,
 * and line number where the log statement was called. The logger only outputs messages
 * whose level is equal to or higher than the configured log level.
 */
class CLogger
{
public:
    /**
     * @brief Constructor for the logger, initializing log header with file and line information.
     *
     * The constructor sets up the log entry header with the log level, file, and line number.
     *
     * @param logLevel The log level of the message (Error, Warning, etc.).
     * @param szFile The file from which the log is being generated.
     * @param iLine The line number where the log is being generated.
     */
    CLogger(LogLevel logLevel, const char * szFile, int iLine)
    {
        constexpr std::array<const char *,6> szLevels = {"", "ERR", "WRN", "INF", "DBG", "TRC"};
        m_bufferHeader << szFile << ";" << iLine << ";" << ((logLevel<szLevels.size())? szLevels[logLevel] : "???") << ";";
    }

    /**
     * @brief Streams a value to the log buffer.
     *
     * This function allows for appending data of any type to the log message.
     *
     * @param value The value to be logged.
     * @return A reference to the logger to allow chaining of logging operations.
     */
    template <typename T>
    CLogger & operator<<(T const & value)
    {
        m_buffer << value;
        return *this;
    }

    /**
     * @brief Destructor that writes the log message to the log file and/or standard output.
     *
     * The destructor writes the complete log message (header + body) to the log file if
     * a file is set. It also optionally echoes the log message to the console.
     */
    ~CLogger()
    {
        m_buffer << std::endl;
        if (CLogger::m_file)
        {
            CLogger::m_file << m_bufferHeader.str();
            CLogger::m_file << m_buffer.str();
            if (m_bEchoToCout)
                std::cout << m_buffer.str();
        }
        else
        {
            std::cerr << m_buffer.str();
        }
    }

    /**
     * @brief Sets the log level for filtering log messages.
     *
     * This function sets the minimum log level, meaning only messages with a log level
     * equal to or higher than the specified level will be logged.
     *
     * @param newLogLevel The new log level to be set.
     */
    static void setLevel(LogLevel newLogLevel) { m_logLevel = newLogLevel; }

    /**
     * @brief Gets the current log level.
     *
     * This function returns the current log level.
     *
     * @return The current log level.
     */
    static LogLevel getLevel() { return m_logLevel; }

    /**
     * @brief Enables logging to standard output (console).
     *
     * This function enables the option to echo log messages to the standard output (console).
     */
    static void enableEchoToCout() { m_bEchoToCout = true; }

    /**
     * @brief Disables logging to standard output (console).
     *
     * This function disables the option to echo log messages to the standard output (console).
     */
    static void disableEchoToCout() { m_bEchoToCout = false; }

    /**
     * @brief Sets the log file for logging output.
     *
     * This function sets the file to which log messages will be written. If the file
     * does not exist, it will be created.
     *
     * @param sFileName The name of the log file.
     */
    static void setFile(const std::string &sFileName) { if (!m_file) m_file.open(sFileName); }

    /**
     * @brief Closes the log file.
     *
     * This function closes the log file if it is open.
     */
    static void closeFile() { if (m_file) m_file.close(); }

private:
    static LogLevel m_logLevel; ///< The current log level for filtering log messages.
    static bool m_bEchoToCout; ///< Flag to indicate whether to echo log messages to std::cout.
    static std::ofstream m_file; ///< The output file stream for logging to a file.
    std::ostringstream m_bufferHeader{}; ///< The header containing file, line, and log level (not printed on the std::cout).
    std::ostringstream m_buffer{}; ///< The main buffer for log message content.
};

/**
 * @brief Macro for logging messages.
 *
 * This macro simplifies logging by automatically capturing the current log level, file,
 * and line number, and invoking the `CLogger` constructor. It only logs messages
 * whose log level is less than or equal to the configured log level.
 *
 * @param level The log level for the message (e.g., Error, Warning, Info, etc.).
 */
#define logPrint(level) if ((level) <= CLogger::getLevel()) CLogger((level), __FILE__, __LINE__)

#else // LOGGING_ENABLED

/**
 * @brief Empty `CLogger` class when logging is disabled.
 *
 * When logging is disabled (i.e., `LOGGING_ENABLED` is not defined), the `CLogger` class
 * is empty, and all logging-related operations are ignored, resulting in no performance overhead.
 */
class CLogger
{
public:
    CLogger() { }

    /**
     * @brief Empty operator overload when logging is disabled.
     *
     * This operator does nothing when logging is disabled.
     *
     * @return A reference to the `CLogger` instance.
     */
    template <typename T>
    CLogger & operator<<(const T&)
    {
        return *this;
    }

    /**
     * @brief Placeholder function to enable echo to `std::cout` when logging is disabled.
     */
    static void enableEchoToCout() { }

    /**
     * @brief Placeholder function to set the log level when logging is disabled.
     */
    static void setLevel(LogLevel) { }

    /**
     * @brief Placeholder function to set the log file when logging is disabled.
     */
    static void setFile(const std::string &) { }

    /**
     * @brief Placeholder function to close the log file when logging is disabled.
     */
    static void closeFile() { }
};

/**
 * @brief Empty logging macro when logging is disabled.
 *
 * This macro does nothing when logging is disabled.
 *
 * @param level The log level (unused in the disabled case).
 */
#define logPrint(level) CLogger()

#endif // LOGGING_ENABLED

/**
 * @brief Overload for outputting `Err` type to an output stream.
 *
 * This function defines how to output the `Err` enum to an output stream (e.g., `std::ostream`).
 *
 * @param stream The output stream to write to.
 * @param o The `Err` object to output.
 * @return The output stream with the serialized `Err` value.
 */
inline std::ostream& operator<<(std::ostream& stream, const Err &o)
{
    stream << static_cast<int>(o);
    return stream;
}

#endif // STL_VIEWER_CLOGGER_H_INCLUDED
