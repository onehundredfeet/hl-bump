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

You can have as many allocators as you want. You can use one per thread if you want, you can use one per update frequence (per frame, per second, etc).  However you want to slice it, it is up to you.

## Warning
This library is not intended for people new to Haxe or people making non-realtime games.  We highly recommened you use the default haxe allocation & arrays unless you really need high-performance realtime memory allocation.

It will require you to know what you are doing with multi-threading and managing your allocations.  It is very easy to accidentally clear a pool with objects you are still referencing in other data structures.

You MUST carefully manage the references yourself.  This library DOES NOT check validity before clearing the bump buffer.

## Status
The allocator is fairly straight forward, but is still missing some features.

## Novel features
### Compact Arrays
This is a concept introduced in Hashlink recently.  Compact Arrays as defined in haxe are a contiguous allocation of object instances that can be indexed indivdiually. This is a different pattern than regular arrays which hold references to objects.

Not all objects are supported yet, only Objects (Instances of a class) or Hashlink Structs (Classes with the struct meta data).  Anonymous structs are not supported.  Dynamics are not supported.  Primitive types.

Keep in mind these are temporary 

### Fixed Arrays (Unimplemented)
These are akin to Hashlink's Native Array, but all can be built from the same bump allocator.  You can allocated arrays of primitive in fixed locations for use with external libraries.  Accessing on a per-element basis will require a functional call, which may make them slower for a lot of tight access than one directly supported by the jit, but for most, this will be negligable. 

### Raw bytes (Unimplemented)
You can simply allocate raw bytes using allocBytes().  Again, this will be temporary, but you will have to manage the lifetime of anything referencing these bytes.  This functionality is fantastic when preparing any kind of low level messages or datastructures to be passed and copied by a subsystem.

### Watermarking
This is fairly unsafe, but can result in significant reduction in required buffer size.  You can ask for a watermark at any time. This will return the number of bytes allocated to this point.  You can then return the allocator to this state at any time using drainTo().  This will not drain past the watermarked location.  This allows you re-use later parts of the allocator while preserving earlier parts, reducing the need for explicit frees or unnecessarily large buffers.

It is up to you to manage thread safety and unsafe references.

## API
```haxe
var a = new bump.BumpAllocator( MAX_BYTES );

var x = a.alloc(MyType); // Allocates a single instance
var y = a.allocCompactArray(MyType, 10); // allocates a compact array of 10 elements, still within the bymp allocator

a.clear(); // effectively frees all allocations, or atleast consideres them invalid

var wl = a.watermark();
var z = a.alloc(MyOtherType);
a.drainTo(wl); // resets the allocation to before Z was allocated
```
## Installing
You will need to get it from git and update submodules to get the IDL dependency.

`git clone https://github.com/onehundredfeet/hl-bump.git`

`cd hl-bump`

`git submodule update --init --recursive`

Once that's done, you'll need to build the library.  


`mkdir build.debug`

`cd build.debug`

`cmake ..`

Then when done, you now have a viably build directory. Run make (ideally make -j 5 for up to 5 threads)

`make` or `make -j XXX` where XXX is the number of threads you want

When that's done, it should output a bump.hdll in the build.debug directory.

I would recommend, for development, using a soft link (if in a system that supports it), over copying so that it's easy to update.  But you can always copy it to the root of any Haxe project you want to use it in.  The hdll is necessary in addition to adding the -lib hl-bump to your .hxml.

`cd ..`

`ln -s build.debug/hl-bump`

This will create a link in the root to the built library in the build.debug directory on unix-like systems.

You will need to provide a path to the latest hl-idl.  One is referenced in this projects.

`haxelib dev hl-idl ext/hl-idl` will set your hl-idl path to the right location.

You can now compile the sample.

`haxe compile.xhml`

and run it

`hl bin/main.hl`

It's only a few small tests at the moment, but it will verify that the extension was loaded correctly.


