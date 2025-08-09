package wrapper

import (
	"sync"
)
import "C"

type loggerFunc func(byte, string)

type logEntry struct {
	level   byte
	message string
}

var logger loggerFunc
var logBuffer []logEntry
var loggerMutex sync.Mutex

func SetLogger(l loggerFunc) {
	loggerMutex.Lock()
	defer loggerMutex.Unlock()
	logger = l
	if logger == nil {
		return
	}
	if len(logBuffer) > 0 {
		for _, entry := range logBuffer {
			logger(entry.level, entry.message)
		}
		logBuffer = nil
	}
}

//export logMessage
func logMessage(level C.uchar, message *C.char) {
	loggerMutex.Lock()
	defer loggerMutex.Unlock()
	if logger != nil {
		logger(byte(level), C.GoString(message))
	} else {
		logBuffer = append(logBuffer, logEntry{byte(level), C.GoString(message)})
	}
}
