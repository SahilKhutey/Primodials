#pragma once

#include <mutex>

namespace Shape {

using Mutex = std::mutex;
using ScopedLock = std::lock_guard<Mutex>;

} // namespace Shape
