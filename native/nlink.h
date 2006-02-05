#pragma once

#include "com4j.h"

const int BYREF = 0x8000;

// see nlink.NativeType
enum Conv {
	cvBSTR = 1,
	cvBSTR_byRef = 1|BYREF,
	cvLPCWSTR = 2,
	cvLPCSTR = 3,


	cvINT8 = 100,
	cvINT8_byRef = 100|BYREF,
	cvINT16 = 101,
	cvINT16_byRef = 101|BYREF,
	cvINT32 = 102,
	cvINT32_byRef = 102|BYREF,
	cvBool = 103,
	cvVariantBool = 104,
	cvVariantBool_byRef = 104|BYREF,
	cvFloat = 120,
	cvDouble = 121,

	cvHRESULT = 200,

	cvComObject = 300,
	cvComObject_byRef = 300|BYREF,
	cvGUID = 301,
	cvVARIANT = 302,
	cvVARIANT_byRef = 302|BYREF,
	cvDISPATCH = 303,
	cvPVOID = 304,

	cvDATE = 400,

	cvSAFEARRAY = 500,
};


class Environment {
	// JNI environment
	JNIEnv* const		env;
	// post actions
	PostAction* postActions;
public:
	Environment( JNIEnv* _env ) : env(_env) {
		postActions = NULL;
	}
	~Environment();

	// invoke a method
	jobject invoke(
		void* function,		// pointer to the function to invoke
		jobjectArray	args,	// arguments
		jint*			convs,	// conversions
		jclass			retType,
		jint			retConv
	);

	// adds a new post action
	void add( PostAction* cu );

	// get the char array from jstring and schedule a clean up
	const jchar* toChars( jstring s );

	// get the \0-terminated wide string
	const wchar_t* toLPCWSTR( jstring s );

	// get the \0-terminated string
	LPCSTR toLPCSTR( jstring s );

	BSTR toBSTR( jstring s );
};


// throw a Java NLinkException
void error( JNIEnv* env, const char* fileName, int line, const char* msg ... );
void error( JNIEnv* env, const char* fileName, int line, HRESULT hr, const char* msg ... );
