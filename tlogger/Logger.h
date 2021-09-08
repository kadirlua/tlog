#pragma warning(disable : 4996)
#pragma once
#include <mutex>
#include <string>
#include <memory>
#include <fstream>
#include <locale>
#include <codecvt>
#include "Formatter.h"
#if __cplusplus >= 201703L
#include <string_view>
#include <filesystem>
#else
#if defined _MSC_VER
	#include <direct.h>
#elif defined __GNUC__
	#include <sys/types.h>
	#include <sys/stat.h>
#endif
#endif


/*
* TODO:
	Done :
	- Use type alias for template class. char -> Logger / wchar_t -> LoggerW
	- Add time functions & __FILE__ macro
	- Fix LogMessage
	- Add time to streams in logMessage
	- test setter functions
	- Add Format support
	- Improve comments. Especially Classes and  enum classes
	- Fix character encoding
	- limit unsigned int to unsigned long long
	- Add C++17 filesystem for limit the size of file
	- add UTF8 support

	Add :
	- add C++14 file mkdir
	
	Test :
	- Macro definitions
	
	Attention :
	- changelog -> (release note)
	- Attention to keywords. virtual  noexcept , override etc.
	- Try shared_lock for C++17 & C++20
*/

namespace aricanli {
	namespace general {

		// LogPriority enum class
		// Possible priority levels
		enum class LogPriority : unsigned int {
			Quiet = 0,
			Fatal = 8,
			Error = 16,
			Warning = 24,
			Info = 32,
			Verbose = 40,
			Debug = 48,
			Trace = 56
		};

		// LogOutput enum class
		// Possible ways to stream
		enum class LogOutput {
			Console,
			File
		};

		// Stream wrapper class for console in char and wchar_t types
		template <typename T>
		class StreamWrapper {
		public:
			static std::basic_ostream<T>& tout;
		};

		template<>
		std::ostream& StreamWrapper<char>::tout = std::cout;

		template<>
		std::wostream& StreamWrapper<wchar_t>::tout = std::wcout;

		// Class Logger<> 
		// record of variadic arguments to selected stream in formatted string
		// Example:
		// 	Logger<wchar_t>::setLogOutput(L"log.txt");
		//	Logger<wchar_t>::setLogPriority(LogPriority::Debug);
		//	auto log = Logger<wchar_t>::getInstance();
		//	log->setFormatter(L"%f %l %m %t");
		//	log->log(LogPriority::Error, __LINE__, _T(__FILE__), L"tellus felis condimentum odio, : ", 1, );
		template <typename T = char>
		class Logger {
		public:
			Logger(const Logger&) noexcept = delete;
			Logger& operator=(const Logger&) noexcept = delete;
			Logger(Logger&&) noexcept = default;
			Logger& operator=(Logger&&) noexcept = default;
			virtual ~Logger() noexcept {
				std::lock_guard<std::mutex> _lock(m_mutex);
				m_ofs.close();
			}

			static std::shared_ptr<Logger> getInstance() {
				/**
				* Get single instance or create new object if not created
				* @return: std::shared_ptr<Logger>
				*/
				if (loggerInstance == nullptr)
					loggerInstance = std::shared_ptr<Logger<T>>(new Logger<T>{  });
				return loggerInstance;
			}

			static void setLogOutput(std::basic_string<T> t_filePath) {
				/*
				* Set log path as std::basic_string
				* @param t_fileName : file name
				* @param t_extName : extension name
				*/
				std::lock_guard<std::mutex> _lock(m_mutex);		 
				m_logPath = t_filePath;  
			}

			static void setLogPriority(LogPriority t_logPriority) {
				/*
				* Set log priority level
				* @param t_logPriority: enum class LogPriority
				*/
				std::lock_guard<std::mutex> _lock(m_mutex);
				m_logPriority = t_logPriority;
			}

			static void log(LogPriority messageLevel){} // For Quiet priority 

