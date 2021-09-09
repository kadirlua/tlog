#include <iostream>
#include "Logger.h"

using namespace aricanli::general;


int main() {

	LOG_SET_FORMAT_W(L"%m %t")

	LOG_SET_PRIORITY_VERBOSE_W()
	LOG_QUIET()
	LOG_FATAL_W(__LINE__, L"Lorem ipsum dolor sit amet, consectetur adipiscing elit.", L"Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  ")
	LOG_ERROR_W(__LINE__, L"tellus felis condimentum odio, : ", 1, 'A', L"porta tincidunt lectus turpis id sem. Integer et est neque.", L"Vestibulum at maximus ante. ")
	LOG_WARNING_W(__LINE__, L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ")
	LOG_INFO_W(__LINE__,  L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ")
	LOG_VERBOSE_W(__LINE__, L" mattis dolor id,  : ", L" vestibulum nibh.", "Nulla in auctor leo. ")
	LOG_DEBUG_W(__LINE__, L"Nunc a varius purus. : ", 3.14159, L"Proin dictum erat sit amet posuere tristique. ", L"Fusce a arcu rhoncus, ")
	LOG_TRACE_W(__LINE__, L" mattis dolor id,  : ", 2.712, 'a', L" vestibulum nibh.", L"Nulla in auctor leo. ")

	return 0;
}
