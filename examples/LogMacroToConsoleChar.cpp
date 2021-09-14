#include <iostream>
#include "Logger.h"

using namespace aricanli::general;

int main() {

	LOG_SET_FORMAT_C(" %m %t ");

	LOG_SET_PRIORITY_C(56);
	LOG_QUIET();

	LOG_FATAL_C(__LINE__, __FILE__, "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", "Sed ornare commodo urna, eget sollicitudin ex eleifend ut.  ");
	LOG_ERROR_C(__LINE__, __FILE__, "tellus felis condimentum odio, : ", 1, 'A', "porta tincidunt lectus turpis id sem. Integer et est neque.", "Vestibulum at maximus ante. ");
	LOG_WARNING_C(__LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	LOG_INFO_C(__LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	LOG_VERBOSE_C(__LINE__, __FILE__, " mattis dolor id,  : ", " vestibulum nibh.", "Nulla in auctor leo. ");
	LOG_DEBUG_C(__LINE__, __FILE__, "Nunc a varius purus. : ", 3.14159, "Proin dictum erat sit amet posuere tristique. ", "Fusce a arcu rhoncus, ");
	LOG_TRACE_C(__LINE__, __FILE__, " mattis dolor id,  : ", 2.712, 'a', " vestibulum nibh.", "Nulla in auctor leo. ");

	return 0;
}
