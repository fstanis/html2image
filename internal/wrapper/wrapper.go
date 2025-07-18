package wrapper

/*
#cgo CXXFLAGS: -std=c++20
#cgo LDFLAGS: -lUltralight -lUltralightCore -lWebCore -lAppCore -Wl,-rpath,./ultralight

#include "wrapper.h"
*/
import "C"
import "unsafe"

type RenderResult struct {
	Bytes  []byte
	Width  int
	Height int
	BPP    int
}

type Renderer struct {
	handle unsafe.Pointer
}

func NewRenderer(width, height int) Renderer {
	return Renderer{C.newRenderer(C.uint(width), C.uint(height))}
}

func (r Renderer) Free() {
	unregisterLogger(r.handle)
	C.deleteRenderer(r.handle)
}

func (r Renderer) Render(html string) RenderResult {
	htmlStr := C.CString(html)
	defer C.free(unsafe.Pointer(htmlStr))
	res := C.render(r.handle, htmlStr)
	return RenderResult{
		Bytes:  C.GoBytes(res.bytes, C.int(res.size)),
		Width:  int(res.width),
		Height: int(res.height),
		BPP:    int(res.bpp),
	}
}

func (r Renderer) SetLogger(logger loggerFunc) {
	registerLogger(r.handle, logger)
}
