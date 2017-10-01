# [Glitter](http://polytonic.github.io/Glitter/)
![Screenshot](http://i.imgur.com/MDo2rsy.jpg)

## Summary
Glitter is a dead simple boilerplate for OpenGL, intended as a starting point for the tutorials on [learnopengl.com](http://www.learnopengl.com) and [open.gl](https://open.gl). Glitter compiles and statically links every required library, so you can jump right into doing what you probably want: how to get started with OpenGL.

## Getting Started
Glitter has a single dependency: [cmake](http://www.cmake.org/download/), which is used to generate platform-specific makefiles or project files. Start by cloning this repository, making sure to pass the `--recursive` flag to grab all the dependencies. If you forgot, then you can `git submodule update --init` instead.

```bash
git clone --recursive https://github.com/NathanZabriskie/Glitter
cd Glitter
cd Build
```

Now generate a project file or makefile for your platform. If you want to use a particular IDE, make sure it is installed; don't forget to set the Start-Up Project in Visual Studio or the Target in Xcode.

```bash
# UNIX Makefile
cmake ..

# Mac OSX
cmake -G "Xcode" ..

# Microsoft Windows
cmake -G "Visual Studio 14" ..
cmake -G "Visual Studio 14 Win64" ..
...
```

**Note: If you are using Visual Studio on Windows set Glitter as your startup project in the solution created by cmake.**

If you compile and run, you should now be at the same point as the [Hello Window](http://www.learnopengl.com/#!Getting-started/Hello-Window) or [Context Creation](https://open.gl/context) sections of the tutorials. Open [main.cpp](https://github.com/Polytonic/Glitter/blob/master/Glitter/Sources/main.cpp) on your computer and start writing code!

## Documentation
Many people overlook how frustrating it is to install dependencies, especially in environments lacking package managers or administrative privileges. For beginners, just getting set up properly set up can be a huge challenge. Glitter is meant to help you overcome that roadblock.

Glitter provides the most basic windowing example. It is a starting point, and tries very hard not to enforce any sort of directory structure. Feel free to edit the include paths in `CMakeLists.txt`. Glitter bundles most of the dependencies needed to implement a basic rendering engine. This includes:

Functionality           | Library
----------------------- | ------------------------------------------
Mesh Loading            | [assimp](https://github.com/assimp/assimp)
Physics                 | [bullet](https://github.com/bulletphysics/bullet3)
OpenGL Function Loader  | [glad](https://github.com/Dav1dde/glad)
Windowing and Input     | [glfw](https://github.com/glfw/glfw)
OpenGL Mathematics      | [glm](https://github.com/g-truc/glm)
Texture Loading         | [stb](https://github.com/nothings/stb)

**Added for BYU CS455**
### ShaderLoading
In addition to the libraries included in the original Glitter repository we have provided a file to help with the tedious process of loading and compiling GLSL shaders. ShaderHelpers.h and ShaderHelpers.cpp are included in the project by default. To load your shaders call LoadProgram.

```GLuint myShader = LoadProgram("path/to/vertex/shader", "path/to/fragment/shader");```

This will attempt to load and compile the provided vertex and fragment shaders and output any errors to the console. It returns the ID used by OpenGL to identify the shader on success and 0 on failure. Feel free to modify these files or ignore them if they don't meet your needs, they are simply provided for your convenience. 

### Loading 3D Models
We have also provided a simple 3D model loader which uses the assimp library included in Glitter to load models 
from a variety of file formats. To use it call
```
mlModel model;
bool success = LoadModel("path/to/model/directory", "modelName.fileExtension", model);
```
This function will attempt to load the model contained in "path/to/model/directory/modelName.fileExtension" 
and returns true on success. On failure it will return false and print errors to the console.

Since a model can contain submeshes that each have a different material/texture, mlModel is simply a list of mlMesh structs.
Each mlMesh contains the vertex information for a single submesh and the path of the mesh's texture if it has one. For more
information about what each mlMesh contains look at MeshLoader.h and MeshLoader.cpp.

## License
>The MIT License (MIT)

>Copyright (c) 2015 Kevin Fung

>Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

>The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
