#include <iostream>
#include "Logger.h"

using namespace aricanli::general;

int main() {

	Logger<char>::setLogOutput("basicLog/log/log.txt");
	Logger<char>::setLogPriority(LogPriority::Debug);
	auto log = Logger<char>::getInstance();
	log->setFileLimit(10*1024*1024);
	log->setFormatter("%t %m");
	log->log(LogPriority::Quiet);

	auto start1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Quiet);
	auto finish1 = std::chrono::high_resolution_clock::now();

	auto start2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Fatal, __LINE__, __FILE__, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", "Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  ");
	auto finish2 = std::chrono::high_resolution_clock::now();

	auto start3 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Error, __LINE__, __FILE__, "tellus felis condimentum odio, : ", 1, 'A', "porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
	auto finish3 = std::chrono::high_resolution_clock::now();

	auto start4 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Warning, __LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	auto finish4 = std::chrono::high_resolution_clock::now();

	auto start5 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Info, __LINE__, __FILE__, " mattis dolor id,  : ", " vestibulum nibh.", "Nulla in auctor leo. ");
	auto finish5 = std::chrono::high_resolution_clock::now();

	auto start6 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Verbose, __LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	auto finish6 = std::chrono::high_resolution_clock::now();

	auto start7 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Debug, __LINE__, __FILE__, " mattis dolor id,  : ", 2.712, 'a', " vestibulum nibh.", "Nulla in auctor leo. ");
	auto finish7 = std::chrono::high_resolution_clock::now();

	auto start8 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Trace, __LINE__, __FILE__, " mattis dolor id,  : ", 2.712, 'a', " vestibulum nibh.", "Nulla in auctor leo. ");
	auto finish8 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed1 = finish1 - start1;
	std::chrono::duration<double> elapsed2 = finish2 - start2;
	std::chrono::duration<double> elapsed3 = finish3 - start3;
	std::chrono::duration<double> elapsed4 = finish4 - start4;
	std::chrono::duration<double> elapsed5 = finish5 - start5;
	std::chrono::duration<double> elapsed6 = finish6 - start6;
	std::chrono::duration<double> elapsed7 = finish7 - start7;
	std::chrono::duration<double> elapsed8 = finish8 - start8;


	std::cout << "Quiet : " << elapsed1.count() << "\nFatal : " << elapsed2.count()
			<< "\nError : " << elapsed3.count() << "\nWarning" << elapsed4.count()
			<< "\nInfo : " << elapsed5.count() << "\nVerbose" << elapsed6.count()
			<< "\nDebug : " << elapsed7.count() << "\nTrace" << elapsed8.count();

	return 0;
}
