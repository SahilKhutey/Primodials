# Code API Reference

Complete API reference for Polygonal Primordials (`ShapeEngine`).

---

## 🏛️ World & ECS (`Shape::ECS2::World2`)

```cpp
namespace Shape::ECS2 {
    class World2 {
    public:
        World2();
        EntityId create();
        void destroy(EntityId e);
        size_t entity_count() const noexcept;

        template<typename T>
        T& add(EntityId e, T value = T{});

        template<typename T>
        void remove(EntityId e);
    };
}
```

---

## 🎨 Theme Manager (`ShapeEngine::Themes::ThemeManager`)

```cpp
namespace ShapeEngine::Themes {
    class ThemeManager {
    public:
        bool initialize(const std::string& themeDir);
        std::shared_ptr<Theme> loadTheme(const std::string& themeId);
        void setTransitionTime(float seconds);
    };
}
```

---

## 🌍 Locale Manager (`ShapeEngine::Localization::LocaleManager`)

```cpp
namespace ShapeEngine::Localization {
    class LocaleManager {
    public:
        static LocaleManager& get();
        bool loadLocale(const std::string& localeCode);
        std::string get(const std::string& key) const;
    };
}
```
