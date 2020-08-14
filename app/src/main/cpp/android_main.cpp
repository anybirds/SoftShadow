#include <android_native_app_glue.h>
#include <jni.h>

#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <glm/glm.hpp>

#include "NDKHelper.h"

#define HELPER_CLASS_NAME "com/example/softshadow/NDKHelper"

#include "Camera.h"
#include "Light.h"
#include "Scene.h"
#include "Script.h"
#include "Time.h"
#include "GUI.h"
#include "Button.h"
#include "Slider.h"

const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
};
const EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
};

EGLint width, height, format;
EGLint numConfigs;
EGLConfig config = nullptr;
EGLSurface surface;
EGLSurface context;
EGLDisplay display;

GLuint vao, vbo;
GLuint program;
GLuint texture;

Scene *scene;

bool init_display(struct android_app *app) {
    // initialize OpenGL ES and EGL
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, nullptr, nullptr);
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    surface = eglCreateWindowSurface(display, config, app->window, nullptr);
    context = eglCreateContext(display, config, nullptr, context_attribs);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return false;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    // check opengl on the system
    auto opengl_info = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
    for (auto name : opengl_info) {
        auto info = glGetString(name);
        LOGI("OpenGL Info: %s", info);
    }

    Time::Init();
    Light::Init();
    GUI::Init();

    Script::Start();

    // create the pre-defined scene
    scene = new Scene();

    return true;
}

void term_display() {
    // destroy the entire scene
    delete scene;

    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }

    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

void draw_frame() {
    if (display == nullptr) {
        return;
    }

    // render from the main camera
    Time::Tick();
    Script::Update();
    Camera::GetMainCamera()->Render();
    eglSwapBuffers(display, surface);
}

int32_t handle_input(android_app *app, AInputEvent *event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int32_t action = AMotionEvent_getAction(event);
        float x = AMotionEvent_getX(event, 0);
        float y = AMotionEvent_getY(event, 0);
        switch (action) {
            case AMOTION_EVENT_ACTION_DOWN:
                Button::CheckIfPressed(glm::vec2(x, y));
            case AMOTION_EVENT_ACTION_MOVE:
                Slider::CheckIfValueChanged(glm::vec2(x, y));
                break;
        }
    }

    return 0;
}

void handle_cmd(android_app *pApp, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            if (!init_display(pApp)) {
                break;
            }
            draw_frame();
            break;
        case APP_CMD_TERM_WINDOW:
            term_display();

    }
}

void android_main(struct android_app *pApp) {
    pApp->onAppCmd = handle_cmd;
    pApp->onInputEvent = handle_input;

    ndk_helper::JNIHelper::Init(pApp->activity, HELPER_CLASS_NAME);
    int events;
    android_poll_source *pSource;
    do {
        if (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
            if (pSource) {
                pSource->process(pApp, pSource);
            }
        }
        draw_frame();
    } while (!pApp->destroyRequested);
}