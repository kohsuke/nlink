// nlink.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "java_symbols.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}



void error( JNIEnv* env, const char* file, int line, const char* msg ... ) {
	// format the message
	va_list va;
	va_start(va,msg);

	int len = _vscprintf(msg,va);
	char* w = reinterpret_cast<char*>(alloca(len+1)); // +1 for '\0'
	vsprintf(w,msg,va);

	env->ExceptionClear();
	env->Throw( (jthrowable)nlinkexception_new( env, env->NewStringUTF(w), env->NewStringUTF(file), line ) );
}
