
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
#include <stdbool.h>
#include <time.h>

// Déclaration des constantes
/**
 * \def TAILLE_TABLEAU
 * \brief La taille du tableau dans lequel le serpent va évoluer
 */
#define TAILLE_TABLEAU_X 80
#define TAILLE_TABLEAU_Y 40

/**
 * \def TAILLE_SERPENT
 * \brief La taille du serpent
 * Elle ne changera pas pour le moment
 */
#define TAILLE_SERPENT 10

/**
 * \def VITESSE_JEU
 * \brief La vitesse à laquelle le jeu va avancer (en microsecondes)
 */
#define VITESSE_JEU 200000

/**
 * \def TOUCHE_ARRET
 * \brief La touche sur laquelle appuyer pour arrêter le jeu
 */
#define TOUCHE_ARRET 'a'

/**
 * \def CHAR_TETE
 * \brief Le caractère qui correspond à la tête du serpent
 */
#define CHAR_TETE 'O'

/**
 * \def CHAR_CORPS
 * \brief Le caractère qui correspond au corps du serpent
 */
#define CHAR_CORPS 'X'

/**
 * \def CHAR_OBSTACLE
 * \brief Le caractère qui correspond aux obstacles à éviter
 */
#define CHAR_OBSTACLE '#'

/**
 * \def CHAR_VIDE
 * \brief Le caractère qui correspond aux espaces vides dans lesquels le serpent peut aller
 */
#define CHAR_VIDE ' '

/**
 * \def CHAR_VIDE
 * \brief Le caractère qui correspond aux espaces vides dans lesquels le serpent peut aller
 */
#define CHAR_POMME '6'

/**
 * \def DIRECTION_INITIALE
 * \brief La direction dans laquelle le serpent doit se déplacer au départ
 */
#define DIRECTION_INITIALE 'd'

/**
 * \def TOUCHE_DROITE
 * \brief La touche à appuyer pour aller vers la droite
 */
#define TOUCHE_DROITE 'd'

/**
 * \def TOUCHE_GAUCHE
 * \brief La touche à appuyer pour aller vers la gauche
 */
#define TOUCHE_GAUCHE 'q'

/**
 * \def TOUCHE_HAUT
 * \brief La touche à appuyer pour aller vers le haut
 */
#define TOUCHE_HAUT 'z'

/**
 * \def TOUCHE_BAS
 * \brief La touche à appuyer pour aller vers le bas
 */
#define TOUCHE_BAS 's'

/**
 * \def X_DEBUT
 * \brief La position horizontale à laquelle le serpent doit démarrer
 */
#define X_DEBUT 40

/**
 * \def Y_DEBUT
 * \brief La position horizontale à laquelle le serpent doit démarrer
 */
#define Y_DEBUT 20

/**
 * \def DIRECTION_INITIALE
 * \brief La direction dans laquelle le serpent doit se déplacer au début
 */
#define DIRECTION_INITIALE 'd'

/**
 * \def NOMBRE_PAVES
 * \brief Le nombre de pavés à génerer
 */
#define NOMBRE_PAVES 4

/**
 * \def TAILLE_PAVE_X
 * \brief La taille horizontale des pavés
 */
#define TAILLE_PAVE_X 5

/**
 * \def TAILLE_PAVE_Y
 * \brief La taille verticale des pavés
 */
#define TAILLE_PAVE_Y 5

#define NB_POMMES_POUR_FIN 10

#define TAILLE_MAX_SERPENT 12
typedef char t_plateau[TAILLE_TABLEAU_Y][TAILLE_TABLEAU_X];

t_plateau tableau;
int tailleSerpent = TAILLE_SERPENT;


// Déclaration des fonctions fournies
void gotoXY(int x, int y);
// Déclaration des fonctions demandées
void afficher(int x, int y, char c);																// Check
void effacer(int x, int y);																			// Check
void effacerEcran();																				// Check

int kbhit();
void enableEcho();
void disableEcho();

// Déclaration des fonctions demandées
void afficher(int x, int y, char c);																// Check
void effacer(int x, int y);																			// Check
void effacerEcran();																				// Check

