/**
 * The macro for log.
 */

#include <jni.h>
#include <android/log.h>

#define TAG		"Native"

#define LOG_I(...)	__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOG_D(...)	__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOG_W(...)	__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define	LOG_E(...)	__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#define LOGI(tag, ...)	__android_log_print(ANDROID_LOG_INFO, tag, __VA_ARGS__)
#define LOGD(tag, ...)	__android_log_print(ANDROID_LOG_DEBUG, tag, __VA_ARGS__)
#define LOGW(tag, ...)	__android_log_print(ANDROID_LOG_WARN, tag, __VA_ARGS__)
#define	LOGE(tag, ...)	__android_log_print(ANDROID_LOG_ERROR, tag, __VA_ARGS__)
