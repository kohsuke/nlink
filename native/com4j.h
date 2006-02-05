#pragma once
#include "java_symbols.h"

// hack 
#define com4j_Holder		nlink_Holder
#define com4j_Holder_value	nlink_Holder_value

// read some files from com4j
// if these include fails, you need to check out
// https://com4j.dev.java.net/ source code and put it side by side with nlink
#include "../../../com4j/com4j/native/cleanup.h"
#include "../../../com4j/com4j/native/unmarshaller.h"