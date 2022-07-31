package tests;

import bump.BumpAllocator;

class TestAllc {
    public var a = 0;
}

class Main {
    public static function main() {
        var x = new BumpAllocator(1024);

        var y = x.alloc(TestAllc);
        var z = x.allocArray(TestAllc, 10);

        x.clear();
    }

}