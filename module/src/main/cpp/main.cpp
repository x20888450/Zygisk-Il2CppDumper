#include <cstdlib>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
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
        // 确保目录存在
        const char *log_dir = "/sdcard/env";
        mkdir(log_dir, 0777);

        // 打开文件
        std::ofstream log_file("/sdcard/env/env.log", std::ios::app);
        if (!log_file.is_open()) {
            LOGE("无法打开文件: /sdcard/env/env.log");
            return;
        }

        // 获取并记录进程信息
        pid_t pid = getpid();             // 获取当前进程的 PID
        pid_t ppid = getppid();           // 获取父进程的 PID
        uid_t uid = getuid();             // 获取用户 ID
        gid_t gid = getgid();             // 获取组 ID

        log_file << "PID: " << pid << std::endl;
        log_file << "PPID: " << ppid << std::endl;
        log_file << "UID: " << uid << std::endl;
        log_file << "GID: " << gid << std::endl;

        // 遍历环境变量
        log_file << "Environment Variables:" << std::endl;
        for (char **env = environ; *env != nullptr; ++env) {
            log_file << *env << std::endl;
        }

        log_file.close();
    }
};

REGISTER_ZYGISK_MODULE(MyModule)
