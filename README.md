![GitHub repo size](https://img.shields.io/github/repo-size/rfmineguy/firefly-lib-tests)

# firefly-lib-tests
A repository dedicated to testing the firefly-lib.

1. This project is built with `make` and relies on you having built and installed firefly-lib. You can find it here [firefly-lib](https://github.com/rfmineguy/firefly-lib) (Sufficient instructions on how to build should be present, if not submit an issue) <br>
2. Once you have built that, there are a few targets in this repo<br>
# Building tests
```makefile
  make <target>               : Build the specified target without running it
  make <target> run           : Build the specified target with running it
  make <target> leaks         : Leak detection for MacOS
  make <target> valgrind      : Leak detection for Linux
```
# Targets
* `testall` => Builds a file that encompases most of what the library is capable of doing (this is the most updated test)
* `testlog` => Contains a comprehensive test suite of how the logging functions/macros work
* `testwindow` => A simple straight to the point example of creating and closing a window
* `testrendering` => Showcases the different ways you can render geometry
* `testresource` => Contains usage of the resource management system
