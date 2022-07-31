package tests;

import bump.BumpAllocator;

class TestAllc {
    public var a = 0;
}

class Main {
    public static function main() {
        var x = new BumpAllocator(1024);

        trace ('Allocating');
        for (i in 0...10) {
            var y = x.alloc(TestAllc);

            y.a = i;    
            trace('${i}  ${y} ${y.a} capacity ${ x.availableBytes() } / ${x.totalBytes()}');
        }

        x.clear();
      
        var z = x.allocCompactArray(TestAllc, 10);

        for (i in 0...10) {
            var zz :TestAllc = z.getAsDynamic(i);
            zz.a = i;
            var yy = z[i];
            trace('${i}  ${zz} ${zz.a} vs ${yy.a}');
        }

        x.clear();
      
        var z = x.allocCompactArray(TestAllc, 10);

        for (i in 0...10) {
            var zz :TestAllc = z.getAsDynamic(i);
            var yy = z[i];
            trace('${i}  ${zz} ${zz.a} vs ${yy.a}');
        }
        x.clear();

        for (i in 0...50) {
            var y = x.alloc(TestAllc);

            y.a = i;    
            trace('${i}  ${y} ${y.a} capacity ${ x.availableBytes() } / ${x.totalBytes()}');
        }


        x.clear();
        x.dispose();
    }

}