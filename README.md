Fundamentals :
	 - Set output stream : 
	    * for stream to file ->
		Logger<wchar_t>::setLogOutput(L"log", L"txt");
	    * for stream to console don't call setLogOutput function. 
	- Set priority level
		Logger<wchar_t>::setLogPriority(LogPriority::Debug);
	- Create Logger class instance :
		auto log = Logger<wchar_t>::getInstance();
	- set format 
	- %m : message ,  %t : time , %l : line , %f : function	
	 	 log->setFormatter(L"%m %t %l %f");
