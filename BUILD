package(default_visibility = ["//visibility:public"])

cc_binary(
  name = "ld53",
  data = ["//content"],
  linkopts = [
    "-lSDL2",
    "-lSDL2_image",
    "-lSDL2_mixer",
  ],
  srcs = ["main.cc"],
  deps = [
    "@libgam//:game",
    ":screens",
  ],
)

cc_library(
  name = "config",
  srcs = ["config.cc"],
  hdrs = ["config.h"],
  deps = [
    "@libgam//:game",
  ],
)

cc_library(
  name = "screens",
  srcs = [
    "boss_screen.cc",
    "maze_screen.cc",
    "puzzle_screen.cc",
    "title_screen.cc",
    "win_screen.cc",
  ],
  hdrs = [
    "boss_screen.h",
    "maze_screen.h",
    "puzzle_screen.h",
    "title_screen.h",
    "win_screen.h",
  ],
  deps = [
    "@libgam//:backdrop",
    "@libgam//:screen",
    "@libgam//:spritemap",
    "@libgam//:text",
    ":bar",
    ":bozos",
    ":buster",
    ":camera",
    ":dialog",
    ":game_state",
    ":maze_player",
    ":warehouse",
    ":worker",
  ],
)

cc_library(
  name = "maze",
  srcs = ["maze.cc"],
  hdrs = ["maze.h"],
  deps = [":direction"]
)

cc_library(
  name = "warehouse",
  srcs = ["warehouse.cc"],
  hdrs = ["warehouse.h"],
  deps = [
    "@libgam//:graphics",
    "@libgam//:rect",
    "@libgam//:spritemap",
    ":config",
    ":maze",
  ]
)

cc_library(
  name = "direction",
  hdrs = ["direction.h"],
)

cc_library(
  name = "maze_player",
  srcs = ["maze_player.cc"],
  hdrs = ["maze_player.h"],
  deps = [
    ":maze_char",
    ":warehouse",
  ],
)

cc_library(
  name = "camera",
  srcs = ["camera.cc"],
  hdrs = ["camera.h"],
  deps = [
    "@libgam//:rect",
    ":config",
  ],
)

cc_library(
  name = "dialog",
  srcs = ["dialog.cc"],
  hdrs = ["dialog.h"],
  deps = [
    "@libgam//:box",
    "@libgam//:text",
    ":config",
  ],
)

cc_library(
  name = "maze_char",
  srcs = ["maze_char.cc"],
  hdrs = ["maze_char.h"],
  deps = [
    "@libgam//:graphics",
    "@libgam//:rect",
    "@libgam//:spritemap",
    ":warehouse",
    ":config",
    ":direction",
  ],
)

cc_library(
  name = "worker",
  srcs = ["worker.cc"],
  hdrs = ["worker.h"],
  deps = [
    ":maze_char",
  ],
)

cc_library(
  name = "game_state",
  hdrs = ["game_state.h"],
  deps = ["@libgam//:util"],
)

cc_library(
  name = "buster",
  srcs = ["buster.cc"],
  hdrs = ["buster.h"],
  deps = [
    ":maze_char",
    ":maze_player",
  ],
)

cc_library(
  name = "bozos",
  srcs = ["bozos.cc"],
  hdrs = ["bozos.h"],
  deps = [
    "@libgam//:graphics",
    "@libgam//:rect",
    "@libgam//:sprite",
    "@libgam//:spritemap",
    ":config",
  ],
)

cc_library(
  name = "bar",
  srcs = ["bar.cc"],
  hdrs = ["bar.h"],
  deps = [
    "@libgam//:graphics",
    "@libgam//:spritemap",
  ],
)
