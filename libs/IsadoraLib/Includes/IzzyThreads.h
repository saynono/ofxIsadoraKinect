// ===========================================================================
// IzzyThreads					� 2006 Mark F. Coniglio.  all rights reserved.
// ===========================================================================
// Cross platform threading for Mac OS X and Windows

#ifndef _H_IzzyThreads
#define _H_IzzyThreads

class CMutex;

#ifdef __cplusplus
extern "C" {
#endif

#if TARGET_OS_MAC

	#include <pthread.h>
	#include <mach/mach_init.h>
	#include <mach/thread_policy.h>

	#define	IZZY_USE_THREADS		1
	#define	msSleep(a)				usleep((a) * 1000);
	#define	IzzIsMainThread()		pthread_main_np()

#elif TARGET_OS_WIN32

	#define	IZZY_USE_THREADS		1
	#define	msSleep(a)				Sleep(a)

	extern DWORD gIzzyThreadsMainThreadID;
	#define	IzzIsMainThread()		( GetCurrentThreadId() == gIzzyThreadsMainThreadID )
#else

	#error "Platform Must Be Mac or Win!"

#endif

	enum IzzyThreadErr {
		IT_NoError					= 0,
		IT_UnknownErr				= -1,
		IT_CantCreate				= -2,
		IT_CantClose				= -3,
		IT_InvalidAttribute			= -4,
		IT_ThreadNotFound			= -5,
		IT_ThreadNotJoinable		= -6,
		IT_Deadlock					= -7,
		IT_UnknownError				= -8
	};
	
	// constant used by some plugins
	const int			IZZY_MAX_THREADS = 2;

	// define the localized type for a thread ID
	#if TARGET_OS_MAC
		typedef	pthread_t	IzzyThreadID;
	#elif TARGET_OS_WIN32
		typedef DWORD		IzzyThreadID;
	#endif

	// typedef for the thread execute function
	typedef int				(*IzzyThreadExecuteFunc)(IzzyThreadID inThreadID, void* inUserData);
	
	// typedef for the IzzyThreadInfo struct
	typedef struct {
		IzzyThreadExecuteFunc	mThreadFunc;
		void*					mUserData;
		IzzyThreadID			mThreadID;
		bool					mCanSignal;
		
		#if TARGET_OS_MAC
			pthread_cond_t			mCondition;
			pthread_mutex_t			mCondMutex;
			bool					mOwnsMutex;
		#elif TARGET_OS_WIN32
			HANDLE					mThreadHandle;
			HANDLE					mCondition;
		#endif
	
	} IzzyThreadInfo;
	
	// returns true if there are multiple processors
	// available on this machine
	bool	MultipleProcessorsAvailable();
	
	UInt32	GetProcessorCount();
	
	bool
	IsIzzyThreadActive(
		const IzzyThreadInfo*		inThreadInfo);
		
	// creates a new thread and immediately starts processing
	IzzyThreadErr
	IzzyCreateAndRunThread(
		IzzyThreadExecuteFunc	inFunction,
		void*					inUserData,
		bool					inRequiresSignal,
		CMutex*					inMutexObject,		// CAN BE NULL
		IzzyThreadInfo*			outThreadInfo);
	
	// waits for a thread to complete its task
	IzzyThreadErr
	IzzyWaitThreadCompleteAndDispose(
		IzzyThreadInfo*			inThreadInfo);

	// blocks until a signal is received from some other thread
	void
	IzzyDisposeThread(
		IzzyThreadInfo*			inThreadInfo);
	
	// blocks until a signal is received from some other thread
	bool
	IzzyThreadBlockUntilSignal(
		IzzyThreadInfo*			inThreadInfo);
	
	// blocks until a signal is received from some other thread
	bool
	IzzyThreadBlockUntilSignalOrTimeout(
		IzzyThreadInfo*			inThreadInfo,
		UInt32					inMilliseconds,
		bool*					outTimedOut = NULL);
	
	// blocks until a signal is received from some other thread
	void
	IzzyThreadSendSignal(
		IzzyThreadInfo*			inThreadInfo);
	
#ifdef __cplusplus
}
#endif

#endif
