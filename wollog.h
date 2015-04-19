/*
 * wollog.h
 *
 *  Created on: Feb 8, 2015
 *      Author: kvareddy
 */

#ifndef WOLLOG_H_
#define WOLLOG_H_


#include <sstream>
#include <string>
#include <stdio.h>
#include <time.h>

//inline std::string NowTime();

enum WolLogLevel {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

class WolLog
{
public:
    WolLog();
    virtual ~WolLog();
    std::ostringstream& Get(WolLogLevel level = logINFO);
public:
    static FILE*& Stream();
    static WolLogLevel& ReportingLevel();
    static std::string ToString(WolLogLevel level);
    static WolLogLevel FromString(const std::string& level);
protected:
    std::ostringstream os;
private:
    WolLog(const WolLog&);
    WolLog& operator =(const WolLog&);
};

inline WolLog::WolLog()
{
}

inline std::ostringstream& WolLog::Get(WolLogLevel level)
{
  //  os << "- " << NowTime();
    os << " " << ToString(level) << ": ";
    os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
    return os;
}

inline WolLog::~WolLog()
{
    os << std::endl;
    FILE* pStream = Stream();
    if (!pStream)
       return;
    fprintf(pStream, "%s", os.str().c_str());
    fflush(pStream);
}

inline FILE*& WolLog::Stream()
{
    static FILE* pStream = stderr;
    return pStream;
}

inline WolLogLevel& WolLog::ReportingLevel()
{
    static WolLogLevel reportingLevel = logDEBUG4;
    return reportingLevel;
}

inline std::string WolLog::ToString(WolLogLevel level)
{
	static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"};
    return buffer[level];
}

inline WolLogLevel WolLog::FromString(const std::string& level)
{
    if (level == "DEBUG4")
        return logDEBUG4;
    if (level == "DEBUG3")
        return logDEBUG3;
    if (level == "DEBUG2")
        return logDEBUG2;
    if (level == "DEBUG1")
        return logDEBUG1;
    if (level == "DEBUG")
        return logDEBUG;
    if (level == "INFO")
        return logINFO;
    if (level == "WARNING")
        return logWARNING;
    if (level == "ERROR")
        return logERROR;
    WolLog().Get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
    return logINFO;
}


#define WOL_LOG(level) \
    if (level > WolLog::ReportingLevel()) ; \
    else WolLog().Get(level)

#define INFO_MSG \
	if (logINFO > WolLog::ReportingLevel()) ; \
	    else WolLog().Get(logINFO)

#define ERROR_MSG \
	if (logERROR > WolLog::ReportingLevel()) ; \
	    else WolLog().Get(logERROR)

#define WARN_MSG \
	if (logWARNING > WolLog::ReportingLevel()) ; \
	    else WolLog().Get(logWARNING)

#define DEBUG_MSG \
	if (logDEBUG > WolLog::ReportingLevel()) ; \
		else WolLog().Get(logDEBUG)

#define DEBUG1_MSG \
	if (logDEBUG1 > WolLog::ReportingLevel()) ; \
		else WolLog().Get(logDEBUG1)

#define DEBUG2_MSG \
	if (logDEBUG2 > WolLog::ReportingLevel()) ; \
		else WolLog().Get(logDEBUG2)

#define DEBUG3_MSG \
	if (logDEBUG3 > WolLog::ReportingLevel()) ; \
		else WolLog().Get(logDEBUG3)

#define DEBUG4_MSG \
	if (logDEBUG4 > WolLog::ReportingLevel()) ; \
		else WolLog().Get(logDEBUG4)


/*
inline std::string NowTime()
{
    char buffer[11];
    time_t t;
    time(&t);
    tm r = {0};
    strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
    struct timeval tv;
    gettimeofday(&tv, 0);
    char result[100] = {0};
    std::sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000);
    return result;
}*/




#endif /* WOLLOG_H_ */