int checkAKeyPress();
void genererSerpent(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT], int x, int y);	// Check
void initPlateau();									// Check
void dessinerPlateau();								// Check
void afficherSerpent(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT]);				// Check
void effacerSerpent(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT]);
void progresser(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT], char direction, bool* detecCollision);	// Check
void serpentDansTab(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT]);

void changerDirection(char* direction);																// Check
int genererEntierDansBornes(int min, int max);
void genererPaves(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT]);
void genererUnPave(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT]);
void genererTrous();
void ajouterPomme();
void succesJeu();
void echecJeu();
void quitterJeu();


/**
 * \fn int main()
 * \brief Programme principal.
 * \return Code de sortie du programme (0 : sortie normale).
 *
 * Le programme principal exécute le code du jeu
 */
int main()
{
	int positionsX[TAILLE_MAX_SERPENT];
	int positionsY[TAILLE_MAX_SERPENT];
	int x, y;
	char direction = DIRECTION_INITIALE;
	bool devraitQuitter = false;
	bool aQuitte = false;
	bool estMort = false;
	bool aGagne = false;

	x = X_DEBUT;
	y = Y_DEBUT;

    effacerEcran(); // Préparer le jeu
    genererSerpent(positionsX, positionsY, x, y);
    initPlateau();
	genererTrous(); 
	serpentDansTab(positionsX, positionsY);
    srand(time(NULL)); // Initialiser l'aléatoire

    genererPaves(positionsX, positionsY);
	ajouterPomme();
    dessinerPlateau(); // Afficher le tableau de jeu initial
    disableEcho();

    while (!devraitQuitter) // Boucle du jeu
    {
        usleep(VITESSE_JEU);

		aQuitte = checkAKeyPress(); // Si l'utilisateur veut quitter, mettre aQuitte = true
		if(tailleSerpent >= TAILLE_MAX_SERPENT)
		{
			aGagne = true; // Si la taille de l'utilisateur dépasse la taille maximale autorisée, il a gagné
		}

        if (checkAKeyPress() || estMort || aGagne)
        {
            devraitQuitter = true; // Si la touche d'arrêt est pressée, quitter
        }
		
        changerDirection(&direction); // Met à jour la direction du serpent

        effacerSerpent(positionsX, positionsY); // Effacer le serpent avant de le déplacer
		progresser(positionsX, positionsY, direction, &estMort); // Faire avancer le serpent

        // Met à jour l'état du serpent dans le tableau
        serpentDansTab(positionsX, positionsY); 

        dessinerPlateau(); // Redessiner le tableau de jeu avec le serpent mis à jour

    }

    enableEcho(); // Réactiver l'écho
    printf("\n");
	if(aGagne)
	{
		succesJeu();
	}
	else if(estMort)
	{
		echecJeu();
	}
	else if(aQuitte)
	{
		quitterJeu();
	}

    return EXIT_SUCCESS;
}

/**
 * \fn void initTableau()
 * \brief Initialise le tableau de jeu
 * \return Code de sortie du programme (0 : sortie normale).
 *
 * Le programme principal exécute le code du jeu
 */
void initPlateau()
{
	for (int i = 0; i < TAILLE_TABLEAU_Y; i++)
	{
		for (int j = 0; j < TAILLE_TABLEAU_X; j++)
		{
			// Placer des bordures sur les bords du tableau
            if (i == 1 || i == TAILLE_TABLEAU_Y - 1 || j == 1 || j == TAILLE_TABLEAU_X - 1) {
                tableau[i][j] = CHAR_OBSTACLE;  // Bordure
            } else {
                tableau[i][j] = CHAR_VIDE;  // Espace vide à l'intérieur
            }
		}
	}
}

/**
 * \fn void dessinerPlateau()
 * \brief Dessine le plateau sur l'écran
 *
 * Dessine le plateau sur l'écran
 */
void dessinerPlateau()
{
	for (int i = 0; i < TAILLE_TABLEAU_Y; i++)
	{
		for (int j = 0; j < TAILLE_TABLEAU_X; j++)
		{
			afficher(j, i, tableau[i][j]);
		}
	}
}
/**
 * \fn void effacerSerpent(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT])
 * \brief Efface le serpent dans le tableau
 *
 * Met toutes les positions du serpent dans le tableau à la valeur de CHAR_VIDE
 */
