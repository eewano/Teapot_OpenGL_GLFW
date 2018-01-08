#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Define.h"
#include "Utilities.h"
#include "Shader.h"

#include "SOIL2/SOIL2.h"

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);

bool keys[1024];

GLFWwindow *window;

int main(int argc, const char * argv[]) {
    
    //当プロジェクトの作業ディレクトリを表示
    std::cout << "Current directory is " << GetCurrentWorkingDir().c_str() << ".\n";
    
    //GLFWの初期化(失敗したら終了)
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }
    
    //GLFWにて要求されるオプションのセット
    //(window生成時に一緒に作成されるコンテキストやスワップチェイン、深度バッファの情報等の指定)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Macの際には必要
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    //GLFWの機能を用いて、GLFWwindowオブジェクトを生成
    window = glfwCreateWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Teapot OpenGL GLFW", nullptr, nullptr);
    
    //window生成失敗時のプログラムの終了処理
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window.\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    Vec2i screen;
    
    //glViewportで必要とされるフレームバッファのサイズを取得
    glfwGetFramebufferSize(window, &screen.x, &screen.y);
    
    //エラー時に呼ばれるコールバックを設定
    glfwSetErrorCallback(ErrorCallback);
    
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    //windowのコンテキストをカレントにする
    glfwMakeContextCurrent(window);
    
    //最新の方法でポインタや拡張機能を検索し、glewを有効にする(コアプロファイルで必要)
    glewExperimental = GL_TRUE;
    //GLEWの初期化
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW.\n";
        return EXIT_FAILURE;
    }
    
    //ビューポートの定義(ウィンドウの左下隅が原点で、そこからx, yの数値分が左下の原点座標となる)
    glViewport(0, 0, screen.x, screen.y);
    
    glEnable(GL_DEPTH_TEST);
    
    
    
    while (!glfwWindowShouldClose(window)) {
        //キーが押されたりマウスを操作した際、該当する応答機能が何かしら作動したかどうかをチェックする
        glfwPollEvents();
        
        //glViewportで指定した範囲をこれで指定した色で塗り潰す
        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
        //指定したバッファを特定
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //スクリーンバッファの交換(ダブルバッファのスワップ)
        glfwSwapBuffers(window);
    }
    
    //GLFW使用の際はプログラムの最後にこれを呼び出し、全てのウィンドウの破棄及びリソースの解放を行なう。
    //これを呼び出した後に再度GLFW機能を利用する場合は改めてglfwInitを呼び出す必要がある。
    glfwTerminate();
    
    return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
    
}
