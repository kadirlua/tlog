## tlog
header only C++ logging library
## Usage
####  1-Set output stream : 
 for stream to file 
 <br/> `Logger<wchar_t>::setLogOutput(L"log", L"txt");`
 <br/> for stream to console no need to call setLogOutput function. Stream to console as default. <br/>
#### 2-Set priority level
`void setLogPriority(LogPriority t_logPriority); `
<br/> t_logPriority : logger severity upper limit. All log messages have its own severity and if it is higher than the limit those messages are dropped.
<br/> `Logger<wchar_t>::setLogPriority(LogPriority::Debug); `

 #### 3-Create instance
Create an singleton instance,, which ensures that only one object of its kind exists and provides a single point
<br/> ` auto log = Logger<char>::getInstance(); `

 #### Set format (Optional)
 Formatters specify the layout of log records in the final output.
<br/>`log->setFormatter("%m %t %l %f"); `
<br/> %m : message ,  %t : time , %l : line , %f : function	<br/>
`
 
 #### Set limit size of file (Optional)
	 ` log->setFileLimit(100); `

#### 4-Log
 ` log->log(LogPriority::Error, __LINE__, __FILE__, "tellus felis condimentum odio, : " `
 
 ## Basic Sample 
 ` 	Logger<char>::setLogOutput("log", "txt"); `
<br/>  ` 	Logger<char>::setLogPriority(LogPriority::Debug); `
<br/>  ` 	auto log = Logger<char>::getInstance(); `
<br/>  ` 	log->setFileLimit(100); `
<br/>  ` 	log->setFormatter("%f %t %l %m"); `
<br/>  ` 	log->log(LogPriority::Error, __LINE__, __FILE__, "tellus felis condimentum odio, : " ); `
