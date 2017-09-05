//////////////////////////////////////////////////////////////////////////
//	中国科学院遥感与数字地球研究所
//  沈占锋
//	shenzf@radi.ac.cn
//////////////////////////////////////////////////////////////////////////
#ifndef MSMapException_h__
#define MSMapException_h__


#include <string>
#include <exception>


//************************************************************************
#define MSMAP_THROW(ExClass, args, ...)	\
	do									\
	{									\
		ExClass e(args);				\
		e.Init(__FILE__, __FUNCTION__, __LINE__);\
		throw e;						\
	}									\
	while(false)

#define MSMAP_DEFINE_EXCEPTION(ExClass, Base)			\
	public:												\
		ExClass(const std::string& msg = "") throw() : Base(msg){};\
		~ExClass() throw(){};							\
		std::string GetClassName() const				\
		{												\
			return #ExClass;							\
		};
//************************************************************************



using namespace std;


#ifdef MHMAPRENDER_EXPORTS
#define MHMAPVIEWOPRIMPL __declspec(dllexport)
#else
#define MHMAPVIEWOPRIMPL __declspec(dllimport)
#endif

class MHMAPVIEWOPRIMPL MSMapException : public exception
{
public:
	MSMapException(const std::string& msg = "") throw();
	virtual ~MSMapException() throw();

	void Init(const char* file, const char* func, int line);
	virtual string GetClassName() const;
	virtual string GetMessage() const;
	const char* what() const throw();
	const string& ToString() const;
	string GetStackTrace() const;

protected:
	string		mMsg;
	const char* mFile;
	const char* mFunc;
	int			mLine;

private:
	enum
	{ 
		MAX_STACK_TRACE_SIZE = 50 
	};

	void*	mStackTrace[MAX_STACK_TRACE_SIZE];
	size_t	mStackTraceSize;
	mutable	string mWhat;
};



//************************************************************************
//
// class MSMInvalidValueExcption 非法值异常
//
//************************************************************************
class MHMAPVIEWOPRIMPL MSMInvalidValueExcption : public MSMapException
{
	MSMAP_DEFINE_EXCEPTION(MSMInvalidValueExcption, MSMapException);
};
#endif // MSMapException_h__