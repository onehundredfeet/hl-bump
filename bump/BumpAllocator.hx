package bump;
//import haxe.macro.Tools.TExprTools;
import hl.BaseType.BaseType;


@:forward.new
abstract BumpAllocator(Native.BumpAllocatorInternal) {
//    var _allocator : Native.BumpAllocator;

   inline public function alloc<T>( cl : Class<T> ) : T {
		return cast this.alloc( (cast cl:BaseType).__type__  );
	}

    inline public function allocArray<T>( cl : Class<T>,  count : Int) : BumpArray<T> {
		return cast this.allocArray( (cast cl:BaseType).__type__, count  );
	}

    inline public function allocCompactArray<T>( cl : Class<T>,  count : Int) : BumpArray<T> {
		return cast this.allocCompactArray( (cast cl:BaseType).__type__, count  );
	}


    inline public function  clear() {
        this.clear();
    }

    inline public function dispose() {
        this.dispose();
    }
}
