package main

import (
	"errors"
	"flag"
	"fmt"
	"image/png"
	"io"
	"log"
	"os"
	"strings"

	"github.com/fstanis/html2image"
)

var (
	outputFlag   = flag.String("output", "", "output filename")
	widthFlag    = flag.Int("width", 800, "help message for flag n")
	heightFlag   = flag.Int("height", 600, "help message for flag n")
	logLevelFlag = flag.String("loglevel", "error", "minimum log level to display")
)

type logger struct {
	minLevel html2image.LogLevel
}

func (l logger) Log(level html2image.LogLevel, message string) {
	if level > l.minLevel {
		return
	}
	var levelStr string
	switch level {
	case html2image.LevelInfo:
		levelStr = "[I] "
	case html2image.LevelWarning:
		levelStr = "[W] "
	case html2image.LevelError:
		levelStr = "[E] "
	}
	log.Println(levelStr + message)
}

func newLogger(minLevelStr string) logger {
	minLevel := html2image.LogLevel(3)
	switch strings.ToLower(minLevelStr) {
	case "err":
		fallthrough
	case "error":
		minLevel = html2image.LevelError
	case "warn":
		fallthrough
	case "warning":
		minLevel = html2image.LevelWarning
	case "info":
		minLevel = html2image.LevelInfo
	}
	return logger{minLevel}
}

func render(output string, width int, height int, lggr logger) error {
	if output == "" {
		return errors.New("no output file specified")
	}
	renderer := html2image.NewRenderer(width, height, lggr)
	defer renderer.Free()
	stdin, err := io.ReadAll(os.Stdin)
	if err != nil {
		return fmt.Errorf("failed to read from stdin: %v", err)
	}
	img := renderer.Render(string(stdin))
	f, err := os.Create(output)
	if err != nil {
		return fmt.Errorf("failed to create output file: %v", err)
	}
	defer f.Close()
	if err := png.Encode(f, img); err != nil {
		return fmt.Errorf("failed to encode PNG file: %v", err)
	}
	return nil
}

func main() {
	flag.Parse()
	if err := render(*outputFlag, *widthFlag, *heightFlag, newLogger(*logLevelFlag)); err != nil {
		log.Fatal(err)
	}
}
