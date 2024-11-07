package main

import (
	"image"
	"image/color"
	"runtime"
	"sync"

	"C"

	"github.com/fstanis/html2image/wrapper"
)

// Renderer represents an HTML engine that can process requests to render HTML
// to images.
type Renderer struct {
	renderMutex   sync.Mutex
	renderChannel chan string
	resultChannel chan wrapper.RenderResult
}

// NewRenderer creates a new instance of a Renderer with the given viewport
// width and height and an optional logger.
func NewRenderer(width int, height int, logger wrapper.Logger) *Renderer {
	r := &Renderer{sync.Mutex{}, make(chan string), make(chan wrapper.RenderResult)}
	go r.start(width, height, logger)
	return r
}

func (r *Renderer) start(width int, height int, logger wrapper.Logger) {
	runtime.LockOSThread()
	defer runtime.UnlockOSThread()
	renderer := wrapper.NewRenderer(width, height)
	defer renderer.Free()
	if logger != nil {
		renderer.SetLogger(logger)
	}
	for html := range r.renderChannel {
		r.resultChannel <- renderer.Render(html)
	}
}

// Free releases the resources used by the Renderer.
func (r *Renderer) Free() {
	close(r.renderChannel)
}

// Render renders the given HTML string to an image.
func (r *Renderer) Render(html string) image.Image {
	r.renderMutex.Lock()
	defer r.renderMutex.Unlock()
	r.renderChannel <- html
	res := <-r.resultChannel
	img := image.NewRGBA(image.Rect(0, 0, res.Width, res.Height))
	for y := 0; y < res.Height; y++ {
		for x := 0; x < res.Width; x++ {
			pos := (y*res.Width + x) * res.BPP
			img.Set(x, y, color.RGBA{
				R: res.Bytes[pos+2],
				G: res.Bytes[pos+1],
				B: res.Bytes[pos],
				A: res.Bytes[pos+3],
			})
		}
	}
	return img
}
