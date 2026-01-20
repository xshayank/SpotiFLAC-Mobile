package main

import "C"
import _ "github.com/zarz/spotiflac_android/go_backend/gobackend"

// main is required for buildmode=c-shared
func main() {}

//export HelloWorld
func HelloWorld() *C.char {
	return C.CString("Hello from SpotiFLAC Go Backend!")
}
