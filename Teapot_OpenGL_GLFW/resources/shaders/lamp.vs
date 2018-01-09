//OpenGL3のシンタックスを使用
#version 330 core

//main.cppで3要素を使用する場合は、GLSLでも3要素を使用しないといけない
layout (location = 0) in vec3 position;

//transformの情報
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    //position: xyzを表す 1.0f: w(大きさの比率...数値が高い程小さくなる)
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
