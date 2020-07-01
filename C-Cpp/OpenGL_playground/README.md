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

---

### Basic Concepts

**VBO & VAO**

**VBO** (Vertex Buffer Object) worked as the memory buffer in the graphics memory to store all kinds of vertex data. When rendering, GPU could get the vertex data directly from the GPU memory without requesting it from CPU, achieving higher performance. 

We could request many VBO. They all have their own ID for address reference. Using this ID, we could easily finish data retrieval/modification operation. 

If we consider **VBO** as the grouping and abstraction for the vertex data. Then, we could consider **VAO** as the grouping and abstraction of **VBO**. **VAO** specifies the model's vertex reference in **VBO**. When drawing **VAO**, the machine would find all the requested **VBO** binding to the **VAO**. 

---

### Pipeline

Vertex Data (Primitives) ->
Vertex Shader (Primitive Assembly) ->
Geometry Shader ->
Tessellation Shader ->
Rasterization ->
Fragment Shader ->
Test and Binding -> Display
