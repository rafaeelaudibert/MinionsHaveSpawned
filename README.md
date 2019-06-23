# Final Assigment INF01047 - Fundamentals of Computer Graphics - Minions Have Spawned

The objective of this assignment is to consolidate the knowledge obtained in the subject taught by the professor [Eduardo Gastal](https://inf.ufrgs.br/~eslgastal) - INF01047 - Fundamentals of Computer Graphics, within the scope of the Computer Science course, at the [INF](https://inf.ufrgs.br) - [UFRGS](https://ufrgs.br).

This knowledge gained on concepts about the representation and visualization of two-dimensional (2D) and three-dimensional (3D) objects is consolidated through the development of a graphical application. The basic concepts of Computer Graphics, such as visualization in three-dimensional environments, interaction, collision detection, and use of textures, among others, were exercised.

**This game is entitled Minions Have Spawned**

## Especification
You must develop a graphical application of your choice that meets all the requirements described in the list below. The final grade of the work will be calculated based on these requirements, so it is important that you strive to achieve them.

Graphical application ideas:

- Racing game like [Mario Kart](https://youtu.be/ASWgJvuQhTA?t=894)
- Game like [Bomberman](http://www.youtube.com/watch?v=5Xe0aPNz39Q)
- Game like [Tower Defense](https://youtu.be/hQLsVTXb5RA?t=238)
- [File System 3D Viewer](https://youtu.be/MmOoIizm9kU?t=3)
- [3D modeling](https://youtu.be/AnfVrP6L89M?t=112) system, like [Wings 3D](https://en.wikipedia.org/wiki/Wings_3D) or [SketchUp](https://en.wikipedia.org/wiki/SketchUp).

### General Advices
- You can use the programming language of your choice.
    - It is recommended, however, the use of C++ language with `OpenGL`. It will facilitate the creation of a high performance application with real-time interaction.
    - You can use a **maximum of 30% of code ready** for this work (without considering the code that we developed in the practical classes). Any use of code beyond this limit will be considered plagiarism and the corresponding work will receive a grade of zero.

- Your application should enable **real-time interaction**.
    - For example, if you develop a game, it can not be "slow" to the point of negatively impacting the gameplay.

- Its application must have some objective and non-trivial control logic.
    - For example, a computer game has a non-trivial logic. But an application that simply carries a 3D geometric model and allows its visualization is trivial.

- Your application **must use the matrices we have seen in class** for model matrix, projection matrix, and specs of the View Matrix.
    - You can not use existing libraries for camera calculation, transformations, etc. For example, the following functions, commonly used in web-based tutorials, can not be used:
        - `gluLookAt()`, `gluOrtho2D()`, `gluPerspective()`, `gluPickMatrix()`, `gluProject()`, `gluUnProject()`, `glm::lookAt()`, `glm::ortho()`, `glm::perspective()`, `glm::pickMatrix()`, `glm::rotate()`, `glm::scale()`, `glm::translate()`, among others.

- Your application should enable **user interaction** with the mouse and keyboard.

- The quality of the presentation of the final work, besides the presence of the duo on the days of presentations of other colleagues, will count towards the final note of the work. **Each member of the pair will receive independent participation score**. Any kind of plagiarism will have a zero score.

### Requirements
Your application should include implementation of the following concepts of Computer Graphics:

- [x] Virtual objects represented through **complex polygonal meshes** (triangle meshes).
    - [x] At a minimum your application should include a geometric model of complexity equal to or greater than the `cow.obj` template available at [this link](https://moodle.inf.ufrgs.br/mod/resource/view.php?id=81157).
    - [x] The greater the variety of geometric models, the better.

- [x] **Geometric transformations** of virtual objects.
    - [x] Through interaction with the keyboard and/or mouse, the user must be able to control geometric transformations applied to virtual objects (not just camera control).

- [x] Control of virtual cameras.
    - [x] At the very least your application should implement:
        - [x] a **look-at camera**, and;
        - [x] a **free camera**.

- [x] At least one virtual object must be copied with two or more **instances**, that is, using two or more model matrix applied to the same set of vertices.

- [ ] **Intersection tests between virtual objects**.
    - [ ] **At a minimum** your application should use **three types** of intersection test):
        - [x] Cube-Cube test
        - [x] Point-Sphere test
        - [ ] Cube-Point test

- [x] **Illumination** models of geometric objects.
    - [x] **At a minimum** your application should include objects with the following lighting models:
        - [x] **diffuse** (Lambert), and;
        - [x] **Blinn-Phong**.
    - [x] **At a minimum** your application should include objects with the following interpolation models for lighting:
        - [x] **At least** one object with a **Gouraud** model: the lighting model is evaluated for each vertex using its normals, generating a color, which is interpolated for each pixel during the rasterization.
        - [x] **At least** one object with **Phong** model: the normals of each vertex are interpolated for each pixel during the rasterization, and the illumination model is evaluated for each pixel, using these interpolated normals.

- [x] Mapping of **textures**.
    - [x] **At a minimum**, a virtual object of your application must have its color defined through a texture defined through an image.

- [x] **Bézier** curves.
    - [x] **At least one** virtual object of your application must have its movement defined through a cubic Bezier curve. The object must move smoothly along space on a curved path (not straight).

- [x] **Motion animation based on time**.
    - [x] **All object movements** (including camera) must be computed based on time (ie, movements must always occur at the same speed regardless of the CPU speed where the program is running).


### Extras

Optionally, you might implement the following extra concepts:
- [ ] [Sound effects](http://www.ambiera.com/irrklang/)
- [ ] [Particle system](http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/)
- [ ] [Shadows](http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/)
- [x] [Billboards / Sprites](http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/billboards/)
- [ ] Graphical interface (buttons, etc.)
- [ ] Selecting virtual objects with the mouse (picking). Example of implementation: Picking with custom Ray-OBB function.
