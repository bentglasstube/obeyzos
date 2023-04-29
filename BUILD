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
    "title_screen.cc",
    "maze_screen.cc",
  ],
  hdrs = [
    "title_screen.h",
    "maze_screen.h",
  ],
  deps = [
    "@libgam//:screen",
    "@libgam//:text",
    ":camera",
    ":dialog",
    ":maze_player",
    ":warehouse",
  ],
)

cc_library(
  name = "maze",
  srcs = ["maze.cc"],
  hdrs = ["maze.h"],
  deps = [
    "@libgam//:util",
    ":direction",
  ]
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
    "@libgam//:graphics",
    "@libgam//:rect",
    "@libgam//:spritemap",
    ":warehouse",
    ":config",
    ":direction",
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
