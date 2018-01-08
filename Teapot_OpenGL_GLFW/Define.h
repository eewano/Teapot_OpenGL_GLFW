#ifndef Define_h
#define Define_h

#include <GLFW/glfw3.h>

//主にウィンドウサイズ等に使用
struct Vec2i {
    GLint x, y;
};

struct Vec2f {
    GLfloat x, y;
};

const Vec2i WINDOW_SIZE = {800, 600};

#endif /* Define_h */
