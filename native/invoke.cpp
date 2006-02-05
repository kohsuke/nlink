#include "stdafx.h"
#include "nlink.h"


Environment::~Environment() {
	// run post actions
	while( postActions!=NULL ) {
		postActions->act(env);
		PostAction* old = postActions;
		postActions = postActions->next;
		delete old;
	}
}

#ifdef	_DEBUG
static int invocationCount = 0;
#endif

jobject Environment::invoke( void* function, jobjectArray args, jint* convs,
	jclass retType, jint retConv ) {
	// list of clean up actions

	int i;
	DWORD spBefore,spAfter;
	// the unmarshaller if this method should return an object.
	Unmarshaller* retUnm = NULL;

// somehow anonymous union here makes the compiler upset
// --- when we do "push pv" it pushes something different!
//	union {
		BSTR bstr;
		Unmarshaller* unm;
		const wchar_t*	lpcwstr;
		const char*		lpcstr;
		INT8	int8;
		INT16	int16;
		INT32	int32;
		double d;
		float f;
		void*	pv;
//	};

	DWORD retVal;

	const int paramLen = env->GetArrayLength(args);

	__asm mov [spBefore],ESP;

	// push arguments to the stack
	// since we are accumlating things to the stack
	for( i=paramLen-1; i>=0; i-- ) {
		unm = NULL;
		jobject arg = NULL;
		// TODO: check if we can safely use local variables
		
		if( i!=paramLen ) {
			arg = env->GetObjectArrayElement(args,i);
			switch( convs[i] ) {
			case cvBSTR:
				bstr = toBSTR((jstring)arg);
				_asm push bstr;
				break;

			case cvBSTR_byRef:
				if(arg==NULL) {
					pv = NULL;
				} else {
					unm = new BSTRUnmarshaller(toBSTR((jstring)jholder(arg)->get(env)));
					add( new OutParamHandler( jholder(arg), unm ) );
					pv = unm->addr();
				}
				_asm push pv;
				break;

			case cvLPCWSTR:
				lpcwstr = toLPCWSTR((jstring)arg);
				_asm push lpcwstr;
				break;

			case cvLPCSTR:
				lpcstr = toLPCSTR((jstring)arg);
				_asm push lpcstr;
				break;

			case cvINT8:
				_ASSERT( sizeof(INT8)==sizeof(jbyte) );
				int8 = javaLangNumber_byteValue(env,arg);
				_asm push int8;
				break;

			case cvINT8_byRef:
				if(arg==NULL) {
					pv = NULL;
				} else {
					unm = new ByteUnmarshaller(env,jholder(arg)->get(env));
					add(new OutParamHandler( jholder(arg), unm ) );
					pv = unm->addr();
				}
				_asm push pv;
				break;

			case cvINT16:
				_ASSERT( sizeof(INT16)==sizeof(jshort) );
				int16 = javaLangNumber_shortValue(env,arg);
				_asm push int16;
				break;

			case cvINT16_byRef:
				if(arg==NULL) {
					pv = NULL;
				} else {
					unm = new ShortUnmarshaller(env,jholder(arg)->get(env));
					add(new OutParamHandler( jholder(arg), unm ) );
					pv = unm->addr();
				}
				_asm push pv;
				break;

			case cvINT32:
			case cvComObject:
			case cvDISPATCH:
				_ASSERT( sizeof(INT32)==sizeof(jint) );
				int32 = javaLangNumber_intValue(env,arg);
				_asm push int32;
				break;

			case cvPVOID:
				pv = env->GetDirectBufferAddress(arg);
				if(pv==NULL) {
					error(env,__FILE__,__LINE__,"the given Buffer object is not a direct buffer");
					return NULL;
				}
				_asm push pv;
				break;

			case cvINT32_byRef:
				if(arg==NULL) {
					pv = NULL;
				} else {
					unm = new IntUnmarshaller(env,jholder(arg)->get(env));
					add( new OutParamHandler( jholder(arg), unm ) );
					pv = unm->addr();
				}
				_asm push pv;
				break;

			case cvDATE:
			case cvDouble:
				// TODO: check if this is correct
				d = javaLangNumber_doubleValue(env,arg);
				f = reinterpret_cast<float*>(&d)[1];
				_asm push f;
				f = reinterpret_cast<float*>(&d)[0];
				_asm push f;
				break;

			case cvFloat:
				f = javaLangNumber_floatValue(env,arg);
				_asm push f;
				break;

			case cvBool:
				if(javaLangBoolean_booleanValue(env,arg)) {
					int32 = TRUE;
				} else {
					int32 = FALSE;
				}
				_asm push int32;
				break;

			case cvVariantBool:
				if(javaLangBoolean_booleanValue(env,arg)) {
					int32 = VARIANT_TRUE;
				} else {
					int32 = VARIANT_FALSE;
				}
				_asm push int32;
				break;

			case cvVariantBool_byRef:
				if(arg==NULL) {
					pv = NULL;
				} else {
					unm = new VariantBoolUnmarshaller(env,jholder(arg)->get(env));
					add( new OutParamHandler( jholder(arg), unm ) );
					pv = unm->addr();
				}
				_asm push pv;
				break;

			case cvGUID:
				_ASSERT( sizeof(GUID)==sizeof(jlong)*2 );
				pv = env->GetLongArrayElements( (jlongArray)arg, NULL );
				add(new LongArrayCleanUp((jlongArray)arg,pv));
				_asm push pv;
				break;

			default:
				error(env,__FILE__,__LINE__,"unexpected conversion type: %d",convs[i]);
				return NULL;
			}
		}
	}

#ifdef	_DEBUG
	invocationCount++;	// for debugging. this makes it easier to set a break-point.
#endif

	// invoke the method.
	__asm call function;

	__asm mov [spAfter],ESP;

	// once the call returns, stack should have been cleaned up,
	// and the return value should be in EAX.
	__asm mov retVal,EAX;

	// check that the stack size is correct
	if(spBefore!=spAfter) {
		__asm mov ESP, [spBefore];
		error(env,__FILE__,__LINE__,"Unexpected stack corruption. Is the method definition correct?");
		return NULL;
	}


	switch(retConv) {
	case 0:
		return NULL;	// nothing to return

	case cvBSTR:
		retUnm = new BSTRUnmarshaller(NULL);
		break;
	
	case cvHRESULT:
	case cvINT32:
		return javaLangInteger_valueOf(env,(jint)retVal);

	case cvFloat:
		return javaLangFloat_valueOf(env,(jfloat)retVal);

	//case cvDouble:
	//case cvDATE:
	//	retUnm = new DoubleUnmarshaller(env,NULL);
	//	break;

	default:
		error(env,__FILE__,__LINE__,"unexpected conversion type: %d",retConv);
		return NULL;
	}
}

