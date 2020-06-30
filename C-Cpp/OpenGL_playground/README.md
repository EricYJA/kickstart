## OpenGL Playground

This is place where I learn OpenGL and practice using it in small projects. The main idea is to gain the knowledge of OpenGL, graphics coding basis, rendering pipeline. This document will also record some of my highlights when learning. First I could place some sweet reference tutorials that helped me a lot.

[Learn-OpenGL-GLFW-GLAD](learnopengl.com)
[Learn-OpenGL-GLFW-GLAD-Chinese-Version](learnopengl-cn.github.io)
[Antons-OpenGL4-Tutorial-GLFW-GLEW](antongerdelan.net/opengl/)

---

### Immediate Mode and Core-Profile

The first key point is the Immediate Mode and the Core Profile Mode. The key seperate point is version 3.3. Core Profile Mode is more modern and efficient, leaving more flexibility and computational detail to the programmer. Thus, learning OpenGL 3.3+ is a plus, since it is a watershed and later versions are built on 3.3.

---

### Libraries and Implementation

One thing need to be maeked is that OpenGL is just a standard/interface. Different platform need to implement their own libraries for logic use. Thus, we would have different implementations of libraries. 

**GLUT(OpenGL Utility Tool)**

- window control
- enent control
- state monitoring
- geometry drawing

This is an old implementation and are deprecated by many modern softwares.

**GLFW(Graphics Library Framework)**

- create context and manage windows
- Handle events and inputs

Still under maintenance, a modern replacemnent of GLUT

Recall that OpenGL is just an interface. The implemmentation requires the graphics card producer to implement. When choosing the driver, it's desogned to use a function pointer and determine the final version of function during runtime. Based on this feature, we have **GLEW**, **GL3W**, **GLAD** ... to use. 


