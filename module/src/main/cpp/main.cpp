#include <cstdlib>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include "zygisk.hpp"
#include "log.h"

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
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
    LOGI("postAppSpecialize");
        // 遍历环境变量并记录日志
        for (char **envs = environ; *envs != nullptr; ++envs) {
            LOGD("Environment variable: %s", *envs);  // 输出环境变量
        }
        system("cat /proc/self/environ >> /sdcard/env/env.log)
    }
    
// private:
    // Api *api;

    // void preSpecialize(const char *package_name, const char *app_data_dir) {
    // }
};

REGISTER_ZYGISK_MODULE(MyModule)
