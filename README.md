# ChartDemo

This application draws charts using either Direct2D or OpenGL.

## Application overview

| ![obraz](https://user-images.githubusercontent.com/50883639/173693884-68b69f6b-7dfb-44c8-9444-2a49ad534b1d.png) |
|:--:|
| Sample window |

Main part of the application window is taken by the `Canvas`, presenting a chart on the screen. On the right there are various button groups,
which allow modifying chart's characteristics. Main window can be freely resized, and chart dynamically resizes to fit available area.
The window's title bar displays duration of last frame draw, as well as an average duration updated every 100 frames.

## Charts

User can choose between two types: line chart or scatter chart. They both place datapoints in the same places on the chart, but use different methods of drawing.
Scatter chart represents each data point as a circle, and line chart draws a line segment between each two consecutive points.
It means, that their common methods (setting chart parameters, generating data points and positioning them on the chart etc.) can be implemented in 
abstract class `Chart`, with subclasses `LineChart` and `ScatterChart` overriding pure virtual method `draw` inherited from `Drawable` in their own way.

## Drawing

Both Direct2D and OpenGL are used through their common `RenderTarget` interface. Thanks to that, Drawable objects can be rendered on screen using the same method calls,
regardless of the active rendering method. More implementations of `RenderTarget` using other rendering libraries could be added in the future.

### Direct2D

Before object of `D2DRenderTarget` can be constructed, Direct2D and DirectWrite factories must be created. These can later be used to create `ID2D1HwndRenderTarget`
bound to the Canvas window, and a text format used to draw labels on the charts. Drawing methods of `D2DRenderTarget` are responsible for creating brushes with requested colors,
and calling suitable `ID2D1HwndRenderTarget` drawing functions.

### OpenGL

When constructing `OpenGlRenderTarget` a couple of buffers are created
* circleBuffer stores an array of points approximating a circle
* instancePointBuffer will be storing an array of data points' coordinates on the chart during scatter chart draw
* lineBuffer will be storing an array of data points' coordinates on the chart during line chart draw

Two shader programs are also created: one used when drawing points, and one used when drawing lines. They use different vertex shaders, but share a fragment shader, responsible only for setting color.

#### Drawing a scatter chart

Rendering a scatter chart may require drawing hundreds of points on the screen. To optimize this process, a reference to the whole Points vector is passed to the `RenderTarget` at once.
Each point is drawn as a collection of triangles approximating a circle. Precalculated positions of their vertices are stored in circleBuffer.
Positions of Points will have to be converted from (0 - width)x(0 - height) coordinates, with (0, 0) representing the top left corner, 
to OpenGL coordinates, with (0, 0) representing the center and values ranging from -1 to 1.
As each point is drawn as the same GL_TRIANGLE_FAN in different position, `glDrawArraysInstanced` can be used. Thanks to that, for each vertex a shader will receive its requested translation.
This value is then converted to OpenGL coordinates and combined with vertex data from circleBuffer. 

#### Drawing a line chart

When drawing a line chart, the datapoints are interpreted as a GL_LINE_STRIP. This drawing call uses simpler vertex shader, responsible only for converting points to OpenGL coordinates.

## Sample charts

![obraz](https://user-images.githubusercontent.com/50883639/173705817-34d6082f-fbd9-46b5-9b26-9ddaa6546c35.png)

![obraz](https://user-images.githubusercontent.com/50883639/173705956-958e5d70-13cc-4577-8927-a06cdacb74d0.png)

![obraz](https://user-images.githubusercontent.com/50883639/173706012-9bae078b-f1df-425a-bc09-59f6da006940.png)

![obraz](https://user-images.githubusercontent.com/50883639/173706141-8361b76b-1c2e-4b9f-9aae-ed8ab24c3d65.png)

![obraz](https://user-images.githubusercontent.com/50883639/173706182-92715ed6-60dd-4a05-a900-9605237a8a1d.png)

![obraz](https://user-images.githubusercontent.com/50883639/173706421-4734c6dc-653d-47bd-ba3e-139d03d3fcac.png)
