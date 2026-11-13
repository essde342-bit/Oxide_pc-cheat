#include "hooks.h"
#include "renderer.h"
#include "esp.h"
#include "menu.h"

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <dlfcn.h>
#include <android/log.h>
#include "MinHook.h"

#define LOG_TAG "OxideHooks"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

typedef EGLBoolean (*eglSwapBuffers_t)(EGLDisplay, EGLSurface);
static eglSwapBuffers_t g_origSwapBuffers = nullptr;
static bool g_initialized = false;

static void EnsureInit() {
    if (g_initialized) return;

    // Получаем размер экрана из текущего viewport
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    SetScreenSize(viewport[2], viewport[3]);

    RendererInit();
    MenuInit();
    ESPInit();

    g_initialized = true;
    LOGI("Cheat initialized (screen %dx%d)", viewport[2], viewport[3]);
}

static EGLBoolean HookedSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    EnsureInit();

    // ========== СОХРАНЯЕМ СОСТОЯНИЕ ==========
    GLint prevProgram = 0;
    GLint prevVBO = 0;
    GLint prevViewport[4];
    GLint prevScissor[4];
    GLint prevBlendSrc = GL_ONE;
    GLint prevBlendDst = GL_ZERO;
    GLboolean prevBlend = GL_FALSE;
    GLboolean prevDepth = GL_FALSE;
    GLboolean prevCull = GL_FALSE;
    GLboolean prevScissorTest = GL_FALSE;

    glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prevVBO);
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    glGetIntegerv(GL_SCISSOR_BOX, prevScissor);
    glGetIntegerv(GL_BLEND_SRC, &prevBlendSrc);
    glGetIntegerv(GL_BLEND_DST, &prevBlendDst);
    prevBlend = glIsEnabled(GL_BLEND);
    prevDepth = glIsEnabled(GL_DEPTH_TEST);
    prevCull = glIsEnabled(GL_CULL_FACE);
    prevScissorTest = glIsEnabled(GL_SCISSOR_TEST);

    // ========== НАСТРАИВАЕМ ДЛЯ РИСОВАНИЯ ==========
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ========== РИСУЕМ ==========
    ESPRender();
    MenuRender();

    // ========== ВОССТАНАВЛИВАЕМ СОСТОЯНИЕ ==========
    if (prevDepth)       glEnable(GL_DEPTH_TEST);   else glDisable(GL_DEPTH_TEST);
    if (prevBlend)       glEnable(GL_BLEND);        else glDisable(GL_BLEND);
    if (prevCull)        glEnable(GL_CULL_FACE);    else glDisable(GL_CULL_FACE);
    if (prevScissorTest) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);

    glBlendFunc(prevBlendSrc, prevBlendDst);
    glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
    glScissor(prevScissor[0], prevScissor[1], prevScissor[2], prevScissor[3]);
    glBindBuffer(GL_ARRAY_BUFFER, prevVBO);
    glUseProgram(prevProgram);

    return g_origSwapBuffers(dpy, surface);
}

void InstallHooks() {
    if (MH_Initialize() != MH_OK) {
        LOGE("MH_Initialize failed");
        return;
    }

    void* libEGL = dlopen("libEGL.so", RTLD_NOW);
    if (!libEGL) {
        LOGE("dlopen libEGL failed: %s", dlerror());
        return;
    }

    void* target = dlsym(libEGL, "eglSwapBuffers");
    if (!target) {
        LOGE("dlsym eglSwapBuffers failed");
        return;
    }

    MH_STATUS status = MH_CreateHook(target, (void*)&HookedSwapBuffers,
                                     (void**)&g_origSwapBuffers);
    if (status != MH_OK) {
        LOGE("MH_CreateHook failed: %d", status);
        return;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK) {
        LOGE("MH_EnableHook failed: %d", status);
        return;
    }

    LOGI("eglSwapBuffers hooked OK");
}
