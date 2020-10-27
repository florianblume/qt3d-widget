# Qt3D Widget

An attempt at implementing a subclass of `QWidget` for Qt3D. The only options to embedd Qt3D into an application with widgets is to use QML (which is not always feasible) or to use a `Qt3DWindow` and embedd it using `QWidget::createWindowContainer`. This unfortunatley draws the window's content over everything else, even neighboring widgets. This widget is supposed to be a native widget which is embeddable and usable like any other widget.

## Implementation

<<<<<<< HEAD
**Update:** The implementation is now not using Qt3D internals anymore but renders to a texture which is then used by OpenGL. Should be compatible with many Qt3D versions.

* If you want to disable the fps output simply build the library for release
* If you build the library yourself and alter qt3dwidget.h don't forget to copy it to the include folder, this doesn't happen automatically
=======
**Needs at least Qt >= 5.14! Otherwise you can't run the `QAspectEngine` in manual mode. Automatic mode doesn't work because context sharing doesn't work and using the same context as the `QOpenGLWidget` doesn't work because the renderer can't make it current from another thread.**

### Former Implementation using additional OpenGL draw calls

The first idea was to render Qt3D offscreen to a texture and then use the texture's ID in a `QOpenGLWidget`. This didnt' work unfortunately, the screen stayed black. Capturing Qt3D's content using `QRenderCapture` did work and using the resulting `QImage` for the texture in the OpenGL widget's `paintGL` function showed everything correctly. But this takes way too long and is an unnecessary path since it involves copying Qt3D's fbo content back to CPU. If at some point I manage to make sharing the texture between the two work, this would be the cleaner solution even though there would be an additional draw call to OpenGL in the `paintGL` function. Cleaner because in contrast to the current implementation Qt3D would be in charge of running the render loop etc. and we wouldn't have to use Qt3D internal classes.

_This version used a simple vertex and fragment shader. They have been removed in the newer version._

### Current Implementation

The current implementation drives Qt3D manually instead of letting Qt3D do the work. This means setting the run mode on the `QAspectEngine` to `MANUAL` and manually triggering `processFrame` on the aspect engine and `doRender` on the `QRenderAspect`. Because context sharing doesn't seem to work properly (maybe it does and I'm doing it wrong) we set the context of the OpenGL widget on Qt3D making them use the same context. This way, Qt3D is able to directly draw to the framebuffer object of the OpenGL widget. It still feels a bit hacky, especially because it uses Qt3D's private classes. The better solution would be to share the texture in my opinion. Nevertheless it works and the widget is embeddable in other applications and should provide all necessary functionality.

**Warning:** The this version of the widget uses Qt internal classes because it needs to set the context on Qt3D. This means it might break in some versions and needs code apations.
>>>>>>> master

### Screenshot

With `QPushButton` inside to show that it's now possible to add widgets inside Qt3D.

<<<<<<< HEAD
![img](https://i.imgur.com/EaIWRiF.png)
=======
![img](https://i.imgur.com/EaIWRiF.png)
>>>>>>> master
