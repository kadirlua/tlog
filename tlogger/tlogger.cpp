#include <iostream>
#include <thread>
#include <memory>
#include "Logger.h"

using namespace aricanli::general;

#define WLOG_TEST 1


int main() {

#if WLOG_TEST

	Logger<wchar_t>::setLogOutput(L"log", L"txt");
	Logger<wchar_t>::setLogPriority(LogPriority::Debug);
	auto log = Logger<wchar_t>::getInstance();
	log->setFormatter(L"%m %t %l %f");

	auto start1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Error, __LINE__, _T(__FILE__), L"tellus felis condimentum odio, : ", 1, 'A', L" porta tincidunt lectus turpis id sem. Integer et est neque.", L"Vestibulum at maximus ante. ");
	auto finish1 = std::chrono::high_resolution_clock::now();

	auto start2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Warning, __LINE__, _T(__FILE__), L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ");
	auto finish2 = std::chrono::high_resolution_clock::now();
	auto start3 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Info, __LINE__, _T(__FILE__), L" mattis dolor id,  : ", L" vestibulum nibh.", L"Nulla in auctor leo. ");
	auto finish3 = std::chrono::high_resolution_clock::now();
	auto start5 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Warning, __LINE__, _T(__FILE__), L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ");
	auto finish5 = std::chrono::high_resolution_clock::now();
	auto start6 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Info, __LINE__, _T(__FILE__), L" mattis dolor id,  : ", 2.712, 'z', L" vestibulum nibh.", L"Nulla in auctor leo. ");
	auto finish6 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed1 = finish1 - start1;
	std::chrono::duration<double> elapsed2 = finish2 - start2;
	std::chrono::duration<double> elapsed3 = finish3 - start3;

	std::chrono::duration<double> elapsed5 = finish5 - start5;
	std::chrono::duration<double> elapsed6 = finish6 - start6;

	std::cout << "Error : " << elapsed1.count() << "\nWarning : " << elapsed2.count() <<
		"\nInfo : " << elapsed3.count() << "\nWarning" << elapsed5.count() <<
		"\nInfo : " << elapsed6.count() << "\n";


#else 
	/* Create log ->*/

	Logger<char>::setLogOutput("log.txt");
	Logger<char>::setLogPriority(LogPriority::Debug);
	auto log = Logger<char>::getInstance();

	auto start1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Error, __LINE__, "tellus felis condimentum odio, : ", 1, 'A', "porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
	auto finish1 = std::chrono::high_resolution_clock::now();
	auto start2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Warning, __LINE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	auto finish2 = std::chrono::high_resolution_clock::now();
	auto start3 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Info, __LINE__, " mattis dolor id,  : ", " vestibulum nibh.", "Nulla in auctor leo. ");
	auto finish3 = std::chrono::high_resolution_clock::now();

	auto start5 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Warning, __LINE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	auto finish5 = std::chrono::high_resolution_clock::now();
	auto start6 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100'000; i++)
		log->log(LogPriority::Info, __LINE__, " mattis dolor id,  : ", 2.712, 'a', " vestibulum nibh.", "Nulla in auctor leo. ");
	auto finish6 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed1 = finish1 - start1;
	std::chrono::duration<double> elapsed2 = finish2 - start2;
	std::chrono::duration<double> elapsed3 = finish3 - start3;

	std::chrono::duration<double> elapsed5 = finish5 - start5;
	std::chrono::duration<double> elapsed6 = finish6 - start6;

	std::cout << "Error : " << elapsed1.count() << "\nWarning : " <<
		elapsed2.count() << "\nInfo : " << elapsed3.count() << "\n" <<
		elapsed5.count() << "\nInfo : " << elapsed6.count() << "\n";
#endif


	return 0;
}
