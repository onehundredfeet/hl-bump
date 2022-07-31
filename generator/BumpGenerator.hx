
package ;

#if eval
class BumpGenerator {

	// Put any necessary includes in this string and they will be added to the generated files
	static var INCLUDE = "
#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif


#include <hl.h>
#include \"hl-bump.h\"
";
	
	public static function generateCpp() {	
		var options = { idlFile : "bump/bump.idl", nativeLib : "bump", outputDir : "src", includeCode : INCLUDE, autoGC : true };
		webidl.Generate.generateCpp(options);
	}

}
#end