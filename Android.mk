# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PLATFORM:=android

ifeq ($(NDK_DEBUG),1)
	TARGET_TYPE:=debug
else
	TARGET_TYPE:=release
endif

REMEMBER_PATH:= $(call my-dir)
LOCAL_PATH:= $(call my-dir)
DJENGINE_DIR := /cygdrive/c/source.git/djengine
DJENGINE_DIR_2 := `cygpath -wa $(DJENGINE_DIR)`

include $(CLEAR_VARS)
LOCAL_PATH:= $(DJENGINE_DIR)/lib/$(PLATFORM)/$(TARGET_ARCH_ABI)/$(TARGET_TYPE)
LOCAL_MODULE    := libdjengine
LOCAL_SRC_FILES := libdjengine.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH:= $(DJENGINE_DIR)/lib/$(PLATFORM)/$(TARGET_ARCH_ABI)/$(TARGET_TYPE)
LOCAL_MODULE    := libdjos
LOCAL_SRC_FILES := libdjos.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH:= $(DJENGINE_DIR)/lib/$(PLATFORM)/$(TARGET_ARCH_ABI)/$(TARGET_TYPE)
LOCAL_MODULE    := libpng
LOCAL_SRC_FILES := libpng.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH:= $(DJENGINE_DIR)/lib/$(PLATFORM)/$(TARGET_ARCH_ABI)/$(TARGET_TYPE)
LOCAL_MODULE    := libcurl
LOCAL_SRC_FILES := libcurl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH:= $(DJENGINE_DIR)/lib/$(PLATFORM)/$(TARGET_ARCH_ABI)/$(TARGET_TYPE)
LOCAL_MODULE    := libtinyxml
LOCAL_SRC_FILES := libtinyxml.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH:= $(DJENGINE_DIR)/lib/$(PLATFORM)/$(TARGET_ARCH_ABI)/$(TARGET_TYPE)
LOCAL_MODULE    := libfreetype2
LOCAL_SRC_FILES := libfreetype2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH:= $(DJENGINE_DIR)/lib/$(PLATFORM)/$(TARGET_ARCH_ABI)/$(TARGET_TYPE)
LOCAL_MODULE    := libopenal
LOCAL_SRC_FILES := libopenal.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH:= $(DJENGINE_DIR)/lib/$(PLATFORM)/$(TARGET_ARCH_ABI)/$(TARGET_TYPE)
LOCAL_MODULE    := libpvmp3
LOCAL_SRC_FILES := libpvmp3.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH:= $(DJENGINE_DIR)/lib/$(PLATFORM)/$(TARGET_ARCH_ABI)/$(TARGET_TYPE)
LOCAL_MODULE    := libdjgles2render
LOCAL_SRC_FILES := libdjgles2render.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH:= $(DJENGINE_DIR)/lib/$(PLATFORM)/$(TARGET_ARCH_ABI)/$(TARGET_TYPE)
LOCAL_MODULE    := libdjopenalsound
LOCAL_SRC_FILES := libdjopenalsound.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH:= $(REMEMBER_PATH)

LOCAL_C_INCLUDES += \
	$(DJENGINE_DIR)/include \
	$(DJENGINE_DIR)/externals/chipmunk-physics/include/chipmunk \
	$(DJENGINE_DIR)/externals/tinyxml \
	$(DJENGINE_DIR)/externals/libpng \
	$(DJENGINE_DIR)/externals/libcurl_android/include \
	$(DJENGINE_DIR)/include/osal

LOCAL_MODULE    := libdj

ifeq ($(NDK_DEBUG),1)
	LOCAL_CFLAGS := \
		-O0 \
		-g \
		-D__STDC_CONSTANT_MACROS \
		-DCONFIG_AS_LIBRARY \
		-DNFINALVER \
		-D_DEBUG \
		-D_DJANDROID \
		-D__ARM__
else
	LOCAL_CFLAGS := \
		-O3 \
		-D__STDC_CONSTANT_MACROS \
		-DCONFIG_AS_LIBRARY \
		-D_FINALVER \
		-DNDEBUG \
		-D_DJANDROID \
		-D__ARM__
endif

LOCAL_SHARED_LIBRARIES := \
	libpng

LOCAL_STATIC_LIBRARIES := libdjengine \
						libdjos \
						libpng \
						libcurl \
						libtinyxml \
						libfreetype2 \
						libdjgles2render \
						libdjopenalsound	\
						libopenal			\
						libpvmp3			\
						cpufeatures
	
LOCAL_SRC_FILES := sourcecode/monkeyking.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := libdjos
LOCAL_ARM_MODE := arm
LOCAL_LDLIBS    := -L$(SYSROOT)/usr/lib -llog -lz -lGLESv2 -lOpenSLES
# -ldjengine -ldjos -lgles2render


LOCAL_LDFLAGS := -Wl,--no-undefined -Wl,--error-unresolved-symbols -Wl,--version-script=$(DJENGINE_DIR_2)/djos/djos.version

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/cpufeatures) 
