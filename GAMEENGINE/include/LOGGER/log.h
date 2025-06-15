#pragma once

/*
 * @brief Variadic Macro for logging Information. This macro takes in a string message, followed by the
 * necessary arguments.
 * @param Takes an std::string_view or string in the form of "This is a log value: {0}, and {1}", followed by
 * the arguments
 */
#ifdef _WIN32
#	define LOG_INFO( x, ... ) Logger::GetInstance().Log( x, ##__VA_ARGS__ )
#else
#	define LOG_INFO( x, ... ) Logger::GetInstance().Log( x, ##__VA_ARGS__ )
#endif

/*
 * @brief Variadic Macro for logging warnings. This macro takes in a string message, followed by the
 * necessary arguments.
 * @param Takes an std::string_view or string in the form of "This is a log value: {0}, and {1}", followed by
 * the arguments
 */
#ifdef _WIN32
#	define LOG_WARN( x, ... ) Logger::GetInstance().Warn( x, ##__VA_ARGS__ )
#else
#	define LOG_WARN( x, ... ) Logger::GetInstance().Warn( x, ##__VA_ARGS__ )
#endif

/*
 * @brief Variadic Macro for logging Errors. This macro takes in a string message, followed by the
 * necessary arguments.
 * @param Takes an std::string_view or string in the form of "This is a log value: {0}, and {1}", followed by
 * the arguments
 */
#ifdef _WIN32
#	define LOG_ERROR( x, ... ) \
		Logger::GetInstance().Error( std::source_location::current(), x, ##__VA_ARGS__ )
#else
#	define LOG_ERROR( x, ... ) \
		Logger::GetInstance().Error( std::source_location::current(), x, ##__VA_ARGS__ )
#endif

// ####################LUA#######################################
/*
 * @brief Variadic Macro for logging Information. This macro takes in a string message, followed by the
 * necessary arguments.
 * @param Takes an std::string_view or string in the form of "This is a log value: {0}, and {1}", followed by
 * the arguments
 */
#ifdef _WIN32
#	define LUA_INFO( x, ... ) Logger::GetInstance().LuaLog( x, ##__VA_ARGS__ )
#else
#	define LUA_INFO( x, ... ) Logger::GetInstance().LuaLog( x, ##__VA_ARGS__ )
#endif

/*
 * @brief Variadic Macro for logging warnings. This macro takes in a string message, followed by the
 * necessary arguments.
 * @param Takes an std::string_view or string in the form of "This is a log value: {0}, and {1}", followed by
 * the arguments
 */
#ifdef _WIN32
#	define LUA_WARN( x, ... ) Logger::GetInstance().LuaWarn( x, ##__VA_ARGS__ )
#else
#	define LUA_WARN( x, ... ) Logger::GetInstance().LuaWarn( x, ##__VA_ARGS__ )
#endif

/*
 * @brief Variadic Macro for logging Errors. This macro takes in a string message, followed by the
 * necessary arguments.
 * @param Takes an std::string_view or string in the form of "This is a log value: {0}, and {1}", followed by
 * the arguments
 */
#ifdef _WIN32
#	define LUA_ERROR( x, ... ) \
		Logger::GetInstance().LuaError( x, ##__VA_ARGS__ )
#else
#	define LUA_ERROR( x, ... ) \
		Logger::GetInstance().LuaError( x, ##__VA_ARGS__ )
#endif

// #########################################################################

#define LOGGER_ASSERT( x ) assert( x )
#define LOGGER_INIT_LOGS( console, retain ) Logger::GetInstance().Init( console, retain )
#define LOGGER_LOG_ADDED() Logger::GetInstance().LogAdded()
#define LOGGER_RESET_ADDED() Logger::GetInstance().ResetLogAdded()
#define LOGGER_GET_LOGS() Logger::GetInstance().GetLogs()
#define LOGGER_CLEAR_LOGS() Logger::GetInstance().ClearLogs()

struct LogEntry
{
	enum class LogType
	{
		INFO,
		WARN,
		ERR,
		NONE
	};
	LogType type{ LogType::INFO };
	std::string log{ "" };
};

class Logger
{
private:
	std::vector<LogEntry> m_LogEntries;
	bool m_bLogAdded{ false }, m_bInitialized{ false }, m_bConsoleLog{ true }, m_bRetainLogs{ true };

	Logger() = default;

	struct LogTime
	{
		std::string day, dayNumber, month, year, time;
		LogTime( const std::string& date );
	};

	std::string CurrentDateTime();

public:
	static Logger& GetInstance();

	~Logger() = default;
	// Make the logger non-copyable
	Logger( const Logger& ) = delete;
	Logger& operator=( const Logger& ) = delete;

	void Init( bool consoleLog = true, bool retainLogs = true );

	template <typename... Args>
	void Log( const std::string_view message, Args&&... args );

	template <typename... Args>
	void Warn( const std::string_view message, Args&&... args );

	template <typename... Args>
	void Error( std::source_location location, const std::string_view message, Args&&... args );

	template <typename... Args>
	void LuaLog( const std::string_view message, Args&&... args );

	template <typename... Args>
	void LuaWarn( const std::string_view message, Args&&... args );

	template <typename... Args>
	void LuaError( const std::string_view message, Args&&... args );

	inline void ClearLogs() { m_LogEntries.clear(); }
	inline const std::vector<LogEntry>& GetLogs() { return m_LogEntries; }
	inline void ResetLogAdded() { m_bLogAdded = false; }
	inline const bool LogAdded() const { return m_bLogAdded; }
};

#include "log.inl"
