#include "gcfcpp.h"

using namespace gcfcpp;

class Box: public Cell {
public:
    int x, y, width, height;

    Box(int x, int y, int width, int height) {
        x = x;
        y = y;
        width = width;
        height = height;
	}
	
    virtual void visit(GCVisitor* visitor) override {
        
	}
};

class BoxHolder: public Cell {
public:
    GCHandle<Box> box1, box2;

    BoxHolder(GCHandle<Box> box1, GCHandle<Box> box2):
        box1{box1}, box2{box2}
    {

    }

    virtual void visit(GCVisitor* visitor) override {
        visitor->visit(box1);
        visitor->visit(box2);
    }
};

int main(int argc, char* argv[]) {
    Heap heap;

	GCHandle<Box> box1 = heap.allocate<Box>(1, 1, 1, 1);
	GCHandle<Box> box2 = heap.allocate<Box>(2, 2, 2, 2);
	GCHandle<Box> box3 = heap.allocate<Box>(3, 3, 3, 3);
	GCHandle<BoxHolder> boxholder = heap.allocate<BoxHolder>(box1, box2);

	heap.set_root(boxholder);

    heap.force_garbage_collection();

	heap.statistics();
}
