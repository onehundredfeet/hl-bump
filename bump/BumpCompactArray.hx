package bump;

abstract BumpCompactArray<T>(webidl.Types.VoidPtr) {
	@:arrayAccess inline function get( index : Int ) : T return Native.BumpCompactArrayInternal.getAsDynamic( this, index);
}