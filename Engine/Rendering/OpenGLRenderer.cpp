#include "Rendering/OpenGLRenderer.hpp"
#include "Core/Logger.hpp"
#include "Platform/Window.hpp"
#include <cmath>
#include <string>

#if defined(_WIN32)
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif

// OpenGL constants
#ifndef GL_COLOR_BUFFER_BIT
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_FAN 0x0006
#define GL_LINE_LOOP 0x0002
#define GL_LINES 0x0001
#define GL_FLOAT 0x0140
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_ARRAY_BUFFER 0x8892
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_STATIC_DRAW 0x88E4
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_BLEND 0x0BE2
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#endif

// GL function pointer typedefs
typedef void (APIENTRY *PFNGLCLEARCOLORPROC)(float r, float g, float b, float a);
typedef void (APIENTRY *PFNGLCLEARPROC)(uint32_t mask);
typedef void (APIENTRY *PFNGLVIEWPORTPROC)(int x, int y, int width, int height);
typedef void (APIENTRY *PFNGLENABLEPROC)(uint32_t cap);
typedef void (APIENTRY *PFNGLDISABLEPROC)(uint32_t cap);
typedef void (APIENTRY *PFNGLBLENDFUNCPROC)(uint32_t sfactor, uint32_t dfactor);

typedef uint32_t (APIENTRY *PFNGLCREATESHADERPROC)(uint32_t type);
typedef void (APIENTRY *PFNGLSHADERSOURCEPROC)(uint32_t shader, int count, const char* const* string, const int* length);
typedef void (APIENTRY *PFNGLCOMPILESHADERPROC)(uint32_t shader);
typedef void (APIENTRY *PFNGLGETSHADERIVPROC)(uint32_t shader, uint32_t pname, int* params);
typedef void (APIENTRY *PFNGLGETSHADERINFOLOGPROC)(uint32_t shader, int bufSize, int* length, char* infoLog);

typedef uint32_t (APIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef void (APIENTRY *PFNGLATTACHSHADERPROC)(uint32_t program, uint32_t shader);
typedef void (APIENTRY *PFNGLLINKPROGRAMPROC)(uint32_t program);
typedef void (APIENTRY *PFNGLGETPROGRAMIVPROC)(uint32_t program, uint32_t pname, int* params);
typedef void (APIENTRY *PFNGLGETPROGRAMINFOLOGPROC)(uint32_t program, int bufSize, int* length, char* infoLog);
typedef void (APIENTRY *PFNGLUSEPROGRAMPROC)(uint32_t program);
typedef void (APIENTRY *PFNGLDELETESHADERPROC)(uint32_t shader);
typedef void (APIENTRY *PFNGLDELETEPROGRAMPROC)(uint32_t program);

typedef int (APIENTRY *PFNGLGETUNIFORMLOCATIONPROC)(uint32_t program, const char* name);
typedef void (APIENTRY *PFNGLUNIFORMMATRIX4FVPROC)(int location, int count, unsigned char transpose, const float* value);

typedef void (APIENTRY *PFNGLGENVERTEXARRAYSPROC)(int n, uint32_t* arrays);
typedef void (APIENTRY *PFNGLBINDVERTEXARRAYPROC)(uint32_t array);
typedef void (APIENTRY *PFNGLDELETEVERTEXARRAYSPROC)(int n, const uint32_t* arrays);

typedef void (APIENTRY *PFNGLGENBUFFERSPROC)(int n, uint32_t* buffers);
typedef void (APIENTRY *PFNGLBINDBUFFERPROC)(uint32_t target, uint32_t buffer);
typedef void (APIENTRY *PFNGLBUFFERDATAPROC)(uint32_t target, ptrdiff_t size, const void* data, uint32_t usage);
typedef void (APIENTRY *PFNGLBUFFERSUBDATAPROC)(uint32_t target, ptrdiff_t offset, ptrdiff_t size, const void* data);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC)(int n, const uint32_t* buffers);