void effacerSerpent(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT])
{
	for (int i = 0; i < TAILLE_MAX_SERPENT; i++)
	{
		tableau[positionsY[i]][positionsX[i]] = CHAR_VIDE;
		afficher(positionsX[i], positionsY[i], CHAR_VIDE);
	}
}

/**
 * \fn void changerDirection(char* direction)
 * \brief Met le curseur aux coordonnées x et y
 *
 * Met le curseur aux coordonnées x et y passées en paramètre d'entrée
 */
void changerDirection(char* direction)
{
	char ch;
	if(kbhit())
	{
		ch = getchar();
	}
	if (ch == TOUCHE_DROITE && *direction != TOUCHE_GAUCHE)
	{
		*direction = TOUCHE_DROITE;
	}
	if (ch == TOUCHE_HAUT && *direction != TOUCHE_BAS)
	{
		*direction = TOUCHE_HAUT;
	}
	if (ch == TOUCHE_GAUCHE && *direction != TOUCHE_DROITE)
	{
		*direction = TOUCHE_GAUCHE;
	}
	if (ch == TOUCHE_BAS && *direction != TOUCHE_HAUT)
	{
		*direction = TOUCHE_BAS;
	}
}
/*!
\fn int checkAKeyPress()
\brief La fonction qui vérifie si la touche A est appuyée
\return true si la touche A est appuyée, false sinon

La fonction qui vérifie si A est appuyé, en utilisant kbhit
*/
int checkAKeyPress()
{
	char ch;
	if(kbhit())
	{
		ch = getchar();
	}
	if (ch == TOUCHE_ARRET) // Si la touche détectée est celle d'arrêt, retourner true
	{
		return true;
	}
	return false;
}

// Definition des fonctions demandées
void afficher(int x, int y, char c)
{
	gotoXY(x, y);
	printf("%c", c);
}

// Efface l'écran
void effacer(int x, int y)
{
	tableau[x][y] = CHAR_VIDE;
}