			template<typename ...Args>
			static void log(LogPriority messageLevel, Args &&...args) {
				/*
				* Log given message with defined parameters and pass LogMessage() function
				* @param messageLevel: Log Level
				* @param line : __LINE__ macro
				* @param funcName : __FILE__ macro
				* @param ...args: Variadic template arguments
				*/
				std::lock_guard<std::mutex> _lock(m_mutex);
				if (messageLevel <= m_logPriority) {
					switch (messageLevel) {
					case LogPriority::Quiet:
						break;
					case LogPriority::Fatal:
						LogMessage( "FATAL:", std::forward<Args>(args)...);

						break;
					case LogPriority::Error:
						LogMessage( "ERROR:", std::forward<Args>(args)...);

						break;
					case LogPriority::Warning:
						LogMessage( "WARNING:", std::forward<Args>(args)...);

						break;
					case LogPriority::Info:
						LogMessage( "INFO:", std::forward<Args>(args)...);

						break;
					case LogPriority::Verbose:
						LogMessage( "VERBOSE:", std::forward<Args>(args)...);

						break;
					case LogPriority::Debug:
						LogMessage( "DEBUG:", std::forward<Args>(args)...);

						break;
					case  LogPriority::Trace:
						LogMessage( "TRACE:", std::forward<Args>(args)...);

						break;
					}
				}
			}

			template<typename ...Args>
			static void log(LogPriority messageLevel, int line, const std::basic_string<T> funcName, Args &&...args) {
				/*
				* Log given message with defined parameters and pass LogMessage() function
				* @param messageLevel: Log Level
				* @param line : __LINE__ macro
				* @param funcName : __FILE__ macro
				* @param ...args: Variadic template arguments
				*/
				std::lock_guard<std::mutex> _lock(m_mutex);
				if (messageLevel <= m_logPriority) {
					switch (messageLevel) {
					case LogPriority::Quiet:
						break;
					case LogPriority::Fatal:
						LogMessage(line, funcName, "FATAL:", std::forward<Args>(args)...);

						break;
					case LogPriority::Error:
						LogMessage(line, funcName, "ERROR:", std::forward<Args>(args)...);

						break;
					case LogPriority::Warning:
						LogMessage(line, funcName, "WARNING:", std::forward<Args>(args)...);

						break;
					case LogPriority::Info:
						LogMessage(line, funcName, "INFO:", std::forward<Args>(args)...);

						break;
					case LogPriority::Verbose:
						LogMessage(line, funcName, "VERBOSE:", std::forward<Args>(args)...);

						break;
					case LogPriority::Debug:
						LogMessage(line, funcName, "DEBUG:", std::forward<Args>(args)...);

						break;
					case  LogPriority::Trace:
						LogMessage(line, funcName, "TRACE:", std::forward<Args>(args)...);

						break;
					}
				}
			}

			static void setFormatter(const std::basic_string<T>& t_fmt)  {
				/*
				* Get format type and pass to Formatter::getFormatter() function
				* default as %t %m
				*/
				std::lock_guard<std::mutex> _lock(m_mutex);
				fmt.getFormatter(t_fmt);
			}

			static void setFileLimit(unsigned long long  t_fileLimit)  {
				/*
				* Set file's limit (byte)
				* default as %t %m
				*/
				std::lock_guard<std::mutex> _lock(m_mutex);
				m_maxFileSize = t_fileLimit; 
			}

			static void setLogFormat() {
				if (m_logPath.empty()) {
					m_logOutput = LogOutput::Console;
				}
				else {
					m_logOutput = LogOutput::File;
					openFile(m_logPath);
				}
			}
		protected:
			Logger() noexcept
			{
				/*
				* Construct Logger class as singleton.
				* client can not access default constructor
				* if m_logPath string is empty select the stream as console
				* else select the stream as file
				*/
				setFormatter(stringlit(T,"%m %t"));
				setLogFormat();
			}

#if __cplusplus >= 201703L
			static void openFile(std::filesystem::path t_path) {
				/*
				* Open file in ofstream out or append mode
				* @param t_path : filesystem::path
				*/
				std::lock_guard<std::mutex> _lock(m_mutex);
				try {
					auto t_root = t_path.parent_path();
					if (!std::filesystem::exists(t_path)) {
						std::filesystem::create_directories(t_root.string());
					}
					m_ofs.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
					m_ofs.open(t_path, std::ofstream::out | std::ofstream::app);

					//std::locale utf8_locale(std::locale(), new utf8cvt<false>);


					m_ofs.seekp(0, std::ios_base::end);
				}
				catch (const std::filesystem::filesystem_error& ex) {
					std::cout << ex.what() << "\n";
				}
				catch (...) {
					std::cout << "Unknown error in Logger::openFile()\n";
				}
			}
#else
			static void openFile(const std::basic_string<T>& t_path) {
				/*
				* Open file in ofstream out or append mode
				* @param t_path : filesystem::path
				*/
				std::lock_guard<std::mutex> _lock(m_mutex);


				// Create directory 
#if defined _MSC_VER
				_mkdir(t_path);
#elif defined __GNUC__
				mkdir(t_path, 0777);
#endif

				m_ofs.open(t_path, std::ofstream::out | std::ofstream::app);
			}
#endif


