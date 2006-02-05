#include "stdafx.h"
#include "javah/nlink_Native.h"
#include "nlink.h"


JNIEXPORT jint JNICALL Java_nlink_win32_Native_loadLibrary(JNIEnv* env, jclass __unused__, jstring _name) {
	return reinterpret_cast<jint>(::LoadLibrary(JString(env,_name)));
}

JNIEXPORT jint JNICALL Java_nlink_win32_Native_getLastError(JNIEnv* env, jclass __unused__) {
	return ::GetLastError();
}

JNIEXPORT jstring JNICALL Java_nlink_win32_Native_formatErrorMessage(JNIEnv* env, jclass __unused__, jint code) {
	LPWSTR p;

	DWORD r = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, code,
		0, reinterpret_cast<LPWSTR>(&p), 0, NULL );

	if(r==0)
		return NULL;	// failed

	// trim off the trailing NL
	int len = wcslen(p);
	while(len>0 && (p[len-1]==L'\r' || p[len-1]==L'\n'))
		len--;

	jstring result = env->NewString(p,len);
	LocalFree(p);

	return result;
}

JNIEXPORT jobject JNICALL Java_nlink_win32_Native_invoke(JNIEnv* env, jclass __unused__ , jint functionPtr, jobjectArray args, jintArray _convs, jclass retType, jint retConv) {
	Environment e(env);
	jint* convs =  env->GetIntArrayElements(_convs,NULL);
	jobject r = e.invoke(
		reinterpret_cast<void*>(functionPtr),
		args,
		convs,
		retType,
		retConv );
	env->ReleaseIntArrayElements(_convs,convs,0);
	return r;
}

JNIEXPORT jint JNICALL Java_nlink_win32_Native_getProcAddress2(JNIEnv* env, jclass __unused__, jint hModule, jint ordinal) {
	_ASSERT(sizeof(jint)==sizeof(void*));
	_ASSERT(sizeof(jint)==sizeof(HMODULE));
	return reinterpret_cast<jint>(
		::GetProcAddress(reinterpret_cast<HMODULE>(hModule), reinterpret_cast<LPCSTR>(ordinal)));
}

JNIEXPORT jint JNICALL Java_nlink_win32_Native_getProcAddress(JNIEnv* env, jclass __unused__, jint hModule, jstring _name) {
	_ASSERT(sizeof(jint)==sizeof(void*));
	_ASSERT(sizeof(jint)==sizeof(HMODULE));

	return reinterpret_cast<jint>(
		::GetProcAddress(reinterpret_cast<HMODULE>(hModule),JString(env,_name)));
}
