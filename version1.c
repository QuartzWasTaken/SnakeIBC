/*!
 \file
 \page Général 
 
 \author GOURDON Gabriel
 \version 1.0
 \date 22 octobre 2024
 \brief Un snake pour la SAE 1.01
**/

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

// Déclaration des constantes
/**
 * \def TAILLE_TABLEAU
 * \brief La taille du tableau dans lequel le serpent va évoluer
 */
#define TAILLE_TABLEAU 40 

/**
 * \def TAILLE_SERPENT
 * \brief La taille du serpent
 * Elle ne changera pas pour le moment
 */
#define TAILLE_SERPENT 10

// Déclaration des fonctions fournies
void gotoXY(int x, int y);
int kbhit();

// Déclaration des fonctions demandées
void genererSerpent(int positions[TAILLE_SERPENT][2], int x, int y);
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int positions[TAILLE_SERPENT][2]);
void progresser(int positions[TAILLE_SERPENT][2]);
void effacerEcran();


/**
* \fn int main()
* \brief Programme principal.
* \return Code de sortie du programme (0 : sortie normale).
* 
* Le programme principal exécute le code donné
*/
int main()
{
	int positions[TAILLE_SERPENT][2];
	int x, y;

	// Demander à l'utilisateur de rentrer la position initiale
	printf("Entrez la position initiale de la tête du serpent (x, puis y): ");
	scanf("%d", &x);
	scanf("%d", &y);
	effacerEcran();

	genererSerpent(positions, x, y);
	dessinerSerpent(positions);

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

void effacerEcran()
{
    system("clear");
}

/*!
 \fn void genererSerpent(int positions[TAILLE_SERPENT][2], int x, int y)
 \brief La fonction qui génère le serpent
 \param positions La liste des positions du serpent
 \param x La position X de la tête du serpent
 \param y La position Y de la tête du serpent
 Cette fonction créé la liste des positions du serpent dans la liste en argument positions
*/

void genererSerpent(int positions[TAILLE_SERPENT][2], int x, int y)
{
	int copiePos[TAILLE_SERPENT][2] = positions;
	for(int nbCellule = 0; nbCellule < TAILLE_SERPENT; nbCellule++)
	{
		positions[nbCellule][0] = x - nbCellule; // positions[][I] 0 = X 1 = Y
		positions[nbCellule][1] = y;
	}
}

void dessinerSerpent(int positions[TAILLE_SERPENT][2])
{
	for(int iDessine = 0; iDessine < TAILLE_SERPENT; iDessine++)
	{
		int aDessinerX = positions[iDessine][0];
		int aDessinerY = positions[iDessine][1];

		if(iDessine == 0)
		{
			afficher(aDessinerX, aDessinerX, 'X');
		}
		else
		{
			afficher(aDessinerX, aDessinerY, 'O');
		}
	}
}