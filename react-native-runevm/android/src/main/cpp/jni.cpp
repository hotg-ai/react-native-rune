//
//  Wasm3 - high performance WebAssembly interpreter written in C.
//
//  Copyright © 2019 Steven Massey, Volodymyr Shymanskyy.
//  All rights reserved.
//

#include <memory>
#include <jni.h>
#include <optional>
#include <android/log.h>
#include <rune.hpp>
#include <runetime.cpp>
#include <vector>
namespace
{
    struct Deleter
    {
        Deleter(JNIEnv *env, jbyteArray jdata)
            : m_env(env), m_jdata(jdata) {}

        void operator()(jbyte *ptr) const
        {
            m_env->ReleaseByteArrayElements(m_jdata, ptr, JNI_ABORT);
        }

    private:
        JNIEnv *m_env;
        jbyteArray m_jdata;
    };

    struct JByteArrayData
    {
        JByteArrayData(std::unique_ptr<jbyte, Deleter> &&data, uint32_t size)
            : m_data(std::move(data)), m_size(size) {}

        uint8_t *data() const noexcept
        {
            return reinterpret_cast<uint8_t *>(m_data.get());
        }

        uint32_t size() const noexcept
        {
            return m_size;
        }

    private:
        std::unique_ptr<jbyte, Deleter> m_data;
        uint32_t m_size;
    };

    std::optional<JByteArrayData> getDataFromJArray(JNIEnv *env, jbyteArray jdata)
    {
        auto data = std::unique_ptr<jbyte, Deleter>(
            env->GetByteArrayElements(jdata, 0),
            Deleter(env, jdata));
        if (!data)
            return std::nullopt;

        const auto size = env->GetArrayLength(jdata);
        if (size < 0)
            return std::nullopt;

        return JByteArrayData(std::move(data), size);
    }

   /* struct AndroidLogger : public rune_vm::ILogger
    {
    private:
        void log(
            const rune_vm::Severity severity,
            const std::string &module,
            const std::string &message) const noexcept
        {
            const auto androidSeverity = [severity]
            {
                switch (severity)
                {
                case rune_vm::Severity::Debug:
                    return ANDROID_LOG_DEBUG;
                case rune_vm::Severity::Info:
                    return ANDROID_LOG_INFO;
                case rune_vm::Severity::Warning:
                    return ANDROID_LOG_WARN;
                case rune_vm::Severity::Error:
                    return ANDROID_LOG_ERROR;
                default:
                    return ANDROID_LOG_ERROR;
                }
            }();

            __android_log_print(androidSeverity, module.c_str(), "%s", message.c_str());
        }
    };*/
}

extern "C"
{
    JNIEXPORT jint JNICALL
    JNI_OnLoad(JavaVM *vm, void *reserved)
    {
        auto jniEnv = (JNIEnv *)nullptr;

        if (vm->GetEnv((void **)&jniEnv, JNI_VERSION_1_6) != JNI_OK)
        {
            return JNI_ERR; // JNI version not supported
        }

        // set logger
       // runic_common::setLogger(std::make_shared<AndroidLogger>());

        return JNI_VERSION_1_6;
    }

    Runetime runetime;

    JNIEXPORT jstring JNICALL
    Java_com_reactlibrary_RunevmModule_getManifest(JNIEnv *env, jobject thiz, jbyteArray wasm)
    {


        const auto optData = getDataFromJArray(env, wasm);
        if (!optData)
            return NULL;

        struct rune::Config cfg = {
            .rune = optData->data(),
            .rune_len = (int )optData->size(),
        };
        
        std::string result = runetime.load(cfg);
        __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "########## length %s",result.c_str());
        return env->NewStringUTF(result.c_str());

    }

    JNIEXPORT jstring JNICALL
    Java_com_reactlibrary_RunevmModule_runRune(JNIEnv *env, jobject thiz , jbyteArray input, jintArray lengthsj)
    {
        const auto optData = getDataFromJArray(env, input);
        if (!optData)
            return NULL;
        jsize size = env->GetArrayLength( lengthsj );
        jint *lengths = env->GetIntArrayElements(lengthsj, 0);



        int i;
        int pos =0;
        for (i = 0; i < size; ++i)
        {
            __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "########## length %lu %i",*lengths+i,i);
            runetime.addInputTensor(1,reinterpret_cast<uint8_t*>(const_cast<uint8_t*>(optData->data()+pos)),*(lengths+i));
            pos = pos + *(lengths+i);
        }
        std::string result = runetime.run();
        return env->NewStringUTF(result.c_str());
    }
}