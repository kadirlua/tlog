#include <iostream>
#include "Logger.h"

using namespace aricanli::general;

int main() {

	LoggerW::setLogOutput(L"log.txt");
	LoggerW::setLogPriority(LogPriority::Trace);
	auto log = LoggerW::getInstance();
	log->setFileLimit(1024);
	log->setFormatter(L"%t %m");
	log->log(LogPriority::Quiet);

	log->log(LogPriority::Quiet);
	log->log(LogPriority::Fatal, __LINE__, _T(__FILE__), L"Lorem ipsum dolor sit amet, consectetur adipiscing elit.", L"Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  ");
	log->log(LogPriority::Error, __LINE__, _T(__FILE__), L"tellus felis condimentum odio, : ", 1, 'A', L"porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
	log->log(LogPriority::Warning, __LINE__, _T(__FILE__), L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ");
	log->log(LogPriority::Info, __LINE__, _T(__FILE__), " mattis dolor id,  : ", " vestibulum nibh.", "Nulla in auctor leo. ");
	log->log(LogPriority::Verbose, __LINE__, _T(__FILE__), L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ");
	log->log(LogPriority::Debug, __LINE__, _T(__FILE__), L" mattis dolor id,  : ", 2.712, 'a', L" vestibulum nibh.", L"Nulla in auctor leo. ");
	log->log(LogPriority::Trace, __LINE__, _T(__FILE__), L" mattis dolor id,  : ", 2.712, 'a', L" vestibulum nibh.", L"Nulla in auctor leo. ");


	return 0;
}
