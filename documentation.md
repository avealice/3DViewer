3DViewer v1.0
The program is developed in the C programming language, adhering to the C11 standard, using the GCC compiler with additional QT modules.

The program's code is located in the "src" folder.

Program compilation is configured using a Makefile with a standard set of GNU program targets: all, install, uninstall, clean, dvi, dist, tests, gcov_report.

Installation is performed on the desktop.

The program can:

Load a wireframe model from an OBJ file format (supporting only vertex and face lists).
Translate the model by a specified distance along the X, Y, and Z axes.
Rotate the model by a specified angle around its X, Y, and Z axes.
Scale the model by a specified factor.
The graphical user interface includes:

A button for selecting a model file and a field for displaying its name.
A visualization area for the wireframe model.
Button(s) and input fields for translating the model.
Button(s) and input fields for rotating the model.
Button(s) and input fields for scaling the model.
Information about the loaded model, including the filename, vertex count, and edge count.
There is an option to manipulate the model using mouse interactions for rotation, translation, and scaling.

The program allows configuring the projection type (parallel and central).

The program enables customization of edge rendering options (solid, dashed), edge color, and edge thickness, as well as vertex rendering options (none, circle, square), vertex color, and vertex size.

The program allows choosing the background color.

User settings are preserved between program restarts.

The program allows saving rendered images to files in BMP and JPEG formats.

The program provides a special button to record short "screencasts" of the current user-affine transformations of the loaded object into GIF animations (640x480, 10fps, 5s).
