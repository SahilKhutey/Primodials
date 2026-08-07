#pragma once

namespace Shape {

class DynamicLibrary {
public:
    DynamicLibrary() = default;
    ~DynamicLibrary();

    bool Load(const char* path);
    void* GetSymbol(const char* name);
    void Unload();

private:
    void* m_handle = nullptr;
};

} // namespace Shape
