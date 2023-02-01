# Tetris clone

Simple Tetris implementation in C and OpenGL.

![](logo.png)

## Features

- Variable goal marathon mode
- Delayed autoshift
- 7-bag randomization
- SRS rotation system with wall kick
- Hardware accelerated graphics
- WebAssembly + WebGL support

## Todo

- T-spin detection
- Scoring system
- Font rendering for UI
- More visual feedback (particle system, screen shake)
- SFX

### Requirements

**Native build**

- A C99 compiler
- GNU make
- GLFW
- GLEW

**WebAssembly build**

- Clang
- A web server (ex: `python -m http.server`)

### Building

Running this will compile both the native version and the web version

```
make
```

### Debugging

Compile with debug symbols then use gdb to debug

```
make debug

gdb ./debug/app
```

### Running

For the native version, the executable is located at `./build/app`

For the web version use a web server (See [above](#requirements)) to host the
website.

## License

This project is licensed under the [MIT License](LICENSE).
