package bump;


typedef Native = haxe.macro.MacroType<[webidl.Module.build({ idlFile : "bump/bump.idl", chopPrefix : "rc", autoGC : true, nativeLib : "bump" })]>;