void Environment::add( PostAction* a ) {
	a->next = postActions;
	postActions = a;
}



const jchar* Environment::toChars( jstring s ) {
	if(s==NULL)		return NULL;

	const jchar* r = env->GetStringChars(s,NULL);

	class ReleaseStringCharsCleanUp : public PostAction {
		const jchar*	buf;
		jstring			str;
	public:
		ReleaseStringCharsCleanUp( jstring _str, const jchar* _buf ) {
			str = _str;
			buf = _buf;
		}
		void act( JNIEnv* env ) {
			env->ReleaseStringChars(str,buf);
		}
	};

	add( new ReleaseStringCharsCleanUp(s,r) );
	return r;
}

const wchar_t* Environment::toLPCWSTR( jstring s ) {
	if(s==NULL)		return NULL;

	_ASSERT( sizeof(jchar)==sizeof(wchar_t) );

	const jchar* r = toChars(s);
	jsize sz = env->GetStringLength(s);
	wchar_t* buf = new wchar_t[sz+1];
	memcpy(buf,r,sz*sizeof(wchar_t));
	buf[sz]=L'\0';
	add( new DeleteCleanUp<wchar_t>(buf) );
	return buf;
}

LPCSTR Environment::toLPCSTR( jstring s ) {
	if(s==NULL)		return NULL;

	const jchar* r = toChars(s);
	jsize sz = env->GetStringLength(s);
	char* buf = new char[sz*2+1];
	int len = WideCharToMultiByte( CP_ACP, 0, r, sz, buf, sz*2+1, NULL, NULL );
	buf[len] = '\0';
	add( new DeleteCleanUp<char>(buf) );
	return buf;
}

BSTR Environment::toBSTR( jstring s ) {
	if(s==NULL)		return NULL;

	BSTR bstr = SysAllocStringLen(toChars(s),env->GetStringLength(s));
	add(new BSTRCleanUp(bstr));
	return bstr;
}
