# hl-bump
Bump Allocator for Hashlink


## Motivation
For realtime games, there can a lot of temporary data required to process a frame.  Rendering, physics, various caches, etc, can all be a drag on the garbage collector as either these objects are recreated every frame, or artifically held for the duration of the program.  Either way, this wastes cycles.

We need an allocator that is incredibly fast and doesn't thrash the GC.

## Bump Allocators
Bump allocators are as brain dead as they come.  You pre-allocate a chunk of memory, and then every temporary allocation within a single frame is simply appended onto the last one.  This means that temporary memory has the lifespan dictated by the allocator.  

To free the memory, you simply move the bump allocator to the beginning and considerall the objects freed.

This can be tricky to use as you must make sure than none of the allocated objects have made their way into the data structures that persist past a frame.

The trade of here, is an incredibly fast allocator that can be used inside the tight duration of frame without having to worry about freeing or distracting the GC, or even in fact, of trigger the GC.

## Status
The allocator is fairly straight forward, but is still missing some features.


## Usage
```haxe
var a = new bump.BumpAllocator( MAX_BYTES );

var x = a.alloc(MyType); // Allocates a single instance
var y = a.allocCompactArray(MyType, 10); // allocates a compact array of 10 elements, still within the bymp allocator

a.clear(); // effectively frees all allocations, or atleast consideres them invalid

var wl = a.watermark();
var z = a.alloc(MyOtherType);
a.drainTo(wl); // resets the allocation to before Z was allocated
```