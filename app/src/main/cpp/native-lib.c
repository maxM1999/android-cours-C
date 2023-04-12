#include <jni.h>
#include <stdio.h>

jfloat NativeAdd(JNIEnv* env, jobject thiz, jfloat a, jfloat b)
{
    return a + b;
}

typedef struct
{
    jstring name;
    jint age;
} Person;

void PrintPerson(JNIEnv* env, Person p)
{
    const char* name = (*env)->GetStringUTFChars(env, p.name, NULL);
    char mess[260];
    snprintf(mess, 260, "Name: %s Age: %d", name, p.age);
    (*env)->ReleaseStringChars(env, p.name, name);

    jstring message = (*env)->NewStringUTF(env, mess);
    jclass systemClass = (*env)->FindClass(env, "java/lang/System");
    jfieldID outField = (*env)->GetStaticFieldID(env, systemClass, "out", "Ljava/io/PrintStream;");
    jobject outObj = (*env)->GetStaticObjectField(env, systemClass, outField);
    jclass printStreamClass = (*env)->FindClass(env, "java/io/PrintStream");
    jmethodID printlnMethodID = (*env)->GetMethodID(env, printStreamClass, "println", "(Ljava/lang/String;)V");
    (*env)->CallVoidMethod(env, outObj, printlnMethodID, message);
}

void SendPerson(JNIEnv* env, jobject thiz, jobject p)
{
    jclass personClass = (*env)->GetObjectClass(env, p);
    jfieldID nameField = (*env)->GetFieldID(env, personClass, "name", "Ljava/lang/String;");
    jfieldID ageField = (*env)->GetFieldID(env, personClass, "age", "I");

    Person person;
    person.name = (*env)->GetObjectField(env, p, nameField);
    person.age = (*env)->GetIntField(env, p, ageField);

    PrintPerson(env, person);

}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    if((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        return JNI_ERR;
    }

    jclass c = (*env)->FindClass(env, "com/example/jni_1/MainActivity");
    if(c == NULL) return JNI_ERR;

    static const JNINativeMethod methods[] = {
            {"Add", "(FF)F", NativeAdd},
            {"SendPerson", "(Lcom/example/jni_1/Person;)V", SendPerson }
    };

    int rc = (*env)->RegisterNatives(env, c, methods, sizeof(methods) / sizeof(JNINativeMethod));
    if(rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}

