shape_add_library(ShapeProcedural
    "include/Shape/Procedural/Noise.h"
    "include/Shape/Procedural/SimplexNoise.h"
    "include/Shape/Procedural/WorleyNoise.h"
    "include/Shape/Procedural/FBM.h"
    "include/Shape/Procedural/WorldGenerator.h"
    "include/Shape/Procedural/BiomeMap.h"
    "include/Shape/Procedural/ResourceDeposits.h"
    "include/Shape/Procedural/WorldDescription.h"
    "src/Noise.cpp"
    "src/SimplexNoise.cpp"
    "src/WorleyNoise.cpp"
    "src/WorldGenerator.cpp"
    "src/BiomeMap.cpp"
    "src/ResourceDeposits.cpp"
)
target_link_libraries(ShapeProcedural PUBLIC ShapeCore ShapeMath ShapeSimulation)
