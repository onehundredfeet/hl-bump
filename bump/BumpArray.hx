package bump;

abstract BumpArray<T>(webidl.Types.VoidPtr) {
	@:arrayAccess inline function get( index : Int ) : T return Native.BumpArrayInternal.getAsDynamic( this, index);

}