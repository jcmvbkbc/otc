#ifndef _Diagnostics_h
#define _Diagnostics_h

#include <string>
#include <iostream>

struct Location
{
    static Location current;

    std::string m_file;
    int m_line;

    Location(): m_file(current.m_file), m_line(current.m_line)
    {
    }

    Location(const std::string& file, int line):
        m_file(file), m_line(line)
    {
    }
};

std::ostream& operator << (std::ostream& stm, const Location& l);
class Diagnostics
{
public:
    virtual std::ostream& error(const Location& l) = 0;
    virtual std::ostream& warn(int level, const Location& l) = 0;
    virtual std::ostream& debug(int level, const Location& l) = 0;

    virtual int getWarnLevel() const = 0;
    virtual int getDebugLevel() const = 0;

    virtual bool isClear() const = 0;
};

class DiagnosticsSetup
{
public:
    virtual void setWarnLevel(int level) = 0;
    virtual void setDebugLevel(int level) = 0;
};

Diagnostics& getDiagnostics();
DiagnosticsSetup& getDiagnosticsSetup();

#endif
