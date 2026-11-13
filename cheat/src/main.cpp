#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <android/log.h>
#include "hooks.h"

#define LOG_TAG "OxideCheat"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static void* CheatThread(void*) {
    // Задержка перед хуком
    // Можно переопределить через переменную окружения CHEAT_DELAY
    int delay = 5;
    const char* envDelay = getenv("CHEAT_DELAY");
    if (envDelay) {
        int parsed = atoi(envDelay);
        if (parsed > 0 && parsed < 60) delay = parsed;
    }

    LOGI("Waiting %d seconds before hooking...", delay);
    sleep(delay);

    InstallHooks();
    return nullptr;
}

__attribute__((constructor))
static void OnLibraryLoaded() {
    LOGI("OxideCheat library loaded");
    pthread_t tid;
    pthread_create(&tid, nullptr, CheatThread, nullptr);
    pthread_detach(tid);
}
