package wrapper

import (
	"unsafe"
)
import "C"

type LogLevel byte

type Logger interface {
	Log(level LogLevel, message string)
}

const (
	LevelError LogLevel = iota
	LevelWarning
	LevelInfo
)

var loggerMap = make(map[unsafe.Pointer]Logger)
var logBuffer = make(map[unsafe.Pointer][]string)

func registerLogger(app unsafe.Pointer, logger Logger) {
	loggerMap[app] = logger
	if len(logBuffer[app]) > 0 {
		for _, message := range logBuffer[app] {
			logger.Log(LevelInfo, message)
		}
		delete(logBuffer, app)
	}
}

func unregisterLogger(app unsafe.Pointer) {
	delete(loggerMap, app)
}

//export logMessage
func logMessage(app unsafe.Pointer, level C.uchar, message *C.char) {
	if logger, ok := loggerMap[app]; ok {
		logger.Log(LogLevel(level), C.GoString(message))
	} else {
		logBuffer[app] = append(logBuffer[app], C.GoString(message))
	}
}
