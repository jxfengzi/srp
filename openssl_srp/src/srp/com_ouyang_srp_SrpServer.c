#include <jni.h>
#include "tiny_log.h"
#include "TinyMap.h"
#include "SrpServer.h"

#define TAG             "srp_s_jni"
#define MODULUS         "3072"
#define STR_NULL        "null"

#ifdef __cplusplus
extern "C" {
#endif

static TinyMap _instances;

static void ItemDeleteListener (void * data, void *ctx)
{
    SrpServer *instance = (SrpServer *)data;
    SrpServer_Delete(instance);
}

JNIEXPORT jint JNICALL _createInstance(JNIEnv *env, jobject obj, jstring name, jstring password, jstring instanceId)
{
    const char * c_name = NULL;
    const char * c_password = NULL;
    const char * c_instanceId = NULL;
    SrpServer *instance = NULL;

    LOG_D(TAG, "%s", "_createInstance");

    c_name = (*env)->GetStringUTFChars(env, name, NULL);
    if (c_name == NULL)
    {
        LOG_E(TAG, "name invalid");
        return -1;
    }

    c_password = (*env)->GetStringUTFChars(env, password, NULL);
    if (c_password == NULL)
    {
        LOG_E(TAG, "password invalid");
        return -2;
    }

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return -3;
    }

    instance = SrpServer_New(MODULUS, c_name, c_password);
    if (instance == NULL)
    {
        LOG_E(TAG, "SrpServer_New Failed");
        return -4;
    }

    if (RET_FAILED(TinyMap_Insert(&_instances, c_instanceId, instance)))
    {
        LOG_E(TAG, "TinyMap_Insert Failed");
        return -5;
    }

    return 0;
}

JNIEXPORT jint JNICALL _releaseInstance(JNIEnv *env, jobject obj, jstring instanceId)
{
    const char * c_instanceId = NULL;

    LOG_D(TAG, "%s", "_releaseInstance");

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return -1;
    }

    if (RET_FAILED(TinyMap_Erase(&_instances, c_instanceId)))
    {
        LOG_E(TAG, "TinyMap_Erase Failed");
        return -2;
    }

    return 0;
}

JNIEXPORT jstring JNICALL _generate_s(JNIEnv *env, jobject obj, jstring instanceId)
{
    const char * c_instanceId = NULL;
    SrpServer *instance = NULL;
    char *hex = NULL;
    size_t len = 0;
    jstring s;

    LOG_D(TAG, "%s", "_generate_s");

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    instance = (SrpServer *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpServer_generate_s(instance, &hex, &len)))
    {
        LOG_E(TAG, "generate_s failed");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    s = (*env)->NewStringUTF(env, hex);
    free(hex);

    return s;
}

JNIEXPORT jstring JNICALL _generate_B(JNIEnv *env, jobject obj, jstring instanceId)
{
    const char * c_instanceId = NULL;
    SrpServer *instance = NULL;
    char *hex = NULL;
    size_t len = 0;
    jstring B;

    LOG_D(TAG, "%s", "_generate_B");

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    instance = (SrpServer *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpServer_generate_B(instance, &hex, &len)))
    {
        LOG_E(TAG, "generate_B failed");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    B = (*env)->NewStringUTF(env, hex);
    free(hex);

    return B;
}

