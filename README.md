# cfractal

To compile:

    $ make

To run:

    $ bin/cfractal 1 2 3 6 7 13 15 17 19

Substitute the list of numbers for which variations you want the fractal to
include (requires at least two).

`-w [NUM]` and `-h [NUM]` flags specify width and height, which both default to
1000.

Outputs to `out000.ppm`. You may convert this to ex. png with ImageMagick:

    $ convert out000.ppm out.png

To do animations, include the `-f [NUM]` flag specifying how many frames of
animations to render. This will produce `out000.ppm` through `out<f-1>.ppm`,
and then automagically combine them into `out.gif` if you have ImageMagick
installed.
