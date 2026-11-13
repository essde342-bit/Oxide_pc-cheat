#include "renderer.h"
#include <GLES3/gl3.h>
#include <cmath>
#include <cstring>

static GLuint  g_program = 0;
static GLuint  g_vbo = 0;
static GLint   g_locScreenSize = -1;
static int     g_screenW = 1080;
static int     g_screenH = 1920;
static bool    g_ready = false;

struct Vertex {
    float x, y;
    float r, g, b, a;
};

static const char* VS_SRC =
    "#version 300 es\n"
    "layout(location=0) in vec2 aPos;\n"
    "layout(location=1) in vec4 aColor;\n"
    "uniform vec2 uScreen;\n"
    "out vec4 vColor;\n"
    "void main(){\n"
    "  vec2 ndc = (aPos / uScreen) * 2.0 - 1.0;\n"
    "  ndc.y = -ndc.y;\n"
    "  gl_Position = vec4(ndc, 0.0, 1.0);\n"
    "  vColor = aColor;\n"
    "}\n";

static const char* FS_SRC =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec4 vColor;\n"
    "out vec4 frag;\n"
    "void main(){\n"
    "  frag = vColor;\n"
    "}\n";

static GLuint CompileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    return s;
}

void SetScreenSize(int w, int h) {
    if (w > 0 && h > 0) {
        g_screenW = w;
        g_screenH = h;
    }
}

void RendererInit() {
    if (g_ready) return;

    GLuint vs = CompileShader(GL_VERTEX_SHADER, VS_SRC);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, FS_SRC);

    g_program = glCreateProgram();
    glAttachShader(g_program, vs);
    glAttachShader(g_program, fs);
    glLinkProgram(g_program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // Кэшируем uniform location
    g_locScreenSize = glGetUniformLocation(g_program, "uScreen");

    glGenBuffers(1, &g_vbo);

    g_ready = true;
}

void RendererShutdown() {
    if (g_program) { glDeleteProgram(g_program); g_program = 0; }
    if (g_vbo)     { glDeleteBuffers(1, &g_vbo); g_vbo = 0; }
    g_ready = false;
}

static void DrawVerts(const Vertex* v, int count, GLenum mode) {
    if (!g_ready || count <= 0) return;

    glUseProgram(g_program);
    glUniform2f(g_locScreenSize, (float)g_screenW, (float)g_screenH);

    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), v, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)(2 * sizeof(float)));

    glDrawArrays(mode, 0, count);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void DrawRectFilled(float x, float y, float w, float h,
                    float r, float g, float b, float a) {
    Vertex v[6] = {
        {x,     y,     r,g,b,a},
        {x + w, y,     r,g,b,a},
        {x + w, y + h, r,g,b,a},
        {x,     y,     r,g,b,a},
        {x + w, y + h, r,g,b,a},
        {x,     y + h, r,g,b,a},
    };
    DrawVerts(v, 6, GL_TRIANGLES);
}

void DrawLine(float x1, float y1, float x2, float y2,
              float r, float g, float b, float a, float t) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float len = sqrtf(dx*dx + dy*dy);
    if (len < 0.001f) return;

    float nx = -dy / len * t * 0.5f;
    float ny =  dx / len * t * 0.5f;

    Vertex v[6] = {
        {x1+nx, y1+ny, r,g,b,a},
        {x1-nx, y1-ny, r,g,b,a},
        {x2+nx, y2+ny, r,g,b,a},
        {x1-nx, y1-ny, r,g,b,a},
        {x2-nx, y2-ny, r,g,b,a},
        {x2+nx, y2+ny, r,g,b,a},
    };
    DrawVerts(v, 6, GL_TRIANGLES);
}

void DrawRectOutline(float x, float y, float w, float h,
                     float r, float g, float b, float a, float t) {
    DrawLine(x,     y,     x + w, y,     r,g,b,a, t);  // top
    DrawLine(x,     y + h, x + w, y + h, r,g,b,a, t);  // bottom
    DrawLine(x,     y,     x,     y + h, r,g,b,a, t);  // left
    DrawLine(x + w, y,     x + w, y + h, r,g,b,a, t);  // right
}