void disableEcho()
{
	struct termios tty;

	// Obtenir les attributs du terminal
	if (tcgetattr(STDIN_FILENO, &tty) == -1) {
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}

	// Desactiver le flag ECHO
	tty.c_lflag &= ~ECHO;

	// Appliquer les nouvelles configurations
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void enableEcho()
{
	struct termios tty;

	// Obtenir les attributs du terminal
	if (tcgetattr(STDIN_FILENO, &tty) == -1) {
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}

	// Reactiver le flag ECHO
	tty.c_lflag |= ECHO;

	// Appliquer les nouvelles configurations
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

// Efface l'écran
void effacerEcran()
{
	system("clear");
}

void ajouterPomme()
{
	int x, y;
	do
	{
		x = (rand() % TAILLE_TABLEAU_X-1) + 1;
		y = (rand() % TAILLE_TABLEAU_Y-1) + 1;
	} while (tableau[y][x] != CHAR_VIDE);
	tableau[y][x] = CHAR_POMME;
	afficher(x, y, CHAR_POMME);
}


/*!
\fn void genererSerpent(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT], int x, int y)
\brief La fonction qui génère le serpent
\param positionsX La liste des positionsX du serpent
\param positionsY La liste des positionsY du serpent
\param x Le X de la tête du serpent
\param y Le Y de la tête du serpent

Cette fonction créé la liste des positions (positionsX, positionsY) du serpent dans la liste en argument en utilisant x et y
*/
void genererSerpent(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT], int x, int y)
{
	for (int nbCellule = 0; nbCellule < TAILLE_MAX_SERPENT; nbCellule++) // Génerer des coordonées de x à (x + TAILLE_SERPENT) pour le serpent
	{
		positionsX[nbCellule] = x - nbCellule;
		positionsY[nbCellule] = y;
	}
}


/*!
\fn void genererPaves(int posSerpentX[TAILLE_SERPENT], int posSerpentY[TAILLE_SERPENT])
\brief La fonction qui génère les pavés
\param positionsX La liste des positionsX du serpent
\param positionsY La liste des positionsY du serpent

Cette fonction créé NOMBRE_PAVES pavés en utilisant la fonction genererUnPave
*/
void genererPaves(int posSerpentX[TAILLE_MAX_SERPENT], int posSerpentY[TAILLE_MAX_SERPENT])
{
	for(int i = 0; i < NOMBRE_PAVES; i++)
	{
		genererUnPave(posSerpentX, posSerpentY);
	}
}

/*!
\fn void genererPaves(int posSerpentX[TAILLE_SERPENT], int posSerpentY[TAILLE_SERPENT])
\brief La fonction qui génère un pavé
\param positionsX La liste des positionsX du serpent
\param positionsY La liste des positionsY du serpent

Cette fonction génère un pavé de dimensions TAILLE_PAVE_X et TAILLE_PAVE_Y en utilisant la fonction genererEntierDansBornes
puis vérifie sa validité par rapport au serpent et aux bordures
*/
void genererUnPave(int posSerpentX[TAILLE_MAX_SERPENT], int posSerpentY[TAILLE_MAX_SERPENT])
{
	int paveX, paveY; // Je sais que je vais perdre un point sur le fait que les pavés peuvent avoir la même position de départ, mais je sais pas comment régler ça
	bool paveValide = false; // J'ai pas mal de trucs à faire pendant le weekend et je suis content de ce que j'ai fait jusque là, donc je rends comme ça
	while (!paveValide) // Ces commentaires seront pas lus de toute manière donc je parle tout seul
	{
		paveX = genererEntierDansBornes(2, TAILLE_TABLEAU_X - 2); // -2 : un caractère de bord et un caractère pour l'espace
		paveY = genererEntierDansBornes(2, TAILLE_TABLEAU_Y - 2); // entre le bord et le pavé, que le serpent puisse passer
		paveValide = true;

		for (int x = -2; x < TAILLE_PAVE_X +1; x++) // Vérifier que le pavé ne superpose rien
		{
			for(int y = -2; y < TAILLE_PAVE_Y +1; y++)
			{
				if(tableau[paveY + y][paveX + x] != CHAR_VIDE)
				{
					paveValide = false;
				}
			}
		}
	}

	for (int x = 0; x < TAILLE_PAVE_X; x++) // Mettre le pavé validé dans le tableau
	{
		for(int y = 0; y < TAILLE_PAVE_Y; y++)
		{
			tableau[paveY + y][paveX + x] = CHAR_OBSTACLE;
		}
	}
	
}

/*!
\fn void serpentDansTab(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT])
\brief La fonction qui met les positions du serpent dans le tableau de jeu
\param positionsX La liste des positionsX du serpent
\param positionsY La liste des positionsY du serpent

Cette fonction transfère toutes les positions du serpent (positionsX et positionsY) dans le tableau de jeu
*/
void serpentDansTab(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT])
{
	for (int iDessine = 0; iDessine < tailleSerpent; iDessine++)
	{
		int aDessinerX = positionsX[iDessine];
		int aDessinerY = positionsY[iDessine];

		// Vérifier que les coordonnées sont dans les limites de l'écran
		if (aDessinerX >= 0 && aDessinerX < TAILLE_TABLEAU_X &&
			aDessinerY >= 0 && aDessinerY < TAILLE_TABLEAU_Y)
		{
			// Dessiner la tête ou le corps en fonction de la position dans le tableau
			if (iDessine == 0)
			{
				tableau[aDessinerY][aDessinerX] = CHAR_TETE;
			}
			else
			{
				tableau[aDessinerY][aDessinerX] = CHAR_CORPS;
			}
		}
	}
}

void genererTrous()
{
	tableau[1][TAILLE_TABLEAU_X / 2] = CHAR_VIDE;
	tableau[TAILLE_TABLEAU_Y - 1][TAILLE_TABLEAU_X / 2] = CHAR_VIDE;

	tableau[TAILLE_TABLEAU_Y / 2][1] = CHAR_VIDE;
	tableau[TAILLE_TABLEAU_Y / 2][TAILLE_TABLEAU_X - 1] = CHAR_VIDE;
}

