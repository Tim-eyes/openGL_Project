# openGL_Project

Author: HE PEILIN 10078,
        CHEN YUXUAN 10011

## Introduction

The core of project is to be familiar with transformations, texture mapping, lighting and window event handling, so that general animation of solar system which consists of the **Sun**, eight planets (**Venus**, **Earth**, **Mars**, **Jupiter**,**Saturn**, **Uranus**, and **Neptune**) and the **Moon** can be realized.

## Core and supporting libraries

The corresponding libraries are documented below:

| Library              |        | Description                        | Related Files                    |
|----------------------|--------|------------------------------------|----------------------------------|
| Core Library         | OpenGL | Provided by Visual Studio 2019 SDK | *opengl32.lib*                     |
| Supporting Libraries |  GLFW  | Window management                  | *glfw3.lib*                        |
|                      |  GLEW  | Extension library                  | *glew32.lib*, *glew32.dll*           |
|                      |   GLM  | Math library                       | source codes in the folder *glm* |
|                      |  SOIL2 | Texture management                 | *soil2-debug.lib*                  |

Also, you can choose other libraries. It is generally preferable to use texture loading library.
Some examples are **FreeImage**, **DevIL**, **GLI**, **GLUT** and **Glraw**.

The framework of these libraries can refer picture below:
![Supporting library folder structure](/Report/image/Supporting%20library%20folder%20structure.png)

How to using **Visual Studio 2019** for **OpenGL** can refer our course slide [Basic Drawing in OpenGL](/openGL_Project/Lecture3%20Basic%20Drawing%20in%20OpenGL-20_21.pdf)

## Module

The main model objects to animate asters are sphere and torus respectively. And we choose **stack** in **C++ STL** relatively straightforward to adapt as a **matrix stack**.

### Camera

**W**,**S**,**A** and **D** in keyboard to adjust the position of camera as your view.

|Keyboard|Function |
|--------|------------------|
|W|translate forward|
|S|translate backward|
|A|translate left|
|D|translate right|

## Output Sample

The output in window can be seen in figure below:
![Sample of Output](/Report/image/Sample-of-output.gif)

## Final Report

More specific information can be seen in our submission of final report [Project in CS104 – Animation of the Solar](/openGL_Project/Project%20in%20CS104%20–%20Animation%20of%20the%20Solar.pdf).