JNIEXPORT jstring JNICALL _compute_u(JNIEnv *env, jobject obj, jstring instanceId, jstring A)
{
    const char * c_instanceId = NULL;
    const char * c_A = NULL;
    SrpServer *instance = NULL;
    char *hex = NULL;
    size_t len = 0;
    jstring u;

    LOG_D(TAG, "%s", "_compute_u");

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    c_A = (*env)->GetStringUTFChars(env, A, NULL);
    if (c_A == NULL)
    {
        LOG_E(TAG, "A invalid");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    instance = (SrpServer *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpServer_compute_u(instance, c_A, &hex, &len)))
    {
        LOG_E(TAG, "compute_u failed");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    u = (*env)->NewStringUTF(env, hex);
    free(hex);

    return u;
}

JNIEXPORT jstring JNICALL _compute_S(JNIEnv *env, jobject obj, jstring instanceId)
{
    const char * c_instanceId = NULL;
    SrpServer *instance = NULL;
    char *hex = NULL;
    size_t len = 0;
    jstring S;

    LOG_D(TAG, "%s", "_compute_S");

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    instance = (SrpServer *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpServer_compute_S(instance, &hex, &len)))
    {
        LOG_E(TAG, "compute_S failed");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    S = (*env)->NewStringUTF(env, hex);
    free(hex);

    return S;
}

JNIEXPORT jstring JNICALL _compute_K(JNIEnv *env, jobject obj, jstring instanceId)
{
    const char * c_instanceId = NULL;
    SrpServer *instance = NULL;
    char *hex = NULL;
    size_t len = 0;
    jstring K;

    LOG_D(TAG, "%s", "_compute_K");

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    instance = (SrpServer *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpServer_compute_K(instance, &hex, &len)))
    {
        LOG_E(TAG, "compute_S failed");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    K = (*env)->NewStringUTF(env, hex);
    free(hex);

    return K;
}

static const char * _theClass = "com/ouyang/srp/SrpServer";

static JNINativeMethod _theMethods[] =
{
    {"createInstance", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I", &_createInstance},
    {"releaseInstance", "(Ljava/lang/String;)I", &_releaseInstance},
    {"generate_s", "(Ljava/lang/String;)Ljava/lang/String;", &_generate_s},
    {"generate_B", "(Ljava/lang/String;)Ljava/lang/String;", &_generate_B},
    {"compute_u", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", &_compute_u},
    {"compute_S", "(Ljava/lang/String;)Ljava/lang/String;", &_compute_S},
    {"compute_K", "(Ljava/lang/String;)Ljava/lang/String;", &_compute_K},
};

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv * env = NULL;
    jclass clazz;
    int version = -1;

    LOG_I(TAG, "%s", "JNI_OnLoad");

    do
    {
        if ((*vm)->GetEnv(vm, (void *) &env, JNI_VERSION_1_6) == JNI_OK)
        {
            version = JNI_VERSION_1_6;
            LOG_I(TAG, "JNI 1.6");
        }
        else
        {
            if ((*vm)->GetEnv(vm, (void *) &env, JNI_VERSION_1_4) == JNI_OK)
            {
                version = JNI_VERSION_1_4;
                LOG_I(TAG, "JNI 1.4");
            }
            else
            {
                break;
            }
        }

        // Get jclass with env->FindClass.
        clazz = (*env)->FindClass(env, _theClass);
        if (clazz == NULL)
        {
            LOG_E(TAG, "Find class failed: %s", _theClass);
            version = JNI_ERR;
            break;
        }

        LOG_I(TAG, "FindClass OK: %s", _theClass);

        // Register methods with env->RegisterNatives.
        if ((*env)->RegisterNatives(env, clazz, _theMethods, sizeof(_theMethods) / sizeof(JNINativeMethod)) < 0)
        {
            LOG_E(TAG, "RegisterNatives failed.");
            version = JNI_ERR;
            break;
        }

        LOG_D(TAG, "RegisterNatives OK.");

        if (RET_FAILED(TinyMap_Construct(&_instances)))
        {
            LOG_E(TAG, "TinyMap_Construct failed.");
            version = JNI_ERR;
            break;
        }

        TinyMap_SetDeleteListener(&_instances, ItemDeleteListener, NULL);
    }
    while (0);

    return version;
}

void JNI_OnUnload(JavaVM* vm, void* reserved)
{
    LOG_I(TAG, "%s", "JNI_OnUnload");

    TinyMap_Dispose(&_instances);
}

#ifdef __cplusplus
}
#endif
