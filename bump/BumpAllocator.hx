package bump;
//import haxe.macro.Tools.TExprTools;
import hl.BaseType.BaseType;


@:forward.new
abstract BumpAllocator(Native.BumpAllocatorInternal) {

   inline public function alloc<T>( cl : Class<T> ) : T {
		return cast this.alloc( (cast cl:BaseType).__type__  );
	}

    // Behaves like a regular array
    inline public function allocArray<T>( cl : Class<T>,  count : Int) : BumpArray<T> {
		return cast this.allocArray( (cast cl:BaseType).__type__, count  );
	}

    // Objects and structs will be allocated directly, not a ref or a pointer
    // Won't currently support non-objects or structs
    inline public function allocCompactArray<T>( cl : Class<T>,  count : Int) : BumpCompactArray<T> {
		return cast this.allocCompactArray( (cast cl:BaseType).__type__, count  );
	}


    inline public function  clear() {
        this.clear();
    }

    inline public function dispose() {
        this.dispose();
    }

    inline public function totalBytes() {
        return this.totalBytes();
    }
    inline public function availableBytes() {
        return this.availableBytes();
    }
    inline public function allocatedBytes() {
        return this.allocatedBytes();
    }

    inline public function watermark() {
        return this.allocatedBytes();
    }

    inline public function drainToWatermark(wm : Int) {
        this.drainTo(wm);
    }

}
