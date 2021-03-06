#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Define.h"
#include "Utilities.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "SOIL2/SOIL2.h"

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void ObjMove();

bool keys[1024];

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = 400;
GLfloat lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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
    
    Shader shader("resources/shaders/modelLoading.vs", "resources/shaders/modelLoading.fs");
    
    Model ourModel("resources/models/teapot.obj");
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)screen.x / (float)screen.y, 0.1f, 100.0f);
    
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;
        
        //キーが押されたりマウスを操作した際、該当する応答機能が何かしら作動したかどうかをチェックする
        glfwPollEvents();
        ObjMove();
        
        //glViewportで指定した範囲をこれで指定した色で塗り潰す
        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
        //指定したバッファを特定
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.Use();
        
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        ourModel.Draw(shader);
        
        //スクリーンバッファの交換(ダブルバッファのスワップ)
        glfwSwapBuffers(window);
    }
    
    //GLFW使用の際はプログラムの最後にこれを呼び出し、全てのウィンドウの破棄及びリソースの解放を行なう。
    //これを呼び出した後に再度GLFW機能を利用する場合は改めてglfwInitを呼び出す必要がある。
    glfwTerminate();
    
    return 0;
}

void ObjMove() {
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    }
    
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    }
    
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_RIGHT]) {
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    }
    
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_LEFT]) {
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    }
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
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement(xOffset, yOffset);
}
