
#pragma warning(disable : 4996)
#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>

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

			static formatType formatHash(const std::basic_string<T>& fmtStr) {
				/*
				* Hash arguments to formatType enum class
				* @param fmtStr : basic_string<T>
				* @return formatType enum class
				*/
				//if (std::is_same_v<T, char>) {
				//	if (fmtStr == "%l")
				//		return formatType::LINE;
				//	if (fmtStr == "%m")
				//		return formatType::MSG;
				//	if (fmtStr == "%t")
				//		return formatType::TIME;
				//	if (fmtStr == "%f")
				//		return formatType::FUNC;
				//}
				if (std::is_same_v<T, wchar_t>) {
					if (fmtStr == _T("%l"))
						return formatType::LINE;
					if (fmtStr == _T("%m"))
						return formatType::MSG;
					if (fmtStr == _T("%t"))
						return formatType::TIME;
					if (fmtStr == _T("%f"))
						return formatType::FUNC;
				}
			}
			template<typename ...Args>
			static std::basic_string<T> format(int line, const std::basic_string<T>& func, Args &&...args) {
				/*
				* Format given arguments with defined parameters and return as basic_string<T>
				* @param line : __LINE__ macro
				* @param func : __FILE__ macro
				* @param ...args: Variadic template arguments
				*/
				std::basic_ostringstream<T> oss;
				oss << "<log>\n";
				for (const auto& fmtLine : m_lineOfString) {
					switch (formatHash(fmtLine)) {
					case formatType::TIME:
						oss << "\t<time>";
						if (std::is_same_v<T, char>) {
							oss << timePointAsString(std::chrono::system_clock::now()).c_str() << " ";
						}
						if (std::is_same_v<T, wchar_t>) {
							oss << timePointAsWString(std::chrono::system_clock::now()).c_str() << " ";
						}
						oss << "<\\time>\n";
						break;
					case formatType::MSG:
						oss << "\t<message>";
						using unused = int[];

						(void)unused {
							0, (oss << args << " ", 0)...
						};
						oss << "<\\message>\n";
						break;
					case formatType::LINE:
						oss << "\t<line>" << line << "<\\line>\n";
						break;
					case formatType::FUNC:
						oss << "\t<function>" << func << "<\\function>\n";
						break;

					}
				}
				oss << "<\\log>\n";
				return oss.str();
			}

			static void split(T t_delim) {
				/*
				* split member data with delimiter from argument
				* param : template parameter
				*/
				std::basic_stringstream<T> ss(m_fmt);
				std::basic_string<T> item;

				while (getline(ss, item, t_delim))
					m_lineOfString.push_back(item);
			}
		protected:
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
				std::basic_string<char> ts = std::ctime(&t);
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
				std::basic_string<char> ret = std::ctime(&t);
				std::basic_string<wchar_t> ts = wtos(ret);
				ts.resize(ts.size() - 1);
				return ts;
			}

		protected:
			static std::vector<std::basic_string<T>> m_lineOfString;
			static std::basic_string<T> m_fmt;
		}; // end of class


		// Intialize static data members
		std::vector<std::basic_string<char>> Formatter<char>::m_lineOfString;
		std::vector<std::basic_string<wchar_t>> Formatter<wchar_t>::m_lineOfString;
		std::basic_string<char> Formatter<char>::m_fmt;
		std::basic_string<wchar_t> Formatter<wchar_t>::m_fmt;

	} // end of general namespace
} // end of aricanli namespace