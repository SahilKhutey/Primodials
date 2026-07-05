#include "Shape/UI/BitmapFont.h"
#include "Shape/UI/FontAtlas.h"
#include "Shape/UI/GraphWidget.h"
#include "Shape/UI/ColorPicker.h"
#include "Shape/UI/DraggableWindow.h"
#include "Shape/UI/IOverlayUI.h"
namespace Shape::UI {
    std::unique_ptr<IOverlayUI> create_imgui_overlay() { return nullptr; }
}
