#include <sstream>
#include "Diagnostics.h"

Location Location::current("", 0);

std::ostream& operator << (std::ostream& stm, const Location& l)
{
    stm << l.m_file << ":" << l.m_line + 1;
    return stm;
}

class DiagnosticsImpl: public Diagnostics, public DiagnosticsSetup
{
	int m_warnLevel;
	int m_debugLevel;
	bool m_isClear;

public:
	DiagnosticsImpl(): m_warnLevel(3), m_debugLevel(1), m_isClear(true)
	{
	}

	virtual std::ostream& error(const Location& l)
	{
		m_isClear = false;
		std::cerr << "*** Error: " << l << ": ";
		return std::cerr;
	}
	virtual std::ostream& warn(int level, const Location& l)
	{
		if (level <= getWarnLevel()) {
			std::cerr << "*** Warn : " << l << ": ";
			return std::cerr;
		} else {
			static std::ostringstream stm;

			stm.clear();
			return stm;
		}
	}
	virtual std::ostream& debug(int level, const Location& l)
	{
		if (level <= getDebugLevel()) {
			std::cerr << "*** Debug: " << l << ": ";
			return std::cerr;
		} else {
			static std::ostringstream stm;

			stm.clear();
			return stm;
		}
	}

	virtual int getWarnLevel() const
	{
		return m_warnLevel;
	}

	virtual void setWarnLevel(int level)
	{
		m_warnLevel = level;
	}

	virtual int getDebugLevel() const
	{
		return m_debugLevel;
	}

	virtual void setDebugLevel(int level)
	{
		m_debugLevel = level;
	}


	virtual bool isClear() const
	{
		return m_isClear;
	}
};

static DiagnosticsImpl& getDiagnosticsImpl()
{
	static DiagnosticsImpl obj;
	return obj;
}

Diagnostics& getDiagnostics()
{
	return getDiagnosticsImpl();
}

DiagnosticsSetup& getDiagnosticsSetup()
{
	return getDiagnosticsImpl();
}
