package tests;

import bump.BumpAllocator;

class TestAllc {
    public var a = 0;
}

class Main {
    public static function main() {
        var x = new BumpAllocator(1024);

        trace ('Allocating');
        var y = x.alloc(TestAllc);

        y.a = 3;

        trace('y ${y} ${y.a}');

        var z = x.allocCompactArray(TestAllc, 10);


        var zz = z.getAsDynamic(3);

        zz.a = 5;

        trace('zz ${zz} ${zz.a} ');
        x.clear();
    }

}