# firefly-lib-tests
A repository dedicated to testing the firefly-lib.

1. This project is built with `make` and relies on you having built and installed firefly-lib. You can find it here [firefly-lib](https://github.com/rfmineguy/firefly-lib) (Sufficient instructions on how to build should be present, if not submit an issue) <br>
2. Once you have built that, there are a few targets in this repo<br>
  a. `make test_all` - builds a test that encompases most of the library (most actively updated)<br>
  b. `make test_resource` - builds a test that utilizes the resource manager<br>
  c. `make test_rendering` - builds a simple test that showcases *some* rendering capabilities<br>
  * There are a few others you can explore by looking at the `makefile` if you like<br>
