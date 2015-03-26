# netEditor

Traffic network road editor, implemented as a proof of concept.

Example usage - https://www.youtube.com/watch?v=KgPSREMmA_0

## Building
Only dependency is Qt4+ (core) 
```
qmake
make
./neteditor
```

### on Linux
* since on Ubuntu 14.10 Qt5 is used as default you need to enforce the use of Qt4.

#### the app
can be compiled with
```
qmake-qt4
make
./neteditor
```

#### the plugins
* won't compile on the command line...
  qmake in the `plugin/` directory will result in separate plugin subfolder problems of the **`macx-clang-libc++`** flavor
* It does however qmake, build and more or less work when loaded as **QT creator** projects with Qt4


## TODO
There is so much to do, i don't know where to start...
* Unit tests (really, all kinds of tests)
* Prebuilt packages (Travis anyone?)
* Define coding style
* ...

## Contributing
Everybody is more than welcome to contribute to this project.

## Authors
Jose Pereira