typedef void (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(uint32_t index);
typedef void (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(uint32_t index, int size, uint32_t type, unsigned char normalized, int stride, const void* pointer);
typedef void (APIENTRY *PFNGLVERTEXATTRIBDIVISORPROC)(uint32_t index, uint32_t divisor);

typedef void (APIENTRY *PFNGLDRAWARRAYSINSTANCEDPROC)(uint32_t mode, int first, int count, int instancecount);
typedef void (APIENTRY *PFNGLDRAWARRAYSPROC)(uint32_t mode, int first, int count);

// Static GL function pointers
static PFNGLCLEARCOLORPROC glClearColor = nullptr;
static PFNGLCLEARPROC glClear = nullptr;
static PFNGLVIEWPORTPROC glViewport = nullptr;
static PFNGLENABLEPROC glEnable = nullptr;
static PFNGLDISABLEPROC glDisable = nullptr;
static PFNGLBLENDFUNCPROC glBlendFunc = nullptr;

static PFNGLCREATESHADERPROC glCreateShader = nullptr;
static PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
static PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
static PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;

static PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
static PFNGLATTACHSHADERPROC glAttachShader = nullptr;
static PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
static PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
static PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
static PFNGLDELETESHADERPROC glDeleteShader = nullptr;
static PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;

static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;

static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;

static PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
static PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
static PFNGLBUFFERDATAPROC glBufferData = nullptr;
static PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;

static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
static PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor = nullptr;

static PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = nullptr;
static PFNGLDRAWARRAYSPROC glDrawArrays = nullptr;

template <typename T>
static T LoadGLProc(const char* name) {
    return reinterpret_cast<T>(SDL_GL_GetProcAddress(name));
}

static bool LoadGLFunctions() {
    glClearColor = LoadGLProc<PFNGLCLEARCOLORPROC>("glClearColor");
    glClear = LoadGLProc<PFNGLCLEARPROC>("glClear");
    glViewport = LoadGLProc<PFNGLVIEWPORTPROC>("glViewport");
    glEnable = LoadGLProc<PFNGLENABLEPROC>("glEnable");
    glDisable = LoadGLProc<PFNGLDISABLEPROC>("glDisable");
    glBlendFunc = LoadGLProc<PFNGLBLENDFUNCPROC>("glBlendFunc");

    glCreateShader = LoadGLProc<PFNGLCREATESHADERPROC>("glCreateShader");
    glShaderSource = LoadGLProc<PFNGLSHADERSOURCEPROC>("glShaderSource");
    glCompileShader = LoadGLProc<PFNGLCOMPILESHADERPROC>("glCompileShader");
    glGetShaderiv = LoadGLProc<PFNGLGETSHADERIVPROC>("glGetShaderiv");
    glGetShaderInfoLog = LoadGLProc<PFNGLGETSHADERINFOLOGPROC>("glGetShaderInfoLog");

    glCreateProgram = LoadGLProc<PFNGLCREATEPROGRAMPROC>("glCreateProgram");
    glAttachShader = LoadGLProc<PFNGLATTACHSHADERPROC>("glAttachShader");
    glLinkProgram = LoadGLProc<PFNGLLINKPROGRAMPROC>("glLinkProgram");
    glGetProgramiv = LoadGLProc<PFNGLGETPROGRAMIVPROC>("glGetProgramiv");
    glGetProgramInfoLog = LoadGLProc<PFNGLGETPROGRAMINFOLOGPROC>("glGetProgramInfoLog");
    glUseProgram = LoadGLProc<PFNGLUSEPROGRAMPROC>("glUseProgram");
    glDeleteShader = LoadGLProc<PFNGLDELETESHADERPROC>("glDeleteShader");
    glDeleteProgram = LoadGLProc<PFNGLDELETEPROGRAMPROC>("glDeleteProgram");

    glGetUniformLocation = LoadGLProc<PFNGLGETUNIFORMLOCATIONPROC>("glGetUniformLocation");
    glUniformMatrix4fv = LoadGLProc<PFNGLUNIFORMMATRIX4FVPROC>("glUniformMatrix4fv");

    glGenVertexArrays = LoadGLProc<PFNGLGENVERTEXARRAYSPROC>("glGenVertexArrays");
    glBindVertexArray = LoadGLProc<PFNGLBINDVERTEXARRAYPROC>("glBindVertexArray");
    glDeleteVertexArrays = LoadGLProc<PFNGLDELETEVERTEXARRAYSPROC>("glDeleteVertexArrays");

    glGenBuffers = LoadGLProc<PFNGLGENBUFFERSPROC>("glGenBuffers");
    glBindBuffer = LoadGLProc<PFNGLBINDBUFFERPROC>("glBindBuffer");
    glBufferData = LoadGLProc<PFNGLBUFFERDATAPROC>("glBufferData");
    glBufferSubData = LoadGLProc<PFNGLBUFFERSUBDATAPROC>("glBufferSubData");
    glDeleteBuffers = LoadGLProc<PFNGLDELETEBUFFERSPROC>("glDeleteBuffers");

    glEnableVertexAttribArray = LoadGLProc<PFNGLENABLEVERTEXATTRIBARRAYPROC>("glEnableVertexAttribArray");
    glVertexAttribPointer = LoadGLProc<PFNGLVERTEXATTRIBPOINTERPROC>("glVertexAttribPointer");
    glVertexAttribDivisor = LoadGLProc<PFNGLVERTEXATTRIBDIVISORPROC>("glVertexAttribDivisor");

    glDrawArraysInstanced = LoadGLProc<PFNGLDRAWARRAYSINSTANCEDPROC>("glDrawArraysInstanced");
    glDrawArrays = LoadGLProc<PFNGLDRAWARRAYSPROC>("glDrawArrays");

    return glClearColor && glCreateShader && glGenVertexArrays && glDrawArraysInstanced;
}

namespace Shape {

OpenGLRenderer::~OpenGLRenderer() {
    Shutdown();
}

bool OpenGLRenderer::Initialize(Window& window) {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SHAPE_LOG_ERROR("OpenGLRenderer: Failed to initialize SDL VIDEO: {}", SDL_GetError());
        return false;
    }

    m_sdlWindow = reinterpret_cast<SDL_Window*>(window.GetNativeWindow());
    if (!m_sdlWindow) {
        SHAPE_LOG_ERROR("OpenGLRenderer: Invalid native window handle.");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_glContext = SDL_GL_CreateContext(m_sdlWindow);
    if (!m_glContext) {
        // Fallback to 3.3 core if 4.6 context creation fails on driver
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        m_glContext = SDL_GL_CreateContext(m_sdlWindow);
    }

    if (!m_glContext) {
        SHAPE_LOG_ERROR("OpenGLRenderer: Failed to create OpenGL Context: {}", SDL_GetError());
        return false;
    }

    SDL_GL_MakeCurrent(m_sdlWindow, m_glContext);
    SDL_GL_SetSwapInterval(1); // VSync

    if (!LoadGLFunctions()) {
        SHAPE_LOG_ERROR("OpenGLRenderer: Failed to load OpenGL procedure addresses.");
        return false;
    }

    glViewport(0, 0, window.GetWidth(), window.GetHeight());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!InitShaders() || !InitBuffers()) {
        return false;
    }

    // Prepare unit circle vertices (32 segments)
    const int segments = 32;
    m_unitCircleVertices.reserve(segments + 2);
    m_unitCircleVertices.push_back({0.0f, 0.0f});
    for (int i = 0; i <= segments; ++i) {
        f32 angle = (i * 2.0f * 3.14159265f) / segments;
        m_unitCircleVertices.push_back({std::cos(angle), std::sin(angle)});
    }

    SHAPE_LOG_INFO("OpenGLRenderer: Initialized successfully with instanced rendering pipeline.");
    return true;
}

void OpenGLRenderer::Shutdown() {
    if (m_vao && glDeleteVertexArrays) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_vboBase && glDeleteBuffers) {
        uint32_t bufs[2] = {m_vboBase, m_vboInstance};
        glDeleteBuffers(2, bufs);
        m_vboBase = 0;
        m_vboInstance = 0;
    }
    if (m_shaderProgram && glDeleteProgram) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
    if (m_glContext) {
        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;
    }
    m_sdlWindow = nullptr;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool OpenGLRenderer::InitShaders() {
    const char* vertSource = R"(#version 330 core
layout(location = 0) in vec2 a_BasePos;

// Instanced Attributes
layout(location = 1) in vec2 i_Pos;
layout(location = 2) in float i_Rot;
layout(location = 3) in vec2 i_Scale;
layout(location = 4) in vec4 i_Color;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec4 v_Color;

void main() {
    float c = cos(i_Rot);
    float s = sin(i_Rot);
    mat2 rotMat = mat2(c, s, -s, c);

    vec2 localScaled = a_BasePos * i_Scale;
    vec2 worldPos = i_Pos + rotMat * localScaled;

    gl_Position = u_Projection * u_View * vec4(worldPos, 0.0, 1.0);
    v_Color = i_Color;
}
)";

    const char* fragSource = R"(#version 330 core
in vec4 v_Color;
out vec4 FragColor;

void main() {
    FragColor = v_Color;
}
)";

    uint32_t vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSource, nullptr);
    glCompileShader(vertShader);

    int status = 0;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char log[512];
        glGetShaderInfoLog(vertShader, 512, nullptr, log);
        SHAPE_LOG_ERROR("OpenGLRenderer: Vertex Shader compile error: {}", log);
        return false;
    }

    uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, nullptr);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char log[512];
        glGetShaderInfoLog(fragShader, 512, nullptr, log);
        SHAPE_LOG_ERROR("OpenGLRenderer: Fragment Shader compile error: {}", log);
        return false;
    }

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertShader);
    glAttachShader(m_shaderProgram, fragShader);
    glLinkProgram(m_shaderProgram);

    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
    if (!status) {
        char log[512];
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, log);
        SHAPE_LOG_ERROR("OpenGLRenderer: Shader Program link error: {}", log);
        return false;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    m_projLoc = glGetUniformLocation(m_shaderProgram, "u_Projection");
    m_viewLoc = glGetUniformLocation(m_shaderProgram, "u_View");

    return true;
}

