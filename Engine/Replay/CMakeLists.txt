shape_add_library(ShapeReplay
    "include/Shape/Replay/ReplayFormat.h"
    "include/Shape/Replay/ReplayWriter.h"
    "include/Shape/Replay/ReplayPlayer.h"
    "src/ReplayWriter.cpp"
    "src/ReplayPlayer.cpp"
    "src/ReplayFormat.cpp"
)
target_link_libraries(ShapeReplay PUBLIC ShapeCore ShapeMath ShapeSimulation)
