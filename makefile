CC=gcc
CFLAG=-I.


sorteo:sorteo.o cabecera.o
	$(CC) -o sorteo sorteo.o cabecera.o $(CFLAG)
sorteo.o:sorteo.c cabecera.h
	$(CC) -c -o sorteo.o sorteo.c $(CFLAG)
cabecera.o:cabecera.c cabecera.h
	$(CC) -c -o cabecera.o cabecera.c $(CFLAG)
clean:
	rm sorteo *.o
