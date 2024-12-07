package wrapper

import (
	"unsafe"
)
import "C"

type loggerFunc func(byte, string)

type logEntry struct {
	level   byte
	message string
}

var loggerMap = make(map[unsafe.Pointer]loggerFunc)
var logBuffer = make(map[unsafe.Pointer][]logEntry)

func registerLogger(app unsafe.Pointer, logger loggerFunc) {
	loggerMap[app] = logger
	if len(logBuffer[app]) > 0 {
		for _, entry := range logBuffer[app] {
			logger(entry.level, entry.message)
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
		logger(byte(level), C.GoString(message))
	} else {
		logBuffer[app] = append(logBuffer[app], logEntry{byte(level), C.GoString(message)})
	}
}
