#include "cabecera.h"

#define WRITE 1
#define READ 0
extern int ContadorHijos; 
//Manejador
void handler(int signo, siginfo_t *info, void *other ) 
{	
	char *src;
	switch(info->si_code) {
		case SI_QUEUE: 
			src = "sigqueue";
		  	ContadorHijos++;
			
			//printf("%d\n", ContadorHijos);
			break;

		case SI_TIMER: 
			src = "timer"; 
			break;
		
		case SI_ASYNCIO: 
			src = "asyncio"; 
			break;
		
		case SI_MESGQ: 
			src = "msg queue"; 
			break;
		
		case SI_USER: 
			src = "kill/abort/raise"; 
			break;
		
		default: 
			src = "unknown!"; 
			break;
	}
	//printf("\tSignal %d: source = %s, data = %d\n",signo, src, info->si_value.sival_int);
}



int ComprobarRepeticiones(int n, int *v)
{
	int i =0;
	for (i = 0; i < 6; i++)
	{
		if (n == v[i])
			return 1;
	}
	return 0;
}


void GenerarApuesta(int *V)
{
	int i, apuesta;
	
	srand(getpid()+126579);

	for (i = 0; i < 6; i++)
	{
		do{

			apuesta = 1 + rand() % 15;

		  }while(ComprobarRepeticiones(apuesta, V) == 1);
		
		V[i] = apuesta;
	}
}

//Ordenar el vector por el metodo de la burbuja.
void OrdenarApuesta(int *V)
{
	int i, j,aux = 0;

	for (i = 0; i < 6; i++)
	{
		for (j = i + 1; j < 6; j++)
		{
			if (V[i] > V[j])
			{
				aux = V[j];
				V[j] = V[i];
				V[i] = aux;
			}
		}
	}
}


void Mostrar(int *V)
{ 
	int i;

	for (i = 0; i < 6; i++)
	{
		printf("%d - ",V[i] );

	}
}


HIJO* ReservarStructHijo(int Jugadores)
{
	HIJO *h;

	h = (HIJO*)malloc(Jugadores * sizeof(HIJO));
}

char* LeerPIPE(int *RW)
{
	char *ss = (char*)malloc(20 * sizeof(char));
	
	close(RW[WRITE]);
	
	read(RW[READ],ss, 20);

	close(RW[READ]);

	return ss;
}

void EscribirPIPE(int **RW, char *ApuestaPadreChar, int Jugadores)
{
	int i;

	for (i = 0; i < Jugadores; i++)
	{
		close(RW[i][READ]);

		write(RW[i][WRITE],ApuestaPadreChar, strlen(ApuestaPadreChar)+1);
		//close(RW[i][WRITE]);
	}

	
	for (i = 0; i < Jugadores; i++)
	{
		close(RW[i][WRITE]);
	}
}

//Funcion recursiva
char *Array_a_char(char *str, int *array,int n) 
{
	int r;
  	if (n == 0) return 0;
  	if (n == 1) 
  		r = sprintf(str, "%d", array[0]);
  	else        
  		r = sprintf(str, "%d#", array[0]);
  
  	Array_a_char(str + r, array + 1, n - 1); 
  return str;
}

int ComprobarResultados(int *ApuestaHIJO,char *ApuestaPH)
{
	int i,j;
	int p=0;
	
	char *Num;

	Num = strtok(ApuestaPH,"#");

	while((Num = strtok(NULL,"#")) != NULL)
	{
		j = atoi(Num);
		for (i = 0; i < 6; ++i)
		{
			if (j == ApuestaHIJO[i])
			{
				p++;
			}
		}
	}
	return p;	
}

void GenerarFicheros(int Jugadores,int sorteo,HIJO *h)
{
	int i;
	FILE *f;
	char *Nombre = (char*)malloc(4*sizeof(char));
	sprintf(Nombre,"S%dR",sorteo);
	f = fopen(Nombre, "w");

	sprintf(Nombre,"#Resultados Sorteo %d\n",sorteo);
	fputs(Nombre,f);

	for (i = 0; i < Jugadores; ++i)
	{
		sprintf(Nombre,"%ld\n",h[i].premio);
		fputs(Nombre,f);
	}

	fclose(f);
	
}

int Paso_A_Euros(int p)
{
	switch(p)
	{
		case 3:
			return 10;
			break;
		case 4:
			return 50;
			break;
		case 5:
			return 500;
			break;
		case 6:
			return 10000;
			break;
		default:
			return 0;
	}
}


void RegistrarPremios(HIJO *h,int pid,int premio,int j)
{
	int i;

	premio = Paso_A_Euros(premio);

	for (i = 0; i < j; ++i)
	{
		if(h[i].pid == pid)
		{
			h[i].premio = premio;
		}
	}
}
