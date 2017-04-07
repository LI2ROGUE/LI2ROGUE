#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cgi.h"
#include "estado.h"

#define MAX_BUFFER		10240
#define TAM				10
#define ESCALA			48

int posicao_valida(int x, int y) {
	return x >= 0 && y >= 0 && x < TAM && y < TAM;
}

void imprime_casa(int x, int y) {
	char *cor[] = {"#666600", "#555500"};
	int idx = (x + y) % 2;
	QUADRADO(x, y,ESCALA, cor[idx]);
}
int posicao_igual(POSICAO p, int x, int y){
	return p.x == x && p.y == y;
}

int tem_jogador(ESTADO e, int x, int y){
	return posicao_igual(e.jog, x, y);
}

int tem_inimigo(ESTADO e, int x, int y){
	int i;
	for(i=0; i<e.num_inimigos; i++)
		if(posicao_igual(e.inimigo[i], x, y))
		return 1;
	return 0;
}

int tem_obstaculo(ESTADO e, int x, int y){
	int i;
	
	for(i=0; i<e.num_obstaculos; i++)
		if(posicao_igual(e.obstaculo[i], x, y))
		return 1;
	return 0;
}

int posicao_ocupada(ESTADO e, int x, int y){
	if (x==0 && y==0) return 0;
	else return tem_jogador(e, x, y) || tem_inimigo(e, x, y) || tem_obstaculo(e, x, y);
}

ESTADO inicializar_inimigo(ESTADO e){
	int x, y;
	do {
	x = random() % TAM;
	y = random() % TAM;
}	while(posicao_ocupada(e, x, y));
	if (x!=0 || y!=0){
	e.inimigo[(int)e.num_inimigos].x = x;
	e.inimigo[(int)e.num_inimigos].y = y;
	e.num_inimigos++;
	}
	return e;
}

ESTADO inicializar_inimigos(ESTADO e, int num){
	int i;
	for(i=0; i<num; i++)
		e = inicializar_inimigo(e);
	return e;
}

ESTADO inicializar_obstaculo(ESTADO e){
	int x, y;
	do {
	x = random() % TAM;
	y = random() % TAM;
}	while(posicao_ocupada(e, x, y));
	if (x!=0 || y!=0){
	e.obstaculo[(int)e.num_obstaculos].x = x;
	e.obstaculo[(int)e.num_obstaculos].y = y;
	e.num_obstaculos++;
	}
	return e;
}

ESTADO inicializar_obstaculos(ESTADO e, int num){
	int i;
	for(i=0; i<num; i++)
		e = inicializar_obstaculo(e);
	return e;
}

ESTADO inicializar_goal(ESTADO e){
	e.goal.x=0;
	e.goal.y=0;
	return e;
}



ESTADO inicializar() {
	ESTADO e = {{0}};
	int z,w;
	z=random () % 20;
	w= (random () % 30);
	if (z<10) z+=7;
	if (w<13) w+=7;
	e.jog.x = 5;
	e.jog.y = 9;
	e = inicializar_goal(e);	
	e = inicializar_inimigos(e, z);
	e = inicializar_obstaculos(e, w);
	return e;
}



void imprime_movimento(ESTADO e, int dx, int dy) {
	ESTADO novo = e;
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	char link[MAX_BUFFER];
	if(!posicao_valida(x, y))
		return;
	if(posicao_ocupada(e, x, y))
		return;

	novo.jog.x = x;
	novo.jog.y = y;
	sprintf(link, "http://localhost/cgi-bin/exemplo?%s", estado2str(novo));
	ABRIR_LINK(link);
	trans(x, y, ESCALA);
	FECHAR_LINK;
}

void imprime_movimentos(ESTADO e) {
	int dx, dy;
	for(dx=-1;dx<=1; dx++)
		for(dy=-1;dy<=1; dy++)
			imprime_movimento(e,  dx, dy);
}

void imprime_jogador(ESTADO e) {
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "knight.png");
	imprime_movimentos(e);
}

ESTADO ler_estado(char *args) {
	if(strlen(args) == 0)
		return inicializar();
	ESTADO e = str2estado(args);
	if (tem_jogador(e,0,0)) return inicializar();
	else return e;
}

void imprime_inimigos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_inimigos; i++)
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "werecreature_25.png");
}

void imprime_obstaculos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "lava_pool2.png");
}

void imprime_goal(ESTADO e){
	IMAGEM(e.goal.x, e.goal.y, ESCALA,"trapdoor1.png");
}

int main() {
	int x, y;
	srandom(time(NULL));
	ESTADO e = ler_estado(getenv("QUERY_STRING"));

	COMECAR_HTML;
	ABRIR_SVG(600, 600);
	for(y = 0; y < 10; y++)
		for(x = 0; x < 10; x++)
			imprime_casa(x, y);
	imprime_goal(e);
	imprime_inimigos(e);
	imprime_obstaculos(e);	
	imprime_jogador(e);

	FECHAR_SVG;

	return 0;
}