bool OpenGLRenderer::InitBuffers() {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboBase);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboBase);
    // Allocate space for up to 256 base vertices
    glBufferData(GL_ARRAY_BUFFER, 256 * sizeof(Math::Vector2f), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Math::Vector2f), (void*)0);

    glGenBuffers(1, &m_vboInstance);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboInstance);
    // Allocate space for up to 100,000 instances
    glBufferData(GL_ARRAY_BUFFER, 100000 * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);

    // Instance Attribute 1: position (vec2)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, position));
    glVertexAttribDivisor(1, 1);

    // Instance Attribute 2: rotation (float)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, rotation));
    glVertexAttribDivisor(2, 1);

    // Instance Attribute 3: scale (vec2)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, scale));
    glVertexAttribDivisor(3, 1);

    // Instance Attribute 4: color (vec4)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, color));
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);
    return true;
}

void OpenGLRenderer::BeginFrame() {
    // Frame setup
}

void OpenGLRenderer::EndFrame() {
    SDL_GL_SwapWindow(m_sdlWindow);
}

void OpenGLRenderer::SetClearColor(const RenderColor& color) {
    m_clearColor = color;
}

void OpenGLRenderer::Clear() {
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::SetCameraMatrices(const Math::Matrix4& projection, const Math::Matrix4& view) {
    m_projectionMatrix = projection;
    m_viewMatrix = view;
}

void OpenGLRenderer::DrawInstancedPolygons(const Math::Vector2f* baseVertices, usize vertexCount, const InstanceData* instances, usize instanceCount, bool filled) {
    if (!baseVertices || vertexCount == 0 || !instances || instanceCount == 0) return;

    glUseProgram(m_shaderProgram);
    glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, &m_projectionMatrix[0][0]);
    glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);

    glBindVertexArray(m_vao);

    // Upload base mesh geometry
    glBindBuffer(GL_ARRAY_BUFFER, m_vboBase);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(Math::Vector2f), baseVertices);

    // Upload instance transform & color stream
    glBindBuffer(GL_ARRAY_BUFFER, m_vboInstance);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instanceCount * sizeof(InstanceData), instances);

    uint32_t mode = filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP;
    glDrawArraysInstanced(mode, 0, static_cast<int>(vertexCount), static_cast<int>(instanceCount));

    glBindVertexArray(0);
}