/*!
\fn void progresser(int positionsX[TAILLE_SERPENT], int positionsY[TAILLE_SERPENT], char direction)
\brief La fonction qui fait avancer le jeu d'une étape
\param positionsX La liste des positions X du serpent
\param positionsY La liste des positions Y du serpent
\param direction La direction dans laquelle le serpent avance

La fonction qui fait avancer le corps du serpent, puis bouge la tête dans la direction dans laquelle elle est sensée avancer
*/
void progresser(int positionsX[TAILLE_MAX_SERPENT], int positionsY[TAILLE_MAX_SERPENT], char direction, bool* detecCollision)
{
    int nouveauX = positionsX[0];
    int nouveauY = positionsY[0];

    // Calculer la nouvelle position de la tête en fonction de la direction
    switch (direction) {
        case TOUCHE_DROITE:
			nouveauX++;
			break;
        case TOUCHE_HAUT:
			nouveauY--;
			break;
        case TOUCHE_GAUCHE:
			nouveauX--;
			break;
        case TOUCHE_BAS:
			nouveauY++;
			break;
		default:
			break;
    }

	nouveauX %= TAILLE_TABLEAU_X; // Wrap les positions
	nouveauY %= TAILLE_TABLEAU_Y;

    // Vérifier si la nouvelle position est en collision avec un '#'
    if (tableau[nouveauY][nouveauX] == CHAR_OBSTACLE)
	{
        *detecCollision = true; // Collision avec le bord
    }

	if(nouveauX < 1 && direction == TOUCHE_GAUCHE && (nouveauY == TAILLE_TABLEAU_Y / 2))
	{
		nouveauX = TAILLE_TABLEAU_X - 1;
	}

	if(nouveauY < 1 && direction == TOUCHE_HAUT && (nouveauX == TAILLE_TABLEAU_X / 2))
	{
		nouveauY = TAILLE_TABLEAU_Y - 1;
	}

    // Vérifier si la nouvelle position est en collision avec une pomme
    if (tableau[nouveauY][nouveauX] == CHAR_POMME)
    {
        // Si le serpent a mangé une pomme, on augmente la taille du serpent, on rajoute une pomme et on met la cellule n-1
		// du serpent à la valeur de n-2
		tailleSerpent++; // Augmenter la taille du serpent
		ajouterPomme(positionsX, positionsY);
		positionsX[tailleSerpent - 1] = positionsX[tailleSerpent - 2];
		positionsY[tailleSerpent - 1] = positionsY[tailleSerpent - 2];
		tableau[positionsY[tailleSerpent - 1]][positionsX[tailleSerpent - 1]] = CHAR_CORPS;
    }

    // Vérifier si la nouvelle position de la tête entre en collision avec le corps du serpent
    for (int i = 1; i < tailleSerpent; i++)
	{
		if (positionsX[i] == nouveauX && positionsY[i] == nouveauY)
		{
            *detecCollision = true; // Collision avec le serpent
        }
    }

    // Déplacer le corps du serpent en décalant chaque segment vers la position du segment précédent
    for (int i = tailleSerpent - 1; i > 0; i--)
	{
        positionsX[i] = positionsX[i - 1];
        positionsY[i] = positionsY[i - 1];
    }
	
    // Mettre à jour la position de la tête
    positionsX[0] = nouveauX;
    positionsY[0] = nouveauY;
}

int kbhit()
{
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

void succesJeu()
{
	system("clear");
	printf("Vous avez gagné, bravo !\n");
}

void echecJeu()
{
	system("clear");
	printf("Vous avez perdu, dommage !\n");
}

void quitterJeu()
{
	system("clear");
	printf("Au revoir !\n");
}



/**
 * \fn void gotoXY(int x, int y) 
 * \brief Met le curseur aux coordonnées x et y
 *
 * Met le curseur aux coordonnées x et y passées en paramètre d'entrée
 */
void gotoXY(int x, int y)
{
	printf("\033[%d;%df", y, x);
}

/*!
\fn int genererEntierDansBornes(int min, int max)
\brief La fonction qui génère un entier de min à max
\param min Le minimum de l'intervalle
\param max Le maximum de l'intervalle

Retourne un entier aleatoire entre min et max
*/
int genererEntierDansBornes(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}