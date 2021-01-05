# Qt3D Widget

An attempt at implementing a subclass of `QWidget` for Qt3D. The only options to embedd Qt3D into an application with widgets is to use QML (which is not always feasible) or to use a `Qt3DWindow` and embedd it using `QWidget::createWindowContainer`. This unfortunatley draws the window's content over everything else, even neighboring widgets. This widget is supposed to be a native widget which is embeddable and usable like any other widget.

## Requirements

* Qt >= 5.13

Although it doesn't use any Qt3D internals anymore it needs `QAbstractTexutre`'s function `handle()` to get a handle to the OpenGL texture. This function is only available from Qt >= 5.13. Any suggestions how to avoid this dependency are welcome.

## Usage

**IMPORTANT**: You need to add

    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    
to the `main` of your project *BEFORE* calling `QApplication(x, y)` or in whatever way you construct your application.

You can either use the static library (`libqt3d-widget.a`) in your project, which will be compiled into the executable. Or you use the dynamic linked library (`libqt3d-widget.so`).

**Note:**

If you want to use the dynamic linked library specify

    LIBS += -L../path/to/your/libs/folder/ -lqt3d-widget

If you want to use the static library but keep the dynamic library in the same folder you must specify the full file name like so:

    LIBS += ../path/to/your/libs/folder/libqt3d-widget.a

## Implementation

**Update:** The implementation is now not using Qt3D internals anymore but renders to a texture which is then used by OpenGL. Should be compatible with many Qt3D versions.

* If you want to disable the fps output simply build the library for release
* If you build the library yourself and alter qt3dwidget.h don't forget to copy it to the include folder, this doesn't happen automatically

### Screenshot

With `QPushButton` inside to show that it's now possible to add widgets inside Qt3D.

![Imgur](https://i.imgur.com/EaIWRiF.png)
