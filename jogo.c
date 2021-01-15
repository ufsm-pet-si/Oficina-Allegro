#include "jogo.h"

#define X1_TAB 180
#define Y1_TAB 20
#define X2_TAB 580
#define Y2_TAB 560

#define LARGURA 600
#define ALTURA 600

// COMPILAÇÂO
//gcc -Wall -o main jogo.c -lallegro_font -lallegro_color -lallegro_ttf -lallegro_primitives -lallegro

#define tela 0
#define tab 1
#define branco 2
#define vazio 3
#define peca1 4
#define peca2 5
#define peca3 6
#define preto 7


#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int main() {
	jogo jog;

	inicializa_mundo(&jog);
	inicio_jogo(&jog);
  bool termina_jogada = false;
  while(!derrota(&jog)) {
    jog.prox_peca = proxima_peca();
    termina_jogada = false;
    while(termina_jogada == false) {
      jog.lin = 0;
      jog.col = 0;
      desenha_mundo(&jog);
      coloca_peca(jog.prox_peca);
      processa_entradas(&jog);
      termina_jogada = atualiza_mat(&jog);
    }
  }
  pontuacao(&jog);

  ranking(&jog);

  tela_final(&jog);
  
  sleep(10);

  fim();
}

void inicializa_mundo(jogo* jog) {
	preencher_mat(jog);
	jog->lin = 0;
	jog->col = 0;
	jog->pontos = 0;
	for(int i = 0; i < 10; i++){
		jog->ranking[i] = 0;
	}

	//funções gerais
	al_init();
	al_install_mouse();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	inicializa_janela(LARGURA, ALTURA, "Threes");
	inicializa_teclado();
	inicializa_cores();
	inicializa_fontes();
}

void preencher_mat(jogo* jog) {
	int i, j;

	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			jog->mat[i][j] = 0;
		}
	}
}

void inicializa_janela(float larg, float alt, char txt[]) {
	ALLEGRO_DISPLAY* janela;
	janela = al_create_display(larg, alt);

	al_hide_mouse_cursor(janela);
	al_set_window_title(janela, txt);
}

ALLEGRO_EVENT_QUEUE* fila_eventos;
void inicializa_teclado() {
	al_install_keyboard();

	fila_eventos = al_create_event_queue();
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
}

#define NCORES 10
ALLEGRO_COLOR cores[NCORES];
void inicializa_cores() {
	cores[tela] = al_map_rgb(178, 255, 229);
	cores[tab] = al_map_rgb(208, 230, 223);
	cores[vazio] = al_map_rgb(182, 217, 217);
	cores[peca1] = al_map_rgb(103, 203, 252);
	cores[peca2] = al_map_rgb(240, 99, 127);
	cores[peca3] = al_map_rgb(255, 255, 255);
	cores[preto] = al_map_rgb(0, 0, 0);
}

ALLEGRO_FONT* fonteJogo;
ALLEGRO_FONT* fonteRanking;
void inicializa_fontes() {
	fonteJogo = al_load_font("fontes/open-sans.ttf", 26, 0);
	fonteRanking = al_load_font("fontes/arcadeclassic.ttf", 26, 0);
}

void inicio_jogo(jogo* jog) {
	int prob, posicao_lin, posicao_col;
	int i;
	srand(time(NULL));
	for(i = 0; i < 9; i++) {
		prob = sorteio(100);
		do{
			posicao_lin = sorteio(4);
			posicao_col = sorteio(4);
			} while(jog->mat[posicao_lin][posicao_col] != 0);
			jog->mat[posicao_lin][posicao_col] = escolhe_peca_inicio(prob);
	}
}

int sorteio(int quant) {
	return rand() % quant;
}

int escolhe_peca_inicio(int prob) {
	int peca;

	if(prob >= 0 && prob < 35) peca = 1;
	else if(prob >= 35 && prob < 70) peca = 2;
	else if(prob >= 70 && prob < 98) peca = 3;
	else peca = 6;

	return peca;
}

int proxima_peca(){
	int prob, peca;

	prob = sorteio(50);
	if(prob >= 47) peca = escolhe_peca_maior();
	else peca = escolhe_peca_menor(prob);

	return peca;
}

