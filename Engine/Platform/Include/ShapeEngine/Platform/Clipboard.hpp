#pragma once

#include <string>

namespace Shape {

class Clipboard {
public:
    static void SetText(const std::string& text);
    static std::string GetText();
};

} // namespace Shape
