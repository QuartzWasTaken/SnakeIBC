#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

// Déclaration des constantes
#define TAILLE_TABLEAU 40

// Déclaration des fonctions fournies
void gotoXY(int x, int y);
int kbhit();

// Déclaration des fonctions demandées
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int position[TAILLE_TABLEAU][2]);
void progresser(int position[TAILLE_TABLEAU][2]);



int main()
{
    return 0;
}

// Définition des fonctions fournies
void gotoXY(int x, int y) { 
    printf("\033[%d;%df", y, x);
}
int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

// Definition des fonctions demandées
void afficher(int x, int y, char c)
{
    gotoXY(x, y);
    printf("%c", c);
}

void effacer(int x, int y)
{
    gotoXY(x, y);
    printf(" ");
}
