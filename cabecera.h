#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>




 

struct sigaction sa;

union sigval v;

typedef struct 
{
	int pid;		//PID del hijo
	int num;		//Numero de hijo creado
	long premio;	//premio del hijo de un sorteo
} HIJO;

 
int ComprobarRepeticiones(int n, int *v); //Comprueba que no se generen nº iguales

void GenerarApuesta(int *V); // Genera la apuesta de nº aleatorios

void OrdenarApuesta(int *V); //Ordena un vector

void handler (int signo, siginfo_t *info, void *other); //Es el manejador

void Mostrar(int *V); //Muestra una apuesta que se le pasa

char* LeerPIPE(int *RW); //Lee del pipe 

HIJO* ReservarStructHijo(int Jugadores); //Reserva memoria para un vector de estructuras

void EscribirPIPE(int **RW, char *ApuestaPadreChar, int Jugadores); //Escribir en el pipe

char *Array_a_char(char *str, int *array,int n); //Transforma de entero a char
int ComprobarResultados(int *ApuestaHIJO,char *ApuestaPH); //comprueba el nº de aciertos 

void GenerarFicheros(int Jugadores,int sorteo,HIJO *h); //Genera los SnR

int Paso_A_Euros(int p); //Comprueba los aciertos y los pasa a Euros.

void RegistrarPremios(HIJO *h,int pid,int premio,int j); //Comprueba a que hijo corresponde el premio
