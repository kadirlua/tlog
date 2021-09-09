#include <iostream>
#include "Logger.h"

using namespace aricanli::general;


int main() {

	LoggerW::setLogOutput(L"log/log.txt");
	LoggerW::setLogPriority(LogPriority::Trace);
	auto log = LoggerW::getInstance();


	log->log(LogPriority::Fatal, L"Привет, мир");
	log->log(LogPriority::Error,L"नमस्ते दुनिया");
	log->log(LogPriority::Warning, L"Γειά σου Κόσμε");
	log->log(LogPriority::Info, L"こんにちは世界");


	return 0;
}
