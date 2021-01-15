#include <stdbool.h>

typedef struct {
	int mat[4][4];

	int prox_peca;

	int lin, col;

	int pontos;

	int ranking[10];
} jogo;

int main();
void inicializa_mundo(jogo* jog);
void preencher_mat(jogo* jog);
void inicializa_janela(float larg, float alt, char txt[]);
void inicializa_teclado();
void inicializa_cores();
void inicializa_fontes();
void inicio_jogo(jogo* jog);
int sorteio(int quant);
int escolhe_peca_inicio(int prob);
int proxima_peca();
int escolhe_peca_maior();
int escolhe_peca_menor(int prob);
void desenha_mundo(jogo* jog);
void desenho_inicial(jogo* jog);
void inicia_desenho();
void retangulo (float x1, float y1, float x2, float y2, float i, int cor, int cor2);
void termina_desenho();
int escolhe_cor(int n);
int cor_texto(int n);
void coloca_peca(int n);
bool atualiza_mat(jogo *jog);
bool derrota(jogo *jog);
bool condicaoWA(int mat[4][4], int lin, int col);
bool condicaoSD(int mat[4][4], int lin, int col);
void processa_entradas(jogo* jog);
char le_tecla(void);
void pontuacao(jogo *jog);
void ranking(jogo *jog);
void ordem_decrescente(jogo* jog);
void tela_final(jogo* jog);
void fim();