usbiss: main.o usbiss.o
	g++ -o usbiss main.o usbiss.o

main.o: main.cpp usbiss.h
	g++ -c main.cpp

usbiss.o: usbiss.cpp usbiss.h
	g++ -c usbiss.cpp

clean:
	rm -f *.o usbiss
