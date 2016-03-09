# This is the application makefile

# APP_OPTIM two options: debug, release
APP_OPTIM := release

# APP_ABI four options: armeabi, armeabi-v7a, mips, x86, all 
APP_ABI := armeabi armeabi-v7a x86

# APP_PLATFORM: used to specify the include folder
APP_PLATFORM := android-19

# APP_PROJECT_PATH: absolute path of project root path
#APP_PROJECT_PATH := $(call my-dir)/..
# APP_CFLAGS, used to insted of module in Android.mk \
# example: APP_CFLAGS += -I sources/bar
# APP_CPPFLAGS
# APP_STL three options :stlport_static, stlport_shared, system(default) for std::...
#APP_STL := stlport_static