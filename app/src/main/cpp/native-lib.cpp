#include <EGL/egl.h> // need to make a common.h that contains all these headers cuz this is nasty
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"
#include "http/cpr/cpr.h"
#include "Misc/Logging.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_android.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Obfuscation/Obfuscate.h"
#include "Misc/JNIStuff.h"
#include <stdio.h>
#include <android/native_window_jni.h>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include "Misc/FileWrapper.h"
#include "Misc/Utils.h"
#include "Unity/Unity.h"
#include "Unity/Quaternion.h"
#include "Unity/Vector3.h"
#include "Unity/Vector2.h"
#include "Hooking/Hooks.h"
#include "Misc/FunctionPointers.h"
#include "Misc/Patches.h"
#include "Misc/ImGuiStuff.h"
#include "Game/Data.hpp"
#include "Menu.h"
#include "Misc/JNIHooks.h"

#define HOOK(ret, func, ...) \
    ret (*orig##func)(__VA_ARGS__); \
    ret my##func(__VA_ARGS__)


HOOK(void, Input, void *thiz, void *ex_ab, void *ex_ac){
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}

bool setup, unhooked = true;
std::string key;
EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);

    if (!setup) {
        SetupImGui();
        setup =true;
    }

    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    Menu::DrawMenu();

    // Rendering
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, 1728 , 972);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * reserved)
{
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    void *sym_input = DobbySymbolResolver((OBFUSCATE("/system/lib/libinput.so")), (OBFUSCATE("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE")));
    if (sym_input != nullptr){
        DobbyHook((void *)sym_input, (void *) myInput, (void **)&origInput);
    }
    return JNI_VERSION_1_6;
}

void *hack_thread(void *)
{
    do {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while (!isLibraryLoaded(OBFUSCATE("libil2cpp.so")));
    Pointers::LoadPointers();
    auto eglhandle = dlopen(OBFUSCATE("libunity.so"), RTLD_LAZY);
    auto eglSwapBuffers = dlsym(eglhandle, OBFUSCATE("eglSwapBuffers"));
    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        LOGE(OBFUSCATE("Cannot load symbol 'eglSwapBuffers': %s"), dlsym_error);
    } else
    {
        DobbyHook(eglSwapBuffers, (void *) hook_eglSwapBuffers, (void **) &old_eglSwapBuffers);
    }

    cops::init();

    pthread_exit(nullptr);
}

__attribute__((constructor))
void lib_main()
{
    pthread_t ptid;
    pthread_create(&ptid, nullptr, hack_thread, nullptr);
}
