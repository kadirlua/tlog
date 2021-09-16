#pragma once
#include <mutex>
#include <string>
#include <memory>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iostream>
#include <vector>
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

namespace aricanli {
	namespace general {

		// LogPriority enum class
		// Possible priority levels : 
		//Quiet , Fatal , Error , Warning , Info , Verbose , Debug , Trace
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

		// For PreC++17 to create a directory 
#if __cplusplus < 201703L

		template <typename T>
		static void t_mkdir(T) { }


#if defined _MSC_VER
		template <>
		static void t_mkdir(std::string t_path) {
			(void)_mkdir(t_path.c_str());
		}

		template <>
		static void t_mkdir(std::wstring t_path) {
			(void)_wmkdir(t_path.c_str());
		}
#elif defined __GNUC__
		template <>
		static void t_mkdir(std::string t_path) {
			mkdir(t_path.c_str(), 0777);
		}

		template <>
		static void t_mkdir(std::wstring t_path) {
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> strconverter;
			mkdir(strconverter.to_bytes(t_path).c_str(), 0777);
		}
#endif

#endif
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
			Logger(const Logger&) = delete;
			Logger& operator=(const Logger&) = delete;
			virtual ~Logger() noexcept {
				std::lock_guard<std::mutex> _lock(m_mutex);
				m_ofs.close();
			}

			/*
			* Get single instance or create new object if not created
			* @return: std::shared_ptr<Logger>
			*/
			static std::shared_ptr<Logger> getInstance() {

				if (loggerInstance == nullptr)
					loggerInstance = std::shared_ptr<Logger<T>>(new Logger<T>{  });
				return loggerInstance;
			}

			/*
			* Not set or call for stream to console
			* Set log path as std::basic_string for stream to file
			* @param t_filePath : basic_string<T>
			*/
			static void setLogOutput(std::basic_string<T> t_filePath) {

				std::lock_guard<std::mutex> _lock(m_mutex);
				m_logPath = t_filePath;
			}

			/*
			* Set log priority level
			* @param t_logPriority: enum class LogPriority
			*/
			static void setLogPriority(LogPriority t_logPriority) {
				std::lock_guard<std::mutex> _lock(m_mutex);
				m_logPriority = t_logPriority;
			}

			static void log(LogPriority messageLevel) {} // For Quiet priority 

			/*
			* Log given message with defined parameters and pass LogMessage() function
			* @param messageLevel: Log Level
			* @param ...args: Variadic template arguments
			*/
			template<typename ...Args>
			static void log(LogPriority messageLevel, Args &&...args) {

				std::lock_guard<std::mutex> _lock(m_mutex);
				if (messageLevel <= m_logPriority) {
					switch (messageLevel) {
					case LogPriority::Quiet:
						break;
					case LogPriority::Fatal:
						LogMessage("FATAL:", std::forward<Args>(args)...);

						break;
					case LogPriority::Error:
						LogMessage("ERROR:", std::forward<Args>(args)...);

						break;
					case LogPriority::Warning:
						LogMessage("WARNING:", std::forward<Args>(args)...);

						break;
					case LogPriority::Info:
						LogMessage("INFO:", std::forward<Args>(args)...);

						break;
					case LogPriority::Verbose:
						LogMessage("VERBOSE:", std::forward<Args>(args)...);

						break;
					case LogPriority::Debug:
						LogMessage("DEBUG:", std::forward<Args>(args)...);

						break;
					case  LogPriority::Trace:
						LogMessage("TRACE:", std::forward<Args>(args)...);

						break;
					}
				}
			}

			/*
			* Get format type and pass to Formatter::getFormatter() function
			* default as %t %m
			*/
			static void setFormatter(const std::basic_string<T>& t_fmt) {

				std::lock_guard<std::mutex> _lock(m_mutex);
				fmt.getFormatter(t_fmt);
			}

			/*
			* Set file's limit (byte)
			*/
			static void setFileLimit(unsigned long long  t_fileLimit) {

				std::lock_guard<std::mutex> _lock(m_mutex);
				m_maxFileSize = t_fileLimit;
			}

