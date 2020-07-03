#include <memory>
#include <cassert>
#include <cmath>
#include <ctime>
#include <fstream>
#include <sstream>

#include <android_native_app_glue.h>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "NDKHelper.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "assimp/include/assimp/Importer.hpp"
#include "assimp/include/assimp/scene.h"
#include "assimp/include/assimp/postprocess.h"
/*
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
*/
#define HELPER_CLASS_NAME "com/example/softshadow/NDKHelper"

const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
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

    // initialize gl state
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    // read brick image
    std::vector<uint8_t> imgBuf;
    ndk_helper::JNIHelper::GetInstance()->ReadFile("brick.jpg", &imgBuf);
    int width, height, nrChannels;
    uint8_t *imgBits = stbi_load_from_memory(imgBuf.data(), imgBuf.size(), &width, &height, &nrChannels, 0);

    GLenum format;
    switch (nrChannels) {
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RGB;
            break;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, width, height, 0, format, GL_UNSIGNED_BYTE, imgBits);
    stbi_image_free(imgBits);

    // read triangle model file
    std::vector<uint8_t> buf;
    ndk_helper::JNIHelper::GetInstance()->ReadFile("triangle.obj", &buf);

    Assimp::Importer *importer = new Assimp::Importer();
    LOGI("importer created");
    const aiScene *scene = importer->ReadFileFromMemory(buf.data(), buf.size(), aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOGI("scene read fail");
        return false;
    }
    LOGI("scene read");
    if (!scene->HasMeshes()) {
        LOGI("scene has no meshes");
        return false;
    }
    aiMesh *aimesh = scene->mMeshes[0];
    LOGI("mesh read");
    unsigned base;

    unsigned acnt = 3;
    unsigned *attrib = new unsigned[acnt]();
    if (aimesh->HasPositions()) {
        attrib[0] = 3;
    }
    if (aimesh->HasNormals()) {
        attrib[1] = 3;
    }
    if (aimesh->HasTextureCoords(0)) {
        attrib[2] = 2;
    }
    unsigned asize = 0; // number of floats for each vertex
    for (unsigned i = 0; i < acnt; i++) {
        asize += attrib[i];
        LOGI("%d", asize);
    }
    LOGI("asize: %d", asize);

    unsigned vcnt = aimesh->mNumVertices;
    LOGI("vcnt: %d", vcnt);
    float *ver = new float[vcnt * asize];
    base = 0;
    for (unsigned i = 0; i < aimesh->mNumVertices; i++) {
        if (aimesh->HasPositions()) {
            ver[base] = aimesh->mVertices[i].x;
            ver[base + 1] = aimesh->mVertices[i].y;
            ver[base + 2] = aimesh->mVertices[i].z;
            LOGI("%f %f %f", ver[base], ver[base+1], ver[base+2]);
            base += 3;
        }
        if (aimesh->HasNormals()) {
            ver[base] = aimesh->mNormals[i].x;
            ver[base + 1] = aimesh->mNormals[i].y;
            ver[base + 2] = aimesh->mNormals[i].z;
            LOGI("%f %f %f", ver[base], ver[base+1], ver[base+2]);
            base += 3;
        }
        if (aimesh->HasTextureCoords(0)) { // texture coordinate set 0
            ver[base] = aimesh->mTextureCoords[0][i].x;
            ver[base + 1] = aimesh->mTextureCoords[0][i].y;
            LOGI("%f %f", ver[base], ver[base+1]);
            base += 2;
        }
    }

    // create triangle vbo, vao
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 60, ver, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, (void *)12);

    // complie shader
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    const char *sourcev_cstr = "#version 320 es\n"
                               "layout(location = 0) in vec3 pos;\n"
                               "layout(location = 1) in vec2 uv_v;\n"
                               "out vec2 uv_f;\n"
                               "void main() {\n"
                               "    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
                               "    uv_f = uv_v;\n"
                               "}";
    glShaderSource(vert, 1, &sourcev_cstr, NULL);
    glCompileShader(vert);
    GLint status = 0;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* infoLog = (GLchar*)malloc(sizeof(GLchar) * (infoLogLength + 1));
        glGetShaderInfoLog(vert, infoLogLength, NULL, infoLog);
        LOGW("Vertex shader could not compile: %s", infoLog);
        free(infoLog);
        return false;
    }
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    const char *sourcef_cstr = "#version 320 es\n"
                               "precision mediump float;\n"
                               "uniform sampler2D sampler;\n"
                               "in vec2 uv_f;\n"
                               "out vec4 output_color;\n"
                               "void main() {\n"
                               "    output_color = texture(sampler, uv_f);\n"
                               "}";
    glShaderSource(frag, 1, &sourcef_cstr, NULL);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* infoLog = (GLchar*)malloc(sizeof(GLchar) * (infoLogLength + 1));
        glGetShaderInfoLog(frag, infoLogLength, NULL, infoLog);
        LOGW("Fragment shader could not compile: %s", infoLog);
        free(infoLog);
        return false;
    }
    program = glCreateProgram();
    glUseProgram(program);
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    GLint GLerror;
    glGetProgramiv(program, GL_LINK_STATUS, &GLerror);
    if (GLerror == GL_FALSE) {
        LOGW("Program linking failed");
        return false;
    }

    return true;
}

void term_display() {
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
    glClearColor(0.5f + 0.5f * std::sin((float)clock() / CLOCKS_PER_SEC), 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "sampler"), 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    eglSwapBuffers(display, surface);
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