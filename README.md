# fribstb
An STB Editor for Rose Online made with Qt

To build:
qmake
make

Features:

-Read, edit, save and save as stb files displayed in a QTableWidget
-Temporarily disabled: table rows and columns can be seeked using the following method: r|c # [ENTER]
-Temporarily disabled; Search stb files for a single cell value, row header or column header
-Convert stb files containing EUC-KR and SHIFT-JIS strings into equivalent STB using utf8

*it doesn't yet confirm before closing an unsaved file / before replacing another file when saving so be careful!
