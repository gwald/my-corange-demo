


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := corange



SDL_PATH := ../SDL2
IMG_PATH := ../SDL2_image-2.0.1
NET_PATH := ../SDL2_net-2.0.1
MIX_PATH := ../SDL2_mixer-2.0.1
TTF_PATH := ../SDL2_ttf-2.0.14



# Required actions for including SDL2 headers
# SDL2 RUN in SDL2 folder" ln -s include SDL2
# in SDL2_mixer mkdir SDL2 copy source(non .c files) into SDL2 folder
# same as above for SDL2_net

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/include \
$(LOCAL_PATH)/include/assets \
$(LOCAL_PATH)/include/data \
$(LOCAL_PATH)/include/entities \
$(LOCAL_PATH)/include/SDL2 \
$(LOCAL_PATH)/include/ui \
$(LOCAL_PATH)/include/rendering \
$(LOCAL_PATH)/$(SDL_PATH) \
$(LOCAL_PATH)/$(IMG_PATH) \
$(LOCAL_PATH)/$(NET_PATH) \
$(LOCAL_PATH)/$(MIX_PATH) \
$(LOCAL_PATH)/$(TTF_PATH) 





LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)




# jni/corange/src/SDL2/SDL_local.c:15:24
# fatal error: execinfo.h: No such file or directory  #include <execinfo.h>
# http://stackoverflow.com/questions/8115192/android-ndk-getting-the-backtrace
# http://stackoverflow.com/questions/11194963/can-i-get-a-c-stack-trace-when-android-app-crashes
# https://github.com/android-ndk/ndk/issues/50
#  -U  __unix__
# vi jni/corange/src/SDL2/SDL_local.c comment out line 15: #include <execinfo.h>


LOCAL_CFLAGS += -std=c99

#LOCAL_SRC_FILES := *.c

# LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,  $(wildcard $(LOCAL_PATH)/src/*.c) )



LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/src/*.c) \
	$(wildcard $(LOCAL_PATH)/src/assets/*.c) \
	$(wildcard $(LOCAL_PATH)/src/data/*.c) \
	$(wildcard $(LOCAL_PATH)/src/entities/*.c) \
	$(wildcard $(LOCAL_PATH)/src/SDL2/*.c) \
	$(wildcard $(LOCAL_PATH)/src/ui/*.c) \
	$(wildcard $(LOCAL_PATH)/src/rendering/*.c) \
)


LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf SDL2_image SDL2_net SDL2_mixer
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2
#LOCAL_LDLIBS := -lGLESv1_CM -llog -lEGL

include $(BUILD_SHARED_LIBRARY)



