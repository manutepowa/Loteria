
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "cabecera.h"


#define WRITE 1
#define READ 0


int ContadorHijos=0;

int main(int argc, char const *argv[])
{

	if (argc != 3)
	{
		exit(-1);
	}
	
	//Numero de jugadores 
	int Jugadores = atoi(argv[2]);
	int sorteo = atoi(argv[1]);

	
	int *ApuestaPadre = (int*)malloc(6*sizeof(int));
	HIJO *hijo;
	int k,i;
	int PremioDevuelto = 0;



	hijo = ReservarStructHijo(Jugadores);

	pid_t pid;
	 

	//Reservar memoria para los pipes
	int **PIPE=(int**)malloc(Jugadores * sizeof(int*));
	for (i = 0; i < Jugadores; i++)
	{
		PIPE[i] = (int*)malloc(2 * sizeof(int));
	}

	for (i = 0; i < Jugadores; i++)
	{
		if(pipe(PIPE[i]) == -1)
		{
			printf("ERROR en el PIPE %d\n",i );
			exit(0);
		}
	}


	//--------------------------------
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGRTMIN,&sa,NULL);
	
	for(k=0;k<Jugadores;k++)
	{

		pid = fork();
					

		
		if(pid == 0)
		{

			int premiohijo;
			int *ApuestaHIJO = (int*)malloc(6 * sizeof(int));
			GenerarApuesta(ApuestaHIJO);
			OrdenarApuesta(ApuestaHIJO);

 
			char *ApuestaPH = (char*)malloc(100 * sizeof(char));
			//printf ("Soy el hijo: %d  con PID %d y mi padre es PPID = %d\n",hijo[k].num,getpid (), getppid ());

			v.sival_int = getpid(); //asignamos valor al dato
			sigqueue(getppid(),SIGRTMIN,v); //enviamos la señal de tiempo real 
			
			
			
			ApuestaPH = LeerPIPE(PIPE[k]);
			premiohijo = ComprobarResultados(ApuestaHIJO,ApuestaPH);
			/*printf("-------------------------------------------------\n");
			Mostrar(ApuestaHIJO);
			printf("El hijo %d con pid = %d ha Acertado %d\n",k+1,getpid(),premiohijo );
			printf("-------------------------------------------------\n");
			*/
			exit(premiohijo);
		}
		else if(pid < 0)
		{
			printf ("ERRORR\n");
			exit(1);
		}
		else
		{
			hijo[k].num = k+1;
			hijo[k].pid = pid;
		}
	}


	while(ContadorHijos < Jugadores)
	{
		pause();
	}
	
	GenerarApuesta(ApuestaPadre);
	OrdenarApuesta(ApuestaPadre);

	char *ApuestaPadreChar = (char*)malloc(20 * sizeof(char));

	
		
	Array_a_char(ApuestaPadreChar,ApuestaPadre,6);

	EscribirPIPE(PIPE, ApuestaPadreChar, Jugadores);
	
	for (i = 0; i < Jugadores; ++i)
	{
		pid = wait(&PremioDevuelto);
		RegistrarPremios(hijo,pid,PremioDevuelto/256,Jugadores);
	}


	GenerarFicheros(Jugadores,sorteo,hijo);
	
	return 0;
}

