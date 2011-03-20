project.name = "ReOrder Mods ++"

project.config["Debug"].bindir   = "../bin/debug"
project.config["Release"].bindir = "../bin/release"

package = newpackage()
package.name = "ReOrder Mods ++"
package.kind = "winexe"
package.language = "c++"
package.files = {
	matchrecursive("../include/*.h", "../src/*.cpp", "../res/*.*")
}


package.buildflags = {
	"no-main", 
	"no-pch" 
}

package.includepaths = {
	"../res",
	"../include" 
}

package.config["Debug"].objdir = "../temp/Debug"
package.config["Release"].objdir = "../temp/Release"
