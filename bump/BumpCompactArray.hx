package bump;

abstract BumpCompactArray<T>(webidl.Types.VoidPtr) {
	public inline function getAsDynamic( index : Int ) : T return Native.BumpCompactArrayInternal.getAsDynamic( this, index);
}