#pragma once

#include <vector>
#include <list>
#include <cstdint>
#include <iostream>
#include <format>

// TODO: page allocator?

namespace gcfcpp {

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

class GCVisitor;

// anything that is gc'd inherits from cell
class Cell {
public:
    Cell(): marked{false} {

	}

    void mark() {
        marked = true;
	}

	void unmark() {
        marked = false;
	}
	
    virtual void visit(GCVisitor* visitor) {

	}

    bool marked;
private:
};


// TODO doesn't need to use 64 bits
template <typename T>
class GCHandle {
public:
    GCHandle(Cell* cell): _handle{cell} {

	}

	T* ptr() {
        return reinterpret_cast<T*>(reinterpret_cast<char*>(_handle) + sizeof(Cell));
	}

	Cell* cell() {
		return _handle;
	}
	

private:
    Cell* _handle;
};

class GCVisitor {
public:
	GCVisitor() {}

	template <typename T>
	void visit(GCHandle<T> handle) {
	    handle.cell()->mark();
		handle.cell()->visit(this);
	}

	void visit(Cell* handle) {
	    handle->mark();
		handle->visit(this);
	}
};

class Heap {
public:
    Heap() {

	}

	template <typename T, typename... Args>
	GCHandle<T> allocate(Args... args) {
		auto* el = new T(args...);
		list.push_back(el);
		cells_allocated++;
		return GCHandle<T>(el);
	}

	template <typename T>
	void set_root(GCHandle<T> handle) {
		roots.push_back(handle.cell());
	}

	void force_garbage_collection() {
        for(auto& root: roots) {
            visitor.visit(root);
		}

		cells_freed += std::erase_if(list, [](auto* x) { return !x->marked; });
	}

	void statistics() {
		std::cout << std::format("Roots: {}\n", roots.size());
		std::cout << std::format("Cells Allocated: {}, Cells Freed: {}\n", cells_allocated, cells_freed); 
	}

private:
    std::list<Cell*> list; // TODO better free list
    std::vector<Cell*> roots;
	GCVisitor visitor;

	int cells_freed = 0, cells_allocated = 0;
};

};

