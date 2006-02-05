#!/bin/sh
mkdir native/javah
javah -jni -classpath runtime/build/classes -o native/javah/nlink_Native.h nlink.win32.Native
