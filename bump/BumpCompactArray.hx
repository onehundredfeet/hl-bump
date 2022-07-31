package bump;

abstract BumpCompactArray<T>(webidl.Types.VoidPtr) {
	public inline function getAsDynamic( index : Int ) : Dynamic return Native.BumpCompactArrayInternal.getAsDynamic( this, index);
	@:arrayAccess public inline function get( index : Int ) : T return cast Native.BumpCompactArrayInternal.getAsDynamic( this, index);
}