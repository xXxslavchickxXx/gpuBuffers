#pragma once

#include <memory>
#include <iostream>
#include "vertex_buffer.hpp"

namespace ag {
    class vertex_buffer_ref {
        std::weak_ptr<vertex_buffer> ptr;
    public:
        vertex_buffer_ref(std::shared_ptr<vertex_buffer> p) : ptr(p) {}

        std::shared_ptr<vertex_buffer> lock() const {
            return ptr.lock();
        }

        vertex_buffer* operator->() {
            auto sp = ptr.lock();
            if (!sp) throw std::runtime_error("Buffer is dead");
            return sp.get();
        }

        operator bool() const { return !ptr.expired(); }
    };
}