int escolhe_peca_maior() {
	int prob, peca;
	prob = sorteio(20);

	if(prob >= 0 && prob < 10) peca = 6;
	else if(prob >= 10 && prob < 16) peca = 12;
	else if(prob >= 16 && prob < 19) peca = 24;
	else peca = 48;

	return peca;
}

int escolhe_peca_menor(int prob) {
	int peca;

	if(prob >= 0 && prob < 16) peca = 1;
	else if(prob >= 16 && prob < 32) peca = 2;
	else peca = 3;

	return peca;
}

void desenha_mundo(jogo* jog) {
	desenho_inicial(jog);

	char s[7];

	int cor_txt;

	int cor_peca;

	float posicaoX, posicaoY;

	int x1e, y1e = Y1_TAB, x2e, y2e = Y1_TAB;
	for(int i = 0; i < 4; i++){
		x1e = X1_TAB + 20;
		x2e = x1e + 75;
		y1e = y2e + 20;
		y2e = y1e + 110;
		for(int j = 0; j < 4; j++){

			if(jog->mat[i][j] != 0) {
				cor_peca = escolhe_cor(jog->mat[i][j]);
				retangulo(x1e, y1e, x2e, y2e, 1, cor_peca, cor_peca);

				sprintf(s, "%d", jog->mat[i][j]);
				cor_txt = cor_texto(jog->mat[i][j]);
				posicaoX = (x1e + x2e)/2;
				posicaoY = (y1e + y2e)/2;

				al_draw_text(fonteJogo, cores[cor_txt], posicaoX, posicaoY - 26/2, ALLEGRO_ALIGN_CENTRE, s);
			}
			x1e = x2e + 20;
			x2e = x1e + 75;
		}
	}
	
}

int escolhe_cor(int n) {
	int cor;
	switch (n) {
	case 1:
		cor = peca1;
		break;
	case 2:
		cor = peca2;
		break;
	default:
		cor = peca3;
		break;
	}

	return cor;
}

int cor_texto(int n) {
	int cor;
	if(n == 1 || n == 2){
		cor = peca3;
	} else {
		cor = preto;
	}

	return cor;
}
 
void desenho_inicial(jogo* jog) {
	inicia_desenho();

	//tela
	retangulo(0, 0, 600, 600, 1, tela, tela);

	//tabuleiro
	retangulo(X1_TAB, Y1_TAB, X2_TAB, Y2_TAB, 1, tab, tab);

	//próxima peça
	retangulo(40, 50, 140, 190, 1, tab, tab);

	int x1e, y1e = Y1_TAB, x2e, y2e = Y1_TAB;
	for(int i = 0; i < 4; i++){
		x1e = X1_TAB + 20;
		x2e = x1e + 75;
		y1e = y2e + 20;
		y2e = y1e + 110;
		for(int j = 0; j < 4; j++){
			retangulo(x1e, y1e, x2e, y2e, 1, vazio, vazio);
			x1e = x2e + 20;
			x2e = x1e + 75;
		}
	}
}

void coloca_peca(int n) {
	int cor_pec = escolhe_cor(n);
	retangulo(56, 66, 124, 174, 1, cor_pec, cor_pec);
	termina_desenho();
}

void inicia_desenho(){
	al_clear_to_color(cores[branco]);
}

void retangulo (float x1, float y1, float x2, float y2, float i, int cor, int cor2) {
	al_draw_filled_rectangle(x1, y1, x2, y2, cores[cor2]);
	al_draw_rectangle(x1, y1, x2, y2, cores[cor], i);
}

void termina_desenho() {
	al_flip_display();
}