void OpenGLRenderer::DrawInstancedCircles(f32 radius, const InstanceData* instances, usize instanceCount, bool filled) {
    (void)radius;
    if (!instances || instanceCount == 0) return;

    // Scale unit circle by radius in instance data if required
    DrawInstancedPolygons(m_unitCircleVertices.data(), m_unitCircleVertices.size(), instances, instanceCount, filled);
}

void OpenGLRenderer::DrawLine(const Math::Vector2f& start, const Math::Vector2f& end, const Math::Vector3f& color, f32 thickness) {
    (void)thickness;
    Math::Vector2f lineVerts[2] = {start, end};
    InstanceData singleInstance;
    singleInstance.color = {color.x, color.y, color.z, 1.0f};

    glUseProgram(m_shaderProgram);
    glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, &m_projectionMatrix[0][0]);
    glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboBase);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(Math::Vector2f), lineVerts);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboInstance);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstanceData), &singleInstance);

    glDrawArraysInstanced(GL_LINES, 0, 2, 1);
    glBindVertexArray(0);
}

void OpenGLRenderer::DrawPolygon(const Math::Vector2f* vertices, usize vertexCount, const Math::Vector3f& color, bool filled) {
    InstanceData singleInstance;
    singleInstance.color = {color.x, color.y, color.z, 1.0f};
    DrawInstancedPolygons(vertices, vertexCount, &singleInstance, 1, filled);
}

void OpenGLRenderer::DrawCircle(const Math::Vector2f& center, f32 radius, const Math::Vector3f& color, bool filled) {
    InstanceData singleInstance;
    singleInstance.position = center;
    singleInstance.scale = {radius, radius};
    singleInstance.color = {color.x, color.y, color.z, 1.0f};
    DrawInstancedCircles(radius, &singleInstance, 1, filled);
}

} // namespace Shape
