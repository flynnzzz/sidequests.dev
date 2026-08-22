# border_pane.c

My attempt at replicating a JavaFX `BorderPane` in C with the `notcurses` library.

## 1. requirements

- CMake 3+
- [notcurses](https://github.com/dankamongmen/notcurses)

## 2. build with cmake

```bash

  # (after cloning the repo)
  mkdir build
  cd build
  cmake ..
  cmake --build .

  # then execute with:
  ./borderpane

```

## 3. notes

- This side-quest has been a great learning experience.
- Trying to re-implement a `BorderPane` in pure C helped me improve my understanding
  of UI building fundamentals.
- Learning the `notcurses` API brought me closer to being able to create a TUI application.