bool atualiza_mat(jogo *jog)
{
  int contC = 0, contL[4]= {0, 0, 0 , 0}, maiorL = 0, maiorC = 0, lin = 0, col = 0;
  bool deu = false;
  if(jog->lin == -1 || jog->col == -1) {    
    lin = 3;
    col = 3;
    for(int i = 0; i < 4; i++) {
      contC = 0;
      for(int j = 0; j < 4; j++) {
        if(jog->mat[i][j] != 0) {
          if((i+jog->lin) >=0 && (j+jog->col) >= 0) {
            if(jog->mat[i+jog->lin][j+jog->col] == 0) {
              jog->mat[i+jog->lin][j+jog->col] = jog->mat[i][j];
              jog->mat[i][j] = 0;
              deu = true;
              contC++;
              contL[j]++;      
            } else if(((jog->mat[i][j] == jog->mat[(i+jog->lin)][(j+jog->col)]) || ((jog->mat[i][j] + jog->mat[(i+jog->lin)][(j+jog->col)]) == 3)) && 
                     (((jog->mat[i][j] + jog->mat[i+jog->lin][j+jog->col]) % 3) == 0)) {
              if(((i+jog->lin) == 0 || (j+jog->col) == 0) || (jog->mat[i+jog->lin+jog->lin][j+jog->col+jog->col] != 0)) {
                jog->mat[(i+jog->lin)][(j+jog->col)] += jog->mat[i][j];
                jog->mat[i][j] = 0;
                contC++;
                contL[j]++;
                deu = true;
              }
            }
          }
        }
        if((jog->lin == -1 && contL[j] > maiorL)) {
          maiorL = contL[j];
          col = j;
        }
      }
      if((jog->col == -1 && contC > maiorC)) {
        maiorC = contC;
        lin = i;
      }
    }
  } else if(jog->lin == 1 || jog->col == 1) {
    for(int i = 3; i >=0 ; i--) {
      contC = 0;
      for(int j = 3; j >=0; j--) {
        if(jog->mat[i][j] != 0) {
          if((i+jog->lin) <= 3 && (j+jog->col) <= 3) {
            if(jog->mat[i+jog->lin][j+jog->col] == 0) {
              jog->mat[i+jog->lin][j+jog->col] = jog->mat[i][j];
              jog->mat[i][j] = 0;
              deu = true;
              contC++;
              contL[j]++; 
            } else if(((jog->mat[i][j] == jog->mat[(i+jog->lin)][(j+jog->col)]) || ((jog->mat[i][j] + jog->mat[(i+jog->lin)][(j+jog->col)]) == 3)) && 
                     (((jog->mat[i][j] + jog->mat[i+jog->lin][j+jog->col]) % 3) == 0)) {
              if(((i+jog->lin) == 3 || (j+jog->col) == 3) || (jog->mat[i+jog->lin+jog->lin][j+jog->col+jog->col] != 0)) {
                jog->mat[(i+jog->lin)][(j+jog->col)] += jog->mat[i][j];
                jog->mat[i][j] = 0;
                deu = true;
                contC++;
                contL[j]++;
              }
            }
          }
        }
        if((jog->lin == 1 && contL[j] > maiorL)) {
          maiorL = contL[j];
          col = j;
        }
      }
      if((jog->col == 1 && contC > maiorC)) {
        maiorC = contC;
        lin = i;
      }
    }
  }
  if(deu == true) {
    jog->mat[lin][col] = jog->prox_peca;
  }
  return deu;
}

bool derrota(jogo *jog)
{
  bool perdeu = false;
  if(!condicaoWA(jog->mat,-1, 0) && !condicaoWA(jog->mat, 0,-1) && !condicaoSD(jog->mat, 1,0) && !condicaoSD(jog->mat, 0,1)) {
    perdeu = true;
  }
  return perdeu;
}

//Condição de derrota para quando são dados os comandos 'w' ou 'a'
bool condicaoWA(int mat[4][4], int lin, int col)
{
  bool flag = false;
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(mat[i][j] != 0) {
        if((i+lin) >=0 && (j+col) >= 0) {
          if(mat[i+lin][j+col] == 0) {
            flag = true;               
          } else if(((mat[i][j] == mat[(i+lin)][(j+col)]) || ((mat[i][j] + mat[(i+lin)][(j+col)]) == 3)) && (((mat[i][j] + mat[i+lin][j+col]) % 3) == 0)) {
            if(((i+lin) == 0 || (j+col) == 0) || (mat[i+lin+lin][j+col+col] != 0)) {
              flag = true;
            }
          }
        }
      }
    }
  }
  return flag;
}

