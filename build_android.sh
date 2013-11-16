#!/bin/bash 
NAME=monkeyking
PLATFORM="${PLATFORM:=android}"
NDK_VERSION="${NDK_VERSION:=android-9}"
#NDK_ABI="${NDK_ABI:=all}"
#NDK_ABI="${NDK_ABI:=armeabi-v7a}"
#NDK_ABI="${NDK_ABI:=armeabi}"
NDK_ABI="${NDK_ABI:="armeabi armeabi-v7a"}"
#NDK_ABI="${NDK_ABI:=arm}"
#NDK_ABI:=armeabi armeabi-v7a
NDK_DEBUG="${NDK_DEBUG:=1}"
#TOOLCHAIN_ABIS="armeabi armeabi-v7a"

if [ "$NDK_DEBUG" = "1" ]
then
	TARGET_TYPE="${TARGET_TYPE:=debug}"
else
	TARGET_TYPE="${TARGET_TYPE:=release}"
fi
export TARGET_TYPE

echo "=========================================================="
printf "%-20s%s\n" "PLATFORM"		$PLATFORM
printf "%-20s%s\n" "NDK_VERSION"	$NDK_VERSION
printf "%-20s%s\n" "NDK_ABI"		$NDK_ABI
printf "%-20s%s\n" "NDK_DEBUG"		$NDK_DEBUG
echo "=========================================================="

APP_BUILD_SCRIPT=${PWD}/Android.mk
export APP_BUILD_SCRIPT
echo $APP_BUILD_SCRIPT
APP_PROJECT_PATH=${PWD}/android/
export APP_PROJECT_PATH
echo $APP_PROJECT_PATH

#if ndk-build NDK_PROJECT_PATH=./out/$PLATFORM/$TARGET_TYPE NDK_DEBUG=$NDK_DEBUG APP_PLATFORM=$NDK_VERSION APP_ABI=$NDK_ABI APP_BUILD_SCRIPT=Android.mk $1 $2 $3
if ndk-build NDK_LIBS_OUT=./out/$PLATFORM/$TARGET_TYPE/libs NDK_DEBUG=$NDK_DEBUG NDK_APP_OUT=./out/$PLATFORM/$TARGET_TYPE/out/ NDK_PROJECT_PATH=./android APP_ABI="$NDK_ABI" APP_PLATFORM=$NDK_VERSION APP_BUILD_SCRIPT=Android.mk $1 $2 $3
then # Built ok
	cd android
	if ant $TARGET_TYPE
	then # built successfully
	#	cp ./out/$PLATFORM/$TARGET_TYPE/obj/local/x86/$NAME.a ../../lib/$PLATFORM/x86/$TARGET_TYPE/
	#	cp ./out/$PLATFORM/$TARGET_TYPE/obj/local/armeabi/$NAME.a ../../lib/$PLATFORM/armeabi/$TARGET_TYPE/
	#	cp ./out/$PLATFORM/$TARGET_TYPE/obj/local/armeabi-v7a/$NAME.a ../../lib/$PLATFORM/armeabi-v7a/$TARGET_TYPE/
		cp ../out/$PLATFORM/$TARGET_TYPE/bin/$NAME-$TARGET_TYPE.apk ../$NAME.apk
		echo "DONE!"
		exit 0
	else
		exit 2
	fi
else # Failed to build
	exit 1
fi

#echo "-------------------------------------"
#echo "-------- BUILDING NATIVE LIB --------"
#echo "-------------------------------------"
#if ndk-build NDK_LIBS_OUT=./out/android/libs NDK_APP_OUT=./out/android/out/ NDK_PROJECT_PATH=./android APP_PLATFORM=android-9 APP_BUILD_SCRIPT=Android.mk $1 $2 $3
#then # built successfully
#	cd android
#	echo ""
#	echo "-------------------------------------"
#	echo "----------- BUILDING APK ------------"
#	echo "-------------------------------------"
#	if ant release
#	then # built successfully
#		echo "-------------------------------------"
#		echo "------------- COPY APK --------------"
#		echo "-------------------------------------"
#		cd ..
#		cp out/android/bin/monkeyking-release.apk monkeyking.apk
#		echo "DONE!"
#	else
#		echo "********** Failed to build! **********"
#	fi
#else
#	echo "********** Failed to build! **********"
#fi

