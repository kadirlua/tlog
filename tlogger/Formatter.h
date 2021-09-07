
#pragma warning(disable : 4996)
#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <array>

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

		// formatPattern enum class
		// possible ways to format type
		enum class formatPattern {
			Plain,
			XML,
			JSON
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


			//static constexpr const std::basic_string<T> _LINE = stringlit(T,"<line>");
			//static constexpr const std::basic_string<T> _LINEEND = stringlit(T, "</line>");
			//static constexpr const std::basic_string<T> _MSG = stringlit(T, "<message>");
			//static constexpr const std::basic_string<T> _MSGEND = stringlit(T, "</message>");
			//static constexpr const std::basic_string<T> _FUNC = stringlit(T, "<function>");
			//static constexpr const std::basic_string<T> _FUNCEND = stringlit(T, "</function>");
			//static constexpr const std::basic_string<T> _TIME = stringlit(T, "<time>");
			//static constexpr const std::basic_string<T> _TIMEEND = stringlit(T, "</time>");
			//static constexpr const std::basic_string<T> _LOG = stringlit(T, "<log>");
			//static constexpr const std::basic_string<T> _LOGEND = stringlit(T, "</log>");





			Formatter() {
			}
			Formatter(const Formatter&) noexcept = delete;
			Formatter& operator=(const Formatter&) = delete;

			static void getFormatter(std::basic_string<T> fmt, formatPattern t_fmtpattern = formatPattern::Plain) {
				/*
				* Get format type as basic_string<T>
				* @param fmtStr : basic_string<T>
				*/
				m_fmt = fmt;
				m_fmtpattern = t_fmtpattern;
			}

			static formatType formatHash(const std::basic_string<T>& fmtStr) {
				/*
				* Hash arguments to formatType enum class
				* @param fmtStr : basic_string<T>
				* @return formatType enum class
				*/	
				if (fmtStr == stringlit(T, "%l"))
					return formatType::LINE;
				if (fmtStr == stringlit(T, "%m"))
					return formatType::MSG;
				if (fmtStr == stringlit(T, "%t"))
					return formatType::TIME;
				if (fmtStr == stringlit(T, "%f"))
					return formatType::FUNC;
			}

			static std::basic_string<T> findAndReplace(std::basic_string<T> t_format, const std::basic_string<T>& t_find, const std::basic_string<T>& t_replace) {
				t_format.replace(t_format.find(t_find), t_find.length(), t_replace);
				return std::move(t_format);
			}

			template<typename ...Args>
			static std::basic_string<T> format(int line, const std::basic_string<T>& func, Args &&...args) {
				/*
				* Format given arguments with defined parameters and return as basic_string<T>
				* @param line : __LINE__ macro
				* @param func : __FILE__ macro
				* @param ...args: Variadic template arguments
				*/
			
				
				if (m_fmtpattern == formatPattern::XML) {
					return formatXML(line, func, std::forward<Args>(args)...);
				}

				//if (m_fmtpattern == formatPattern::JSON) {
				//	std::basic_string<T> t_formattedStr;
				//	t_formattedStr = formatJSON(line, func, std::forward<Args>(args)...);
				//	return t_formattedStr;
				//}
				
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


				// Line ->
				size_t foundLine = t_format.find(stringlit(T, "%l")); // char , wchar_t
				if (foundLine != std::string::npos) {
					std::basic_ostringstream<T> oss;
					oss << line;
					//std::basic_string<T> t_line;
					//t_line = { stringlit(T, "20") }; // fix this line
					t_format = findAndReplace(std::move(t_format), stringlit(T, "%l"), oss.str()); // char , wchar_t

				}

				// Time ->
				size_t foundTime = t_format.find(stringlit(T, "%t")); // char , wchar_t
				if (foundTime != std::string::npos) {

					std::basic_ostringstream<T> oss;
					if (std::is_same_v<T, char>) {
						oss << timePointAsString(std::chrono::system_clock::now()).c_str() << " ";
					}
					if (std::is_same_v<T, wchar_t>) {
						oss << timePointAsWString(std::chrono::system_clock::now()).c_str() << " ";
					}
					t_format = findAndReplace(std::move(t_format), stringlit(T, "%t"), oss.str()); // char , wchar_t
				}

				// Function ->
				size_t foundFunc = t_format.find(stringlit(T, "%f"));// char , wchar_t
				if (foundFunc != std::string::npos) {
					t_format = findAndReplace(std::move(t_format), stringlit(T, "%f"), std::move(func)); // char , wchar_t
				}

				return t_format;
			}
				
				

			template<typename ...Args>
			static std::basic_string<T> formatXML(size_t line, const std::basic_string<T>& func, Args &&...args) {
				/*
				* Format given arguments in XML pattern 
				* with defined parameters and return as basic_string<T>
				* @param line : __LINE__ macro
				* @param func : __FILE__ macro
				* @param ...args: Variadic template arguments
				*/


				std::basic_string<T> t_format = m_fmt;
				std::basic_ostringstream<T> oss;
				// Message ->
				t_format.insert(0, stringlit(T, "<log>") );
				size_t foundMsg = t_format.find(stringlit(T, "%m")); // char , wchar_t
				if (foundMsg != std::string::npos) {
					using unused = int[];

					(void)unused {
						0, (oss << args << " ", 0)...
					};
					t_format.insert(foundMsg - 1 + 4, stringlit(T, "<message>"));
					std::basic_string<T> t_str = oss.str();
					int pos = t_str.length();
					t_format = findAndReplace(std::move(t_format), stringlit(T, "%m"), t_str); // char , wchar_t
					t_format.insert(foundMsg + pos + 10, stringlit(T, "</message>"));
				}

				size_t foundLine = t_format.find(stringlit(T, "%l")); // char , wchar_t
				if (foundLine != std::string::npos) {
					oss << line;
					std::basic_string<T> t_str = oss.str();
					t_format.insert(foundLine - 1, stringlit(T, "<line>"));
					t_format = findAndReplace(std::move(t_format), stringlit(T, "%l"), t_str); // char , wchar_t
					t_format.insert(foundLine + t_str.length() + 8 , stringlit(T, "</line>"));
				}

				 //Time ->
				size_t foundTime = t_format.find(stringlit(T, "%t")); // char , wchar_t
				if (foundTime != std::string::npos) {
					std::basic_ostringstream<T> oss;
					if (std::is_same_v<T, char>) {
						oss << timePointAsString(std::chrono::system_clock::now()).c_str() << " ";
					}
					if (std::is_same_v<T, wchar_t>) {
						oss << timePointAsWString(std::chrono::system_clock::now()).c_str() << " ";
					}
					t_format.insert(foundTime-1, stringlit(T, "<time>"));
					std::basic_string<T> t_str = oss.str();
					int pos = t_str.length();
					t_format = findAndReplace(std::move(t_format), stringlit(T, "%t"), t_str); // char , wchar_t
					t_format.insert(foundTime + pos+7, stringlit(T, "</time>"));
				}

				// Function ->
				size_t foundFunc = t_format.find(stringlit(T, "%f"));// char , wchar_t
				if (foundFunc != std::string::npos) {
					t_format.insert(foundFunc-1, stringlit(T, "<function>"));
					int pos = func.length();
					t_format = findAndReplace(std::move(t_format), stringlit(T, "%f"), std::move(func)); // char , wchar_t
					t_format.insert(foundFunc + pos + 12, stringlit(T, "</function>"));
				}
				t_format += stringlit(T, "</log>");
				return t_format;
			}

			template<typename ...Args>
			static std::basic_string<T> formatJSON(int line, const std::basic_string<T>& func, Args &&...args) {
				/*
				* Format given arguments with defined parameters and return in plain text mode
				* @param line : __LINE__ macro
				* @param func : __FILE__ macro
				* @param ...args: Variadic template arguments
				*/
	/*			std::basic_ostringstream<T> oss;
				oss << "{";
				for (const auto& fmtLine : m_lineOfString) {
					switch (formatHash(fmtLine)) {
					case formatType::TIME:
						oss << "\"time\":\"";
						if (std::is_same_v<T, char>) {
							oss << timePointAsString(std::chrono::system_clock::now()).c_str() << " ";
						}
						if (std::is_same_v<T, wchar_t>) {
							oss << timePointAsWString(std::chrono::system_clock::now()).c_str() << " ";
						}
						oss << "\",";
						break;
					case formatType::MSG:
						oss << "\"message\":\"";
						using unused = int[];

						(void)unused {
							0, (oss << args << " ", 0)...
						};
						oss << "\",";
						break;
					case formatType::LINE:
						oss << "\"line\":\"" << line << "\",";
						break;
					case formatType::FUNC:
						oss << "\"function\":\"" << func << "\",";
						break;

					}
				}
				oss << "},\n";
				return oss.str();*/
			}

			static void split(std::basic_string<T>& t_fmt,  T t_delim) {
				/*
				* split member data with delimiter from argument
				* param : template parameter
				*/
				std::basic_stringstream<T> ss(t_fmt);
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
			static formatPattern m_fmtpattern;
		}; // end of class


		// Intialize static data members
		std::vector<std::basic_string<char>> Formatter<char>::m_lineOfString;
		std::vector<std::basic_string<wchar_t>> Formatter<wchar_t>::m_lineOfString;
		std::basic_string<char> Formatter<char>::m_fmt;
		std::basic_string<wchar_t> Formatter<wchar_t>::m_fmt;
		formatPattern Formatter<char>::m_fmtpattern = formatPattern::Plain;
		formatPattern Formatter<wchar_t>::m_fmtpattern = formatPattern::Plain;

	} // end of general namespace
} // end of aricanli namespace