			template <typename ...Args>
			static void LogMessage( Args &&...args) {
				/*
				* Pass argumants to set Formatter::format() and take back as formatted string
				* and write the string to choosen stream in constructor
				* @param logPriority: Log priority
				* @param ...args: Variadic template arguments
				*/

				auto formattedStr = fmt.format(std::forward<Args>(args)...);

				formattedStr += '\n';
				if (m_logOutput == LogOutput::File) {
					if ((static_cast<unsigned long long>(m_ofs.tellp()) + formattedStr.length()) >= m_maxFileSize) {
						m_ofs.close();
						static int iter = 1;
						std::basic_ostringstream<T> oss;
						oss << iter;

						const size_t pos = m_logPath.find_last_of('.');
						if (pos != std::string::npos) {
							std::basic_string<T> t_pathName = m_logPath.substr(0, pos);
							std::basic_string<T> t_extName = m_logPath.substr(pos + 1);
							t_pathName += oss.str() + stringlit(T, ".") + t_extName;
							m_ofs.open(t_pathName, std::ofstream::out | std::ofstream::app);
						}
						iter++;
					}
					m_ofs << formattedStr.c_str();
				}
				if (m_logOutput == LogOutput::Console)
					StreamWrapper<T>::tout << formattedStr.c_str();
			}


			template <typename ...Args>
			static void LogMessage(int line, const std::basic_string<T> funcName, Args &&...args) {
				/*
				* Pass argumants to set Formatter::format() and take back as formatted string
				* and write the string to choosen stream in constructor
				* @param logPriority: Log priority
				* @param ...args: Variadic template arguments
				*/

				auto formattedStr = fmt.format(line, funcName, std::forward<Args>(args)...);
				
				formattedStr += '\n';
				if (m_logOutput == LogOutput::File ) {
					if ( (static_cast<unsigned long long>(m_ofs.tellp()) + formattedStr.length()) >= m_maxFileSize ) {
						m_ofs.close();
						static int iter = 1;
						std::basic_ostringstream<T> oss;
						oss << iter;

						const size_t pos = m_logPath.find_last_of('.');
						if (pos != std::string::npos) {
							std::basic_string<T> t_pathName = m_logPath.substr(0, pos);
							std::basic_string<T> t_extName = m_logPath.substr(pos + 1);
							t_pathName += oss.str() + stringlit(T, ".") + t_extName;
							m_ofs.open(t_pathName, std::ofstream::out | std::ofstream::app);
						}
						iter++;
					}
					m_ofs << formattedStr.c_str();
				}
				if (m_logOutput == LogOutput::Console)
					StreamWrapper<T>::tout << formattedStr.c_str() ;
			}

		protected:
			static std::mutex m_mutex;
			static Formatter<T> fmt;
			static unsigned long long m_maxFileSize;
			static std::basic_string<T> m_logPath;
			static std::basic_ofstream<T> m_ofs;
			static std::shared_ptr<Logger<T>> loggerInstance;
			static LogPriority m_logPriority;
			static LogOutput m_logOutput;
;
		}; // end of class 

		// Macro definitions for Logger::log() 
#define LOG_QUIET()
#define LOG_SET_FORMAT_C( formatter )  { \
		   aricanli::general::Logger<char>::setFormatter(formatter);}
#define LOG_SET_OUTPUT_C( path, ext)  {									\
		   aricanli::general::Logger<char>::setLogOutput(path, ext);    }
#define LOG_SET_OUTPUT_FORMAT_C( )  { \
		   aricanli::general::Logger<char>::setLogFormat();}
#define LOG_SET_PRIORITY_QUIET_C( )  { \
		   aricanli::general::Logger<char>::setLogPriority(LogPriority::Quiet);}
#define LOG_SET_PRIORITY_FATAL_C( )  { \
		   aricanli::general::Logger<char>::setLogPriority(LogPriority::Fatal);}
