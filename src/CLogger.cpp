/**
 * @file CLogger.cpp
 * @author Grzegorz Pietrusiak <gpsspam2@gmail.com>
 * @date 2024-12-31
 * @copyright MIT License
 */

#include "CLogger.h"

#ifdef LOGGING_ENABLED

// Initialize static members of the logger module.
std::ofstream CLogger::m_file{"output.log"};
LogLevel CLogger::m_logLevel{Trace};
bool CLogger::m_bEchoToCout{false};

#else // LOGGING_ENABLED

#endif // LOGGING_ENABLED
