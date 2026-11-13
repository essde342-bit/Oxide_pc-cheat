#include "menu.h"
#include "renderer.h"
#include <android/log.h>

#define LOG_TAG "OxideMenu"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static bool g_ready = false;
static bool g_menuVisible = true;

void MenuInit() {
    g_ready = true;
    LOGI("Menu initialized");
}

void MenuRender() {
    if (!g_ready || !g_menuVisible) return;

    // Фон
    DrawRectFilled(50, 50, 360, 320, 0.08f, 0.08f, 0.12f, 0.92f);

    // Заголовок
    DrawRectFilled(50, 50, 360, 55, 0.15f, 0.15f, 0.5f, 1.0f);

    // Разделитель
    DrawRectFilled(50, 105, 360, 2, 1.0f, 1.0f, 0.0f, 0.6f);

    // Кнопка "ESP" (визуально)
    DrawRectFilled(70, 130, 320, 50, 0.2f, 0.5f, 0.2f, 0.85f);

    // Кнопка "Закрыть" (визуально)
    DrawRectFilled(70, 200, 320, 50, 0.5f, 0.2f, 0.2f, 0.85f);

    // Индикатор "работает" (зелёный квадрат)
    DrawRectFilled(70, 270, 20, 20, 0.0f, 1.0f, 0.0f, 1.0f);

    // Рамка
    DrawRectOutline(50, 50, 360, 320, 1.0f, 1.0f, 1.0f, 0.4f, 1.5f);
}
