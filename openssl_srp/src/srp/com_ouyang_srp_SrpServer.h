/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_ouyang_srp_SrpServer */

#ifndef _Included_com_ouyang_srp_SrpServer
#define _Included_com_ouyang_srp_SrpServer
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_ouyang_srp_SrpServer
 * Method:    createInstance
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_ouyang_srp_SrpServer_createInstance
  (JNIEnv *, jobject, jstring, jstring, jstring);

/*
 * Class:     com_ouyang_srp_SrpServer
 * Method:    releaseInstance
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_ouyang_srp_SrpServer_releaseInstance
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_ouyang_srp_SrpServer
 * Method:    generate_s
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_ouyang_srp_SrpServer_generate_1s
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_ouyang_srp_SrpServer
 * Method:    generate_B
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_ouyang_srp_SrpServer_generate_1B
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_ouyang_srp_SrpServer
 * Method:    compute_u
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_ouyang_srp_SrpServer_compute_1u
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     com_ouyang_srp_SrpServer
 * Method:    compute_S
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_ouyang_srp_SrpServer_compute_1S
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_ouyang_srp_SrpServer
 * Method:    compute_K
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_ouyang_srp_SrpServer_compute_1K
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_ouyang_srp_SrpServer
 * Method:    compute_M1
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_ouyang_srp_SrpServer_compute_1M1
        (JNIEnv *, jobject, jstring);


#ifdef __cplusplus
}
#endif
#endif
