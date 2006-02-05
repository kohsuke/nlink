#include "stdafx.h"
#include "java_symbols.h"

JClassID nlinkexception("nlink/NLinkException");
JConstructorID nlinkexception_new(nlinkexception,"(Ljava/lang/String;Ljava/lang/String;I)V");

JClassID nlink_Holder("nlink/Holder");
JFieldID<jobject> nlink_Holder_value(nlink_Holder,"value","Ljava/lang/Object;");
