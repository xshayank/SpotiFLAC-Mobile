package main

import "C"

// main is required for buildmode=c-shared
func main() {}

//export HelloWorld
func HelloWorld() *C.char {
	return C.CString("Hello from SpotiFLAC Go Backend!")
}