//Condição de derrota para quando são dados os comandos 's' ou 'd'
bool condicaoSD(int mat[4][4], int lin, int col)
{
  bool flag = false;
  for(int i = 3; i >=0 ; i--) {
    for(int j = 3; j >=0; j--) {
      if(mat[i][j] != 0) {
        if((i+lin) < 4 && (j+col) < 4) {
          if(mat[i+lin][j+col] == 0) {
            flag = true;               
          } else if(((mat[i][j] == mat[(i+lin)][(j+col)]) || ((mat[i][j] + mat[(i+lin)][(j+col)]) == 3)) && (((mat[i][j] + mat[i+lin][j+col]) % 3) == 0)) {
            if(((i+lin) == 3 || (j+col) == 3) || (mat[i+lin+lin][j+col+col] != 0)){
              flag = true;
            }
          }
        }
      }
    }
  }
  return flag;
}

void processa_entradas(jogo* jog) {
  switch (le_tecla())
  {
  case 'w':
    jog->lin = -1;
    jog->col = 0;
    break;
  case 'a':
    jog->lin = 0;
    jog->col = -1;
    break;
  case 's':
    jog->lin = 1;
    jog->col = 0;
    break;
  case 'd':
    jog->lin = 0;
    jog->col = 1;
    break;
  }
}

char le_tecla(void) {
  ALLEGRO_EVENT ev;

  while(al_get_next_event(fila_eventos, &ev)) {
    if(ev.type == ALLEGRO_EVENT_KEY_CHAR) {
      int c = ev.keyboard.unichar;

      if ((c >= ' ' && c <= '~') || c == '\b' || c == '\n') {
        return (char) c;
      }
    }
  }

  return '\0';
}

void pontuacao(jogo *jog) {
  int x, y, n = 0;

  for(x = 0; x < 4; x++) {
    for(y = 0; y < 4; y++) {
      if(jog->mat[x][y] == 3) {
        jog->pontos += 3;
      } else {
        n = jog->mat[x][y];
        jog->pontos += (3*(n/2));
      }
    }
  }
}

void ranking(jogo *jog) {
  FILE* file;

  file = fopen("ranking", "a");

  if(file == NULL) {
    printf("ARQUIVO NÃO ENCONTRADO");
  }

  fprintf(file, "%d\n", jog->pontos);
  fclose(file);

  ordem_decrescente(jog);
}

void ordem_decrescente(jogo* jog) {
  int aux;
  FILE* file;
  file = fopen("ranking", "r");
  for(int i = 0; i < 10; i++) {
    fscanf(file, "%d", &jog->ranking[i]);
  }

  fclose(file);

  file = fopen("ranking", "w");
  for(int i = 0; i < 10; i++) {
    for(int j = i+1; j < 10; j++) {
      if(jog->ranking[i] < jog->ranking[j]) {
        aux = jog->ranking[i];
        jog->ranking[i] = jog->ranking[j];
        jog->ranking[j] = aux;
      }
    }
    if(jog->ranking[i] != 0) {
      fprintf(file, "%d\n", jog->ranking[i]);
    }
  }
  fclose(file);
}

void tela_final(jogo* jog) {
  inicia_desenho();
  char s[20], txt[50];

  float x1=0, y1 = 0, x2 = LARGURA, y2= ALTURA, posicao = 160;
  float posicaoX = (x1+x2)/2;
  
  retangulo(x1, y1, x2, y2, 1, tela, tela);
  
  sprintf(s, "%d", jog->pontos);
  sprintf(txt, "Seus Pontos");

  al_draw_text(fonteRanking, cores[peca1], posicaoX, 75-26/2, ALLEGRO_ALIGN_CENTRE, s);
  al_draw_text(fonteRanking, cores[peca1], posicaoX, 40-26/2, ALLEGRO_ALIGN_CENTRE, txt);

  sprintf(txt, "Ranking");

  al_draw_text(fonteRanking, cores[peca1], posicaoX, 110-26/2, ALLEGRO_ALIGN_CENTRE, txt);

  for(int i = 0; i < 10; i++) {
    sprintf(s, "%d", jog->ranking[i]);
    if(jog->ranking[i] != 0) {
      al_draw_text(fonteRanking, cores[peca1], posicaoX, posicao-26/2, ALLEGRO_ALIGN_CENTRE, s);
      posicao += 40;
    }
  }

  termina_desenho();
}

void fim() {
  al_uninstall_system();
}