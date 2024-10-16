package cdag

import (
	callocator "github.com/jurgen-kluft/callocator/package"
	denv "github.com/jurgen-kluft/ccode/denv"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'cdag'
func GetPackage() *denv.Package {
	// Dependencies
	cunittestpkg := cunittest.GetPackage()
	callocatorpkg := callocator.GetPackage()

	// The main (cdag) package
	mainpkg := denv.NewPackage("cdag")
	mainpkg.AddPackage(cunittestpkg)
	mainpkg.AddPackage(callocatorpkg)

	// 'cdag' library
	mainlib := denv.SetupDefaultCppLibProject("cdag", "github.com\\jurgen-kluft\\cdag")
	mainlib.Dependencies = append(mainlib.Dependencies, callocatorpkg.GetMainLib())

	// 'cdag' unittest project
	maintest := denv.SetupDefaultCppTestProject("cdag"+"_test", "github.com\\jurgen-kluft\\cdag")
	maintest.Dependencies = append(maintest.Dependencies, cunittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
