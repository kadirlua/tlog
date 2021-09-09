#pragma once
#include <sstream>
#include <chrono>
#include <type_traits> 

namespace aricanli {
	namespace general {
		// formatType enum class
		// possible ways to format type
		enum class formatType {
			LINE,
			MSG,
			TIME,
			FUNC
		};

#ifndef _T
#define __T(x)      L ## x
#define _T(x)       __T(x)
#endif

#define _CPP_CONCAT(x, y) x ## y
#define  CPP_CONCAT(x, y) _CPP_CONCAT(x, y)

		template<size_t SZ0, size_t SZ1>
		constexpr auto  _stringlit(char c,
			const char(&s0)[SZ0],
			const wchar_t(&s1)[SZ1]) -> const char(&)[SZ0]
		{
			return s0;
		}

			template<size_t SZ0, size_t SZ1>
		constexpr auto  _stringlit(wchar_t c,
			const char(&s0)[SZ0],
			const wchar_t(&s1)[SZ1]) -> const wchar_t(&)[SZ1]
		{
			return s1;
		}

#define stringlit(code_unit, lit) \
	_stringlit(code_unit (), lit, CPP_CONCAT(L, lit))

			
		// Class Formatter
		// Log message formatter class
		// f :function name  / l : line / m : message / t : time 
		// Example : 
		// Formatter fmt;
		// fmt.getFormatter("%t %l %m %t")
		// auto fmt = fmt.format(line,func,...args);
		template<typename T>
		class Formatter {
		public:
			Formatter() {
			}
			Formatter(const Formatter&) noexcept = delete;
			Formatter& operator=(const Formatter&) = delete;

			static void getFormatter(std::basic_string<T> fmt) {
				/*
				* Get format type as basic_string<T>
				* @param fmtStr : basic_string<T>
				*/
				m_fmt = fmt;
			}


			template<typename ...Args>
			static std::basic_string<T> format( Args &&...args) {
				/*
				* Format given arguments with defined parameters and return as basic_string<T>
				* @param line : __LINE__ macro
				* @param func : __FILE__ macro
				* @param ...args: Variadic template arguments
				*/

				std::basic_string<T> t_format = m_fmt;

				// Message ->
				size_t foundMsg = t_format.find(stringlit(T, "%m")); // char , wchar_t
				if (foundMsg != std::string::npos) {
					std::basic_ostringstream<T> oss;
					using unused = int[];

					(void)unused {
						0, (oss << args << " ", 0)...
					};
					t_format = findAndReplace(std::move(t_format), stringlit(T, "%m"), oss.str()); // char , wchar_t
				}


				// Time ->
				size_t foundTime = t_format.find(stringlit(T, "%t")); // char , wchar_t
				if (foundTime != std::string::npos) {

					std::basic_ostringstream<T> oss;
#if __cplusplus >= 201703L
					if constexpr (std::is_same_v<T, char>) {
						oss << timePointAsString(std::chrono::system_clock::now()).c_str() << " ";
					}
#else
					if (std::is_same_v<T, char>) {
						oss << timePointAsString(std::chrono::system_clock::now()).c_str() << " ";
					}
#endif			


#if __cplusplus >= 201703L
					if constexpr (std::is_same_v<T, wchar_t>) {
						oss << timePointAsWString(std::chrono::system_clock::now()).c_str() << " ";
					}
#else
					if (std::is_same_v<T, wchar_t>) {
						oss << timePointAsWString(std::chrono::system_clock::now()).c_str() << " ";
					}
#endif	

					t_format = findAndReplace(std::move(t_format), stringlit(T, "%t"), oss.str()); // char , wchar_t
				}

				return t_format;
			}


				
		protected:
			static std::basic_string<T> findAndReplace(std::basic_string<T> t_format, const std::basic_string<T>& t_find, const std::basic_string<T>& t_replace) {
				t_format.replace(t_format.find(t_find), t_find.length(), t_replace);
				return std::move(t_format);
			}

			static std::wstring wtos(std::string& value) {
				/*
				* take basic_string<char> and convert to basic_string<wchar_t> format
				* param : basic_string<char>
				* return : basic_string<wchar_t>
				*/
				const size_t cSize = value.size() + 1;
				std::wstring wc;
				wc.resize(cSize);
				size_t cSize1;
				mbstowcs_s(&cSize1, (wchar_t*)&wc[0], cSize, value.c_str(), cSize);
				wc.pop_back();

				return wc;
			}

			static std::basic_string<char> timePointAsString(const std::chrono::system_clock::time_point& tp) {
				/*
				* take time point from argument, convert to basic_string<char> format and return
				* param : chrono::system_clock::timepoint
				* return : basic_string<char>
				*/
				std::time_t t = std::chrono::system_clock::to_time_t(tp);
				char tmBuff[30];
				ctime_s(tmBuff, sizeof(tmBuff), &t);
				std::basic_string<char> ts = tmBuff;
				ts.resize(ts.size() - 1);
				return ts;
			}

			static std::basic_string<wchar_t> timePointAsWString(const std::chrono::system_clock::time_point& tp) {
				/*
				* take time point from argument, convert to basic_string<wchar_t> format and return
				* param : chrono::system_clock::timepoint
				* return : basic_string<wchar_t>
				*/
				std::time_t t = std::chrono::system_clock::to_time_t(tp);
				char tmBuff[30];
				ctime_s(tmBuff, sizeof(tmBuff), &t);
				std::basic_string<char> ret = tmBuff;
				std::basic_string<wchar_t> ts = wtos(ret);
				ts.resize(ts.size() - 1);
				return ts;
			}

		protected:
			static std::basic_string<T> m_fmt;
		}; // end of class


		// Intialize static data members
		std::basic_string<char> Formatter<char>::m_fmt;
		std::basic_string<wchar_t> Formatter<wchar_t>::m_fmt;

	} // end of general namespace
} // end of aricanli namespace