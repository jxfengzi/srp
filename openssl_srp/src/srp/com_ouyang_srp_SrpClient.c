#include <jni.h>
#include "tiny_log.h"
#include "TinyMap.h"
#include "SrpClient.h"

#define TAG             "srp_c_jni"
#define MODULUS         "3072"
#define STR_NULL        "null"

#ifdef __cplusplus
extern "C" {
#endif

static TinyMap _instances;

static void ItemDeleteListener (void * data, void *ctx)
{
    SrpClient *instance = (SrpClient *)data;
    SrpClient_Delete(instance);
}

JNIEXPORT jint JNICALL _createInstance(JNIEnv *env, jobject obj, jstring instanceId, jstring name, jstring password)
{
    const char * c_name = NULL;
    const char * c_password = NULL;
    const char * c_instanceId = NULL;
    SrpClient *instance = NULL;

    LOG_D(TAG, "_createInstance");

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

    LOG_D(TAG, "instanceId: %s name: %s password: %s", c_instanceId, c_name, c_password);

    instance = SrpClient_New(MODULUS, c_name, c_password);
    if (instance == NULL)
    {
        LOG_E(TAG, "SrpClient_New Failed");
        return -4;
    }

    if (RET_FAILED(TinyMap_Insert(&_instances, c_instanceId, instance)))
    {
        LOG_E(TAG, "TinyMap_Insert Failed");
        return -5;
    }

    LOG_D(TAG, "_createInstance OK, instances count: %d", TinyMap_GetCount(&_instances));

    return 0;
}

JNIEXPORT jint JNICALL _releaseInstance(JNIEnv *env, jobject obj, jstring instanceId)
{
    const char * c_instanceId = NULL;

    LOG_D(TAG, "_releaseInstance");

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

    LOG_D(TAG, "instanceId: %s", c_instanceId);
    LOG_D(TAG, "_releaseInstance OK, instances count: %d", TinyMap_GetCount(&_instances));

    return 0;
}

JNIEXPORT void JNICALL _set_s(JNIEnv *env, jobject obj, jstring instanceId, jstring s)
{
    const char * c_instanceId = NULL;
    const char * c_s = NULL;
    SrpClient *instance = NULL;
    char *hex = NULL;
    size_t len = 0;

    LOG_D(TAG, "%s", "set_s");

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return;
    }

    c_s = (*env)->GetStringUTFChars(env, s, NULL);
    if (c_s == NULL)
    {
        LOG_E(TAG, "s invalid");
        return;
    }

    instance = (SrpClient *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found: %s", c_instanceId);
        return;
    }

    if (RET_FAILED(SrpClient_set_s(instance, c_s)))
    {
        LOG_E(TAG, "set_s failed");
        return;
    }

    return;
}

JNIEXPORT jstring JNICALL _generate_A(JNIEnv *env, jobject obj, jstring instanceId)
{
    const char * c_instanceId = NULL;
    SrpClient *instance = NULL;
    char *hex = NULL;
    size_t len = 0;
    jstring A;

    LOG_D(TAG, "%s", "_generate_A");

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    instance = (SrpClient *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found: %s", c_instanceId);
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpClient_generate_A(instance, &hex, &len)))
    {
        LOG_E(TAG, "generate_A failed");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    A = (*env)->NewStringUTF(env, hex);
    free(hex);

    return A;
}

JNIEXPORT jstring JNICALL _compute_u(JNIEnv *env, jobject obj, jstring instanceId, jstring B)
{
    const char * c_instanceId = NULL;
    const char * c_B = NULL;
    SrpClient *instance = NULL;
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

    c_B = (*env)->GetStringUTFChars(env, B, NULL);
    if (c_B == NULL)
    {
        LOG_E(TAG, "B invalid");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    instance = (SrpClient *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found: %s", c_instanceId);
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpClient_compute_u(instance, c_B, &hex, &len)))
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
    SrpClient *instance = NULL;
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

    instance = (SrpClient *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found: %s", c_instanceId);
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpClient_compute_S(instance, &hex, &len)))
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
    SrpClient *instance = NULL;
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

    instance = (SrpClient *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found: %s", c_instanceId);
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpClient_compute_K(instance, &hex, &len)))
    {
        LOG_E(TAG, "compute_K failed");
        return (*env)->NewStringUTF(env, STR_NULL);
    }
    
    K = (*env)->NewStringUTF(env, hex);
    free(hex);

    return K;
}

JNIEXPORT jstring JNICALL _compute_M1(JNIEnv *env, jobject obj, jstring instanceId)
{
    const char * c_instanceId = NULL;
    SrpClient *instance = NULL;
    char *hex = NULL;
    size_t len = 0;
    jstring M1;

    LOG_D(TAG, "%s", "_compute_M1");

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    instance = (SrpClient *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found: %s", c_instanceId);
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpClient_compute_M1(instance, &hex, &len)))
    {
        LOG_E(TAG, "compute_M1 failed");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    M1 = (*env)->NewStringUTF(env, hex);
    free(hex);

    return M1;
}

JNIEXPORT jstring JNICALL _compute_M2(JNIEnv *env, jobject obj, jstring instanceId)
{
    const char * c_instanceId = NULL;
    SrpClient *instance = NULL;
    char *hex = NULL;
    size_t len = 0;
    jstring M2;

    LOG_D(TAG, "%s", "_compute_M2");

    c_instanceId = (*env)->GetStringUTFChars(env, instanceId, NULL);
    if (c_instanceId == NULL)
    {
        LOG_E(TAG, "instanceId invalid");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    instance = (SrpClient *)TinyMap_GetValue(&_instances, c_instanceId);
    if (instance == NULL)
    {
        LOG_E(TAG, "instance not found: %s", c_instanceId);
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    if (RET_FAILED(SrpClient_compute_M2(instance, &hex, &len)))
    {
        LOG_E(TAG, "compute_M2 failed");
        return (*env)->NewStringUTF(env, STR_NULL);
    }

    M2 = (*env)->NewStringUTF(env, hex);
    free(hex);

    return M2;
}

static const char * _theClass = "com/ouyang/srp/SrpClient";

static JNINativeMethod _theMethods[] =
{
    {"createInstance", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I", &_createInstance},
    {"releaseInstance", "(Ljava/lang/String;)I", &_releaseInstance},
    {"set_s", "(Ljava/lang/String;Ljava/lang/String;)V", &_set_s},
    {"generate_A", "(Ljava/lang/String;)Ljava/lang/String;", &_generate_A},
    {"compute_u", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", &_compute_u},
    {"compute_S", "(Ljava/lang/String;)Ljava/lang/String;", &_compute_S},
    {"compute_K", "(Ljava/lang/String;)Ljava/lang/String;", &_compute_K},
    {"compute_M1", "(Ljava/lang/String;)Ljava/lang/String;", &_compute_M1},
    {"compute_M2", "(Ljava/lang/String;)Ljava/lang/String;", &_compute_M2},
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
