package main

import (
	"fmt"
	"github.com/gorilla/mux"
	"github.com/jacobsa/go-serial/serial"
	"io"
	"log"
	"net/http"
)

var port io.ReadWriteCloser

func main() {
	options := serial.OpenOptions{
		PortName:        "/dev/tty.usbserial-A8008HlV",
		BaudRate:        19200,
		DataBits:        8,
		StopBits:        1,
		MinimumReadSize: 4,
	}

	// Open the port.
	port, err := serial.Open(options)
	if err != nil {
		log.Fatalf("serial.Open: %v", err)
	}

	// Make sure to close it later.
	defer port.Close()
	// Initialise the router
	r := mux.NewRouter()

	// Unauthenticated endpoints
	r.PathPrefix("/").Handler(http.StripPrefix("/", http.FileServer(http.Dir("./dist"))))
	r.HandleFunc("/on", toggleOn).Methods("POST")
	r.HandleFunc("/off", toggleOff).Methods("POST")

	r.Use(corsMiddleware)

	fmt.Printf("All setup running, and available on port 8000")
	log.Fatal(http.ListenAndServe(":8000", r))
}

func corsMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE")
		w.Header().Set("Access-Control-Allow-Headers", "Origin, Content-Type, Authorization")

		if r.Method == "OPTIONS" {
			w.WriteHeader(200)
			return
		}

		next.ServeHTTP(w, r)
	})
}

func toggleOn(w http.ResponseWriter, r *http.Request) {
	// Write 4 bytes to the port.
	b := []byte{0x30, 0x0a}
	n, err := port.Write(b)
	if err != nil {
		log.Fatalf("port.Write: %v", err)
	}

	fmt.Println("Wrote", n, "bytes. Toggled on.")
}

func toggleOff(w http.ResponseWriter, r *http.Request) {
	// Write 4 bytes to the port.
	b := []byte{0x31, 0x0a}
	n, err := port.Write(b)
	if err != nil {
		log.Fatalf("port.Write: %v", err)
	}

	fmt.Println("Wrote", n, "bytes. Toggled off.")
}
