package main

import "C"

// Import gobackend package for side effects (initialization and exported functions).
// This allows the gobackend package functions to be available when building as a shared library.
import _ "github.com/zarz/spotiflac_android/go_backend/gobackend"

// main is required for buildmode=c-shared
func main() {}

//export HelloWorld
// HelloWorld is a simple demonstration function for C shared library exports.
// This serves as a test to verify that the DLL can export C-compatible functions.
func HelloWorld() *C.char {
	return C.CString("Hello from SpotiFLAC Go Backend!")
}