#define LOG_SET_PRIORITY_ERROR_C( )  { \
		   aricanli::general::Logger<char>::setLogPriority(LogPriority::Error);}
#define LOG_SET_PRIORITY_WARNING_C( )  { \
		   aricanli::general::Logger<char>::setLogPriority(LogPriority::Warning);}
#define LOG_SET_PRIORITY_INFO_C( )  { \
		   aricanli::general::Logger<char>::setLogPriority(LogPriority::Info);}
#define LOG_SET_PRIORITY_VERBOSE_C( )  { \
		   aricanli::general::Logger<char>::setLogPriority(LogPriority::Verbose);}
#define LOG_SET_PRIORITY_DEBUG_C( )  { \
		   aricanli::general::Logger<char>::setLogPriority(LogPriority::Debug);}
#define LOG_SET_PRIORITY_TRACE_C( )  { \
		   aricanli::general::Logger<char>::setLogPriority(LogPriority::Trace);}

#define LOG_FATAL_C( ... )  { \
		   aricanli::general::Logger<char>::log(aricanli::general::LogPriority::Fatal, __VA_ARGS__ );}
#define LOG_ERROR_C( line, file, ... )   { \
		aricanli::general::Logger<char>::log(aricanli::general::LogPriority::Error, line, file, __VA_ARGS__ );	}
#define LOG_WARNING_C( line,file,... ) { aricanli::general::Logger<char>::log( \
		  aricanli::general::LogPriority::Warning, line, file, __VA_ARGS__ );}
#define LOG_INFO_C( ...)     { aricanli::general::Logger<char>::log( \
		  aricanli::general::LogPriority::Info, __VA_ARGS__ );}
#define LOG_VERBOSE_C( ... ) { aricanli::general::Logger<char>::log( \
		  aricanli::general::LogPriority::Verbose, __VA_ARGS__ );}
#define LOG_DEBUG_C( ... )   { aricanli::general::Logger<char>::log( \
		  aricanli::general::LogPriority::Debug, __VA_ARGS__ );}
#define LOG_TRACE_C( ... )   { aricanli::general::Logger<char>::log( \
		  aricanli::general::LogPriority::Trace, __VA_ARGS__ );}

#define LOG_FATAL_W( ... )  { aricanli::general::Logger<wchar_t>::log( \
		  aricanli::general::LogPriority::Fatal, __VA_ARGS__ );}
#define LOG_ERROR_W( ... )   { aricanli::general::Logger<wchar_t>::log( \
		  aricanli::general::LogPriority::Error, __VA_ARGS__ );}
#define LOG_WARNING_W( ... ) { aricanli::general::Logger<wchar_t>::log( \
		  aricanli::general::LogPriority::Warning, __VA_ARGS__ );}
#define LOG_INFO_W( ...)     { aricanli::general::Logger<wchar_t>::log( \
		  aricanli::general::LogPriority::Info, __VA_ARGS__ );}
#define LOG_VERBOSE_W( ... ) { aricanli::general::Logger<wchar_t>::log( \
		  aricanli::general::LogPriority::Verbose, __VA_ARGS__ );}
#define LOG_DEBUG_W( ... )   { aricanli::general::Logger<wchar_t>::log( \
		  aricanli::general::LogPriority::Debug, __VA_ARGS__ );}
#define LOG_TRACE_W( ... )   { aricanli::general::Logger<wchar_t>::log( \
		  aricanli::general::LogPriority::Trace, __VA_ARGS__ );}


		// Intialize static data members
		template<typename T>
		std::mutex Logger<T>::m_mutex;
		template<typename T>
		std::basic_ofstream<T> Logger<T>::m_ofs;
		template<typename T>
		unsigned long long Logger<T>::m_maxFileSize = 512 *1024 * 1024; // 512 MB
		template<typename T>
		LogPriority Logger<T>::m_logPriority = LogPriority::Trace;
		template<typename T>
		LogOutput Logger<T>::m_logOutput = LogOutput::Console;
		template<typename T>
		std::basic_string<T> Logger<T>::m_logPath;
		template<typename T>
		std::shared_ptr<Logger<T>> Logger<T>::loggerInstance = nullptr;
		template <typename T>
		Formatter<T> Logger<T>::fmt;

		// template alias for Logger class
		using LoggerW = Logger<wchar_t>;

	} // end of general namespace
} // end of aricanli namespace
