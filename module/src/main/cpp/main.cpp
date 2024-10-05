#include <cstdlib>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include "zygisk.hpp"

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
        // auto package_name = env->GetStringUTFChars(args->nice_name, nullptr);
        // auto app_data_dir = env->GetStringUTFChars(args->app_data_dir, nullptr);
        // preSpecialize(package_name, app_data_dir);
        // env->ReleaseStringUTFChars(args->nice_name, package_name);
        // env->ReleaseStringUTFChars(args->app_data_dir, app_data_dir);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        // 获取当前进程 ID
        pid_t pid = getpid();

        // 构建文件路径
        std::ostringstream filepath;
        filepath << "/sdcard/env/" << pid << "_env.log";

        // 创建目录（如果不存在）
        mkdir("/sdcard/env", 0755);

        // 打开文件
        std::ofstream log_file(filepath.str(), std::ios::out);

        if (log_file.is_open()) {
            // 遍历环境变量
            for (char **envs = environ; *envs != nullptr; ++envs) {
                log_file << *envs << std::endl;
            }
            log_file.close();
        }
    }
};
