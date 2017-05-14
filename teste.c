#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER		10240

typedef struct posicao {
	char x;
	char y;
} POSICAO;

typedef struct estado {
	char pagina;
	POSICAO jog;
	char num_inimigos;
	char num_obstaculos;
	char num_arqueiros;
	POSICAO inimigo[10];
	POSICAO obstaculo[10];
	POSICAO arqueiro[10];
	POSICAO goal;
	char vida;
	char nivel;
	char score;
	char clas [10];
} ESTADO;

char *estado2str(ESTADO e)
{
	static char buffer[MAX_BUFFER];
	char *p = (char *) &e;
	unsigned int i;

	buffer[0] = 0;

	for(i = 0; i < sizeof(ESTADO); i++)
		sprintf(buffer, "%s%02x", buffer, p[i]);
	
	return buffer;
}

ESTADO str2estado(char *argumentos)
{
	ESTADO e;
	char *p = (char *) &e;
	unsigned int i;

	for(i = 0; i < sizeof(ESTADO); i++, argumentos += 2)
	{
		unsigned int d;
		sscanf(argumentos, "%2x", &d);
		p[i] = (char) d;
	}

	return e;
}

int main()
{
	ESTADO e = {0};
	char *t;
	int i;		
	t = estado2str(e);
	FILE *fp;
	fp = fopen("/home/osboxes/TESTE/teste_save_file/jogo","r");
	fscanf(fp, "%s", t);
	fclose(fp);
	e = str2estado(t);
	printf("%d %d\n", e.jog.x, e.jog.y);
	return 0;
}
