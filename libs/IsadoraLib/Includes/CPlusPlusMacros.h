// ===========================================================================
//	CPlusPlusMacros				 ©2009 Mark F. Coniglio. All rights reserved.
// ===========================================================================

#ifndef _H_CPLUSPLUSMACROS
#define _H_CPLUSPLUSMACROS

#ifdef __cplusplus
	#define CLASSHDR			class
	#define PUBLIC				public:
	#define PRIVATE				private:
	#define PROTECTED			protected:
#else
	#define CLASSHDR			typedef struct
	#define PUBLIC		
	#define PRIVATE		
	#define PROTECTED
#endif

#endif