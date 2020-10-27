# Qt3D Widget

An attempt at implementing a subclass of `QWidget` for Qt3D. The only options to embedd Qt3D into an application with widgets is to use QML (which is not always feasible) or to use a `Qt3DWindow` and embedd it using `QWidget::createWindowContainer`. This unfortunatley draws the window's content over everything else, even neighboring widgets. This widget is supposed to be a native widget which is embeddable and usable like any other widget.

## Implementation

**Update:** The implementation is now not using Qt3D internals anymore but renders to a texture which is then used by OpenGL. Should be compatible with many Qt3D versions.

* If you want to disable the fps output simply build the library for release
* If you build the library yourself and alter qt3dwidget.h don't forget to copy it to the include folder, this doesn't happen automatically

### Screenshot

With `QPushButton` inside to show that it's now possible to add widgets inside Qt3D.

![img](https://i.imgur.com/EaIWRiF.png)
