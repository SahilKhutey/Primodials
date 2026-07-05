# Engine/AssetPipeline/CMakeLists.txt
add_library(ShapeAssetPipeline STATIC
    "include/Shape/AssetPipeline/AssetRegistry.h"
    "include/Shape/AssetPipeline/AssetCooker.h"
    "include/Shape/AssetPipeline/AssetPackage.h"
    "include/Shape/AssetPipeline/AssetStreamer.h"
    "include/Shape/AssetPipeline/AssetTypes.h"
    "include/Shape/AssetPipeline/AssetWatcher.h"
    "src/AssetCooker.cpp"
    "src/AssetPackage.cpp"
    "src/AssetStreamer.cpp"
)

target_include_directories(ShapeAssetPipeline PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine>
)

target_link_libraries(ShapeAssetPipeline PUBLIC ShapeEngine)
