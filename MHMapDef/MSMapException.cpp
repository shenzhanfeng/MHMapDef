#include "stdafx.h"

#include "MSMapException.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
//#include <cxxabi.h>
//#include <execinfo.h>


MSMapException::MSMapException(const std::string& msg) throw()
	: mMsg(msg), mFile("<unknown file>"), mFunc("<unknown func>"), mLine(-1), mStackTraceSize(0)
{

}


MSMapException::~MSMapException() throw()
{

}


void MSMapException::Init(const char* file, const char* func, int line)
{
	mFile = file;
	mFunc = func;
	mLine = line;

	//mStackTraceSize = backtrace(mStackTrace, MAX_STACK_TRACE_SIZE);
}


string MSMapException::GetClassName() const
{
	return "MSMapException";
}


const char* MSMapException::what() const throw()
{
	return ToString().c_str();
}


const string& MSMapException::ToString() const
{
	if(mWhat.empty())
	{
		stringstream sstr("");
		if(mLine > 0)
		{
			sstr<<mFile<<"("<<mLine<<")";
		}
		sstr<<": "<<GetClassName();
		if(!GetMessage().empty())
		{
			sstr<<": "<<GetMessage();
		}
		sstr<<"\nStack Trace:\n";
		sstr<<GetStackTrace();

		mWhat = sstr.str();
	}
	return mWhat;
}


std::string MSMapException::GetMessage() const
{
	return mMsg;
}


string MSMapException::GetStackTrace() const
{
	if(mStackTraceSize == 0) return "<No stack trace>\n";

	//char** strings = backtrace_symbols(mStackTrace, 10);
	//if(strings == NULL) // Since this is for debug only thus non-critical, don't throw an exception.
	//	return "<Unknown error: backtrace_symbols returned NULL>\n";

	//string result;
	//for(size_t i = 0; i < mStackTraceSize; ++i)
	//{
	//	std::string mangledName = strings[i];
	//	std::string::size_type begin = mangledName.find('(');
	//	std::string::size_type end = mangledName.find('+', begin);

	//	if(begin == std::string::npos || end == std::string::npos)
	//	{
	//		result += mangledName;
	//		result += '\n';
	//		continue;
	//	}
	//	++begin;

	//	int status;
	//	char* s = abi::__cxa_demangle(mangledName.substr(begin, end-begin).c_str(), NULL, 0, &status);
	//	if(status != 0)
	//	{
	//		result += mangledName;
	//		result += '\n';
	//		continue;
	//	}
	//	std::string demangledName(s);
	//	free(s);

	//	// Ignore ExceptionBase::Init so the top frame is the
	//	// user's frame where this exception is thrown.
	//	//
	//	// Can't just ignore frame#0 because the compiler might
	//	// inline ExceptionBase::Init.
	//	result += mangledName.substr(0, begin);
	//	result += demangledName;
	//	result += mangledName.substr(end);
	//	result += '\n';
	//}

	//free(strings);

	//return result;

	return "Î´ÊµÏÖ";
}