			/*
			* Set output format
			* if m_logPath is empty then stream to console
			* if m_logPath is not empty then stream to file
			*/
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
			/*
			* Construct Logger class as singleton.
			* client can not access default constructor
			* if m_logPath string is empty select the stream as console
			* else select the stream as file
			*/
			Logger() noexcept
			{
				setLogFormat();
			}

#if __cplusplus >= 201703L
			/*
			* For C++17 and C++20 versions
			* Open file in UTF-8 standart in ofstream write or append mode
			* if the parent path not exist then create directory
			* @param t_path : filesystem::path
			*/
			static void openFile(std::filesystem::path t_path) {

				std::lock_guard<std::mutex> _lock(m_mutex);
				try {
					auto t_root = t_path.parent_path();

					if (!std::filesystem::exists(t_path) && !t_root.empty()) {
						std::filesystem::create_directories(t_root.string());
					}
#if defined _MSC_VER
					m_ofs.imbue(std::locale(std::locale::empty(), new std::codecvt<wchar_t, char, mbstate_t>("en_US.utf8")));
#elif defined __GNUC__
					m_ofs.imbue(std::locale(std::locale(), new std::codecvt<wchar_t, char, mbstate_t>));
#endif
					m_ofs.open(t_path, std::ofstream::out | std::ofstream::app);
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
			/*
			* For C++14 and previous versions
			* Open file in UTF-8 standart in ofstream write or append mode
			* if the parent path not exist then create directory
			* @param t_path : basic_string<T>
			*/
			static void openFile(const std::basic_string<T>& t_path) {

				std::lock_guard<std::mutex> _lock(m_mutex);

				char delim = t_path.find('/') != std::string::npos ? '/' : '\\';
				auto ret = split(t_path, delim);
				std::basic_string<T> path;
				for (int i = 0; i < ret.size() - 1; i++) {
					path += (i != 0) ? stringlit(T, "/") + ret[i] : ret[i];
					t_mkdir(path);
				}
#if defined _MSC_VER
				m_ofs.imbue(std::locale(std::locale::empty(), new std::codecvt<wchar_t, char, mbstate_t>("en_US.utf8")));
#elif defined __GNUC__
				m_ofs.imbue(std::locale(std::locale(), new std::codecvt<wchar_t, char, mbstate_t>));
#endif
				m_ofs.open(t_path.c_str(), std::ofstream::out | std::ofstream::app);
				m_ofs.seekp(0, std::ios_base::end);
			}
#endif
			/*
			* Pass argumants to set Formatter::format() and take back as formatted string
			* and write the string to choosen stream in constructor
			* @param ...args: Variadic template arguments
			*/
			template <typename ...Args>
			static void LogMessage(Args &&...args) {

				auto formattedStr = fmt.format(std::forward<Args>(args)...);

				formattedStr += '\n';
				if (m_logOutput == LogOutput::File) {
					if ((static_cast<unsigned long long>(m_ofs.tellp()) + formattedStr.length()) >= m_maxFileSize) {
						m_ofs.close();
						m_ofs.open(m_logPath, std::ios::out | std::ios::trunc);
					}
					m_ofs << formattedStr.c_str();
				}
				if (m_logOutput == LogOutput::Console)
					StreamWrapper<T>::tout << formattedStr.c_str();

			}

			/*
			* Divides a String into an ordered list of substrings, puts these substrings into
			* an vector of string
			* @param strSplit: basic_string<T>
			* @param Delim: Template argument
			* @return : result : vector<basic_string<T>>
			*/
			static std::vector<std::basic_string<T>> split(const std::basic_string<T>& strSplit, T delim) {
				std::vector<std::basic_string<T>> result;
				std::basic_stringstream<T> ss(strSplit);
				std::basic_string<T> item;

				while (getline(ss, item, delim)) {
					result.push_back(item);
				}
				return result;
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
#define LOG_SET_FORMAT_C( formatter )  aricanli::general::Logger<char>::setFormatter(formatter)
#define LOG_SET_FILE_LIMIT_C( fileLimit) aricanli::general::Logger<char>::setFileLimit(fileLimit)
#define LOG_SET_OUTPUT_C( path) \
		   aricanli::general::Logger<char>::setLogOutput(path); \
			aricanli::general::Logger<char>::setLogFormat()
#define LOG_SET_PRIORITY_C( severity ) \
		 aricanli::general::Logger<char>::setLogPriority( static_cast<aricanli::general::LogPriority>(severity))

#define LOG_FATAL_C( ... )   aricanli::general::Logger<char>::log(aricanli::general::LogPriority::Fatal, __VA_ARGS__ )
#define LOG_ERROR_C(  ... )  aricanli::general::Logger<char>::log(aricanli::general::LogPriority::Error, __VA_ARGS__ )
#define LOG_WARNING_C( ... ) aricanli::general::Logger<char>::log(aricanli::general::LogPriority::Warning, __VA_ARGS__ )
#define LOG_INFO_C( ...)     aricanli::general::Logger<char>::log(aricanli::general::LogPriority::Info, __VA_ARGS__ )
#define LOG_VERBOSE_C( ... ) aricanli::general::Logger<char>::log(aricanli::general::LogPriority::Verbose, __VA_ARGS__ )
#define LOG_DEBUG_C( ... )   aricanli::general::Logger<char>::log(aricanli::general::LogPriority::Debug, __VA_ARGS__ )
#define LOG_TRACE_C( ... )   aricanli::general::Logger<char>::log(aricanli::general::LogPriority::Trace, __VA_ARGS__ )


#define LOG_SET_FORMAT_W( formatter ) aricanli::general::Logger<wchar_t>::setFormatter(formatter)
#define LOG_SET_OUTPUT_W( path) \
		   aricanli::general::Logger<wchar_t>::setLogOutput(path); \
		aricanli::general::Logger<wchar_t>::setLogFormat()
#define LOG_SET_FILE_LIMIT_W( fileLimit) \
		aricanli::general::Logger<wchar_t>::setFileLimit(fileLimit)

#define LOG_SET_PRIORITY_W(severity)\
	 aricanli::general::Logger<wchar_t>::setLogPriority( static_cast<aricanli::general::LogPriority>(severity))

#define LOG_FATAL_W( ... )   aricanli::general::Logger<wchar_t>::log(aricanli::general::LogPriority::Fatal, __VA_ARGS__ )
#define LOG_ERROR_W( ... )   aricanli::general::Logger<wchar_t>::log( aricanli::general::LogPriority::Error, __VA_ARGS__ )
#define LOG_WARNING_W( ... ) aricanli::general::Logger<wchar_t>::log(aricanli::general::LogPriority::Warning, __VA_ARGS__ )
#define LOG_INFO_W( ...)     aricanli::general::Logger<wchar_t>::log(aricanli::general::LogPriority::Info, __VA_ARGS__ )
#define LOG_VERBOSE_W( ... ) aricanli::general::Logger<wchar_t>::log(aricanli::general::LogPriority::Verbose, __VA_ARGS__ )
#define LOG_DEBUG_W( ... )   aricanli::general::Logger<wchar_t>::log(aricanli::general::LogPriority::Debug, __VA_ARGS__ )
#define LOG_TRACE_W( ... )   aricanli::general::Logger<wchar_t>::log(aricanli::general::LogPriority::Trace, __VA_ARGS__ )


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
 