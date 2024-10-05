#include <cstdlib>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include "zygisk.hpp"
#include "log.h"  // 包含日志头文件

extern char **environ;

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

class MyModule : public zygisk::ModuleBase {
private:
    Api *api;
    JNIEnv *env;

public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
        LOGI("Module loaded");  // 输出模块加载日志
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        // auto package_name = env->GetStringUTFChars(args->nice_name, nullptr);
        // auto app_data_dir = env->GetStringUTFChars(args->app_data_dir, nullptr);
        // preSpecialize(package_name, app_data_dir);
        // env->ReleaseStringUTFChars(args->nice_name, package_name);
        // env->ReleaseStringUTFChars(args->app_data_dir, app_data_dir);

        LOGD("preAppSpecialize called");  // 输出函数调用日志
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        // 获取当前进程 ID
        pid_t pid = getpid();

        // 输出日志
        LOGI("Post App Specialization, PID: %d", pid);  // 记录进程ID

        // 遍历环境变量并记录日志
        for (char **envs = environ; *envs != nullptr; ++envs) {
            LOGD("Environment variable: %s", *envs);  // 输出环境变量
        }
    }
};
