#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

/* Prosta gra snake oparta na liscie dwu kierunkowej.
 * W tej wersji snake'a, kazdy wezel zlozony jest z dwoch znakow, np. '[' i ']', poniewaz
 * ladniej i naturalniej to wyglada */

enum GAME_MODE { ONE_PLAYER = 1, TWO_PLAYERS = 2 };

typedef struct snakeEl *snakeElPnt;
typedef struct snakeEl{
	int x, y, dir; /* Wspolrzedne wezla oraz kierunek jego poruszania sie */
	char z1, z2;   /* Dwa znaki skladajace sie na wezel, w przypadku poruszania sie wezla
					* w prawo, gore, dol z1 to znak zewnetrzny np. ']' a z2 wewnetrzny np '['.
					* Na odwrot w przypadku ruchu w lewo.
					*/	
	short int l;			/*Dlugosc weza, uzywana narazie tylko do obliczen punktow */
	short int p_num;/*Numer gracza sterujacego tym wezem */
	snakeElPnt next;/*Nastepny wezel*/
	snakeElPnt prev;/*Poprzedni wezel*/
	snakeElPnt tail;/*Wskaznik na ogon, okreslony tylko dla wezla poczatkowego*/
} TsnakeEl;

/* sa dwa sposoby, albo zapamietywac zawsze ostatni element listy zeby nie trzeba bylo zawsze przez nia przechodzic cala
 * np. przy powiększaniu wezyka. Druga opcja jest bez zapamiętywania ogona weza i zawsze gdy jest potrzebny odnajdywanie go przez
 * przechodzenie listy, malo optymalne rozwiazanie jesli waz sie rozrosnie lub bedzie ich kilka */


typedef struct snakeEeats *snakeEatsPnt;
typedef struct snakeEeats{
	/* Jedzenie dla weza reprezentowane przez literki rozne. Pole gs mowi przy jakim numerze stanu gry byla ostatnia aktualizacja (ostatni zjedzony) snakeEats */
	int x, y;	/* Wspolrzedne */
	int gs;		/* Pole pomocniczne, moze byc uzyte do kontroli tego jakimi znakami
				 * mabyc wyswietlane jedzenie na ekranie */
	char z;		/* Symbol reprezentujacy jedzenie na ekranie */
} TsnakeEats;

int GAME_SPEED = 15;
int GAME_STATE = 1;
enum GAME_MODE PLAYERS_MODE = ONE_PLAYER;
int ROWS, COLS;

/* tablica symboli, ktore beda kolejno przedstawialy jedzenie dla wezyka */
/* char snakeEatsArray[]={ '!', 'e', 'i', 'C', '*', 'm', 'a', 'h', 'c', 'o', 'k' }; */
char snakeEatsArray[]={ '*' };

/* Funkcje obslugi weza */

void initSnake ( snakeElPnt* , int, int, short int);
void incSnake  ( snakeElPnt, char, char);
void printSnake( snakeElPnt );
void moveSnake ( snakeElPnt , char);
void initSnakeEats ( snakeEatsPnt* );
void setSnakeEats ( snakeEatsPnt );
void checkColSnkEats ( snakeElPnt , snakeEatsPnt );
void printSnakeEats ( snakeEatsPnt );
void checkColSnkSnk ( snakeElPnt, snakeElPnt );
void freeSnake ( snakeElPnt );

/* Funkcje obslugi ncurses */

void initNCurses();
void deinitNCurses();

/* Pozostale */

void displayScore(snakeElPnt, int, int);
void pressAnyKey();

void initSnake( snakeElPnt *t_snakeHead, int x_pos, int y_pos, short int t_p_num){
	/* Funkcja inicjujaca wezyka */ 
	(*t_snakeHead) = (snakeElPnt) malloc (sizeof(TsnakeEl)) ;
	(*t_snakeHead)->next = (*t_snakeHead)->prev = NULL;
	(*t_snakeHead)->x = x_pos;
	(*t_snakeHead)->y = y_pos;
	(*t_snakeHead)->z1 = ']';
	(*t_snakeHead)->z2 = '[';
	(*t_snakeHead)->dir = 2;
	(*t_snakeHead)->l = 1;
	(*t_snakeHead)->p_num = t_p_num;
	(*t_snakeHead)->tail = *t_snakeHead;
	incSnake( *t_snakeHead, ']', '[');
	incSnake( *t_snakeHead, ']', '[');
}

void incSnake( snakeElPnt t_snakeHead, char z1, char z2 ){
	/* Zwiekszanie dlugosci wezyka o jeden wezel */
	/*
	snakeElPnt tmp = t_snakeHead;
	while ( tmp -> prev != NULL ) tmp = tmp->prev;
	
	tmp->prev = (snakeElPnt) malloc (sizeof(TsnakeEl));
	tmp->prev->prev = NULL;
	tmp->prev->next = tmp;
	tmp->prev->dir = tmp->dir;
	tmp->prev->z = z;
	*/
	
	t_snakeHead->tail->prev = (snakeElPnt) malloc (sizeof(TsnakeEl));
	t_snakeHead->tail->prev->prev = NULL;
	t_snakeHead->tail->prev->next = t_snakeHead->tail;
	t_snakeHead->tail->prev->dir = t_snakeHead->tail->dir;
	t_snakeHead->tail->prev->z1 = z1;
	t_snakeHead->tail->prev->z2 = z2;
	t_snakeHead->tail = t_snakeHead->tail->prev;
	t_snakeHead->l++;
}

void printSnake( snakeElPnt t_snakeHead){
	/* Funkcja drukujaca weza */
	snakeElPnt tmp = t_snakeHead;
	
	attron(COLOR_PAIR(t_snakeHead->p_num));
	mvaddch( tmp->y, tmp->x, tmp->z1 );
	mvaddch( tmp->y, tmp->x-1, tmp->z2 );
	
	while ( tmp->prev != NULL ){
		tmp = tmp->prev ;
		mvaddch( tmp->y, tmp->x, tmp->z1 );
		mvaddch( tmp->y, tmp->x - 1, tmp->z2 );		
	}
	
	attroff(COLOR_PAIR(t_snakeHead->p_num));
}

void moveSnake ( snakeElPnt t_snakeHead, char pInput){
	/* Funkcja poruszajaca wezem. 
	 * Jako drugi argument pobiera wczesniej wczytany znak klawiatury.
	 * Jest niezbyt wydajna, poniewaz za kazdym razem musi przebiegac cala liste, ktora sie przeciez powieksza ale to jest tylko wersja testowa.
	 * Na samym koncu wywoluje funkcje rysujaca weza */
	
	if ( t_snakeHead->p_num == 1 ){
		switch( pInput ){
			case 'a': if ( t_snakeHead->dir != 2 ) (t_snakeHead)->dir = 4; break;
			case 'd': if ( t_snakeHead->dir != 4 ) (t_snakeHead)->dir = 2; break;
			case 's': if ( t_snakeHead->dir != 1 ) (t_snakeHead)->dir = 3; break;
			case 'w': if ( t_snakeHead->dir != 3 ) (t_snakeHead)->dir = 1; break;
			case 27 : GAME_STATE = 0; break;
		}
	}
	else if ( t_snakeHead->p_num == 2 ){
		 switch( pInput ){
			case 'j': if ( t_snakeHead->dir != 2 ) (t_snakeHead)->dir = 4; break;
			case 'l': if ( t_snakeHead->dir != 4 ) (t_snakeHead)->dir = 2; break;
			case 'k': if ( t_snakeHead->dir != 1 ) (t_snakeHead)->dir = 3; break;
			case 'i': if ( t_snakeHead->dir != 3 ) (t_snakeHead)->dir = 1; break;
			case 27 : GAME_STATE = 0; break;		
		 }
	}
	
	//snakeElPnt tmp = (t_snakeHead);
	snakeElPnt tmp = t_snakeHead->tail ;
	/*
	while ( tmp->prev != NULL ) {	 // przejscie listy w tyl, zeby znalezc ogon
	
		tmp = tmp->prev;
	}*/
	
	// zmazywanie ostatniego wezla
	mvaddch( tmp->y, tmp->x, ' ' );
	mvaddch( tmp->y, tmp->x-1, ' ' );
	
	while ( tmp != t_snakeHead ) {	
		tmp->x = tmp->next->x;
		tmp->y = tmp->next->y;
		tmp->dir = tmp->next->dir;
	
		tmp = tmp->next;
	}
	/* W zaleznosci od kierunku przesun glowe weza o odpowiednia ilosc znakow.
	 * W poziomie o dwa (poniewaz tyle zajmuje wezel w poziomie) a w pionie o jeden.
	 * Sa brane pod uwage granice ekranu terminala. 
	 */
	switch((t_snakeHead)->dir){
		case 1: if ((t_snakeHead)->y > 1) (t_snakeHead)->y--;
				else (t_snakeHead)->y = ROWS-2;
				break;
		case 2: if ((t_snakeHead)->x < COLS-3) (t_snakeHead)->x+=2;
				else (t_snakeHead)->x = 2; 
				break;
		case 3: if ((t_snakeHead)->y < ROWS-2) (t_snakeHead)->y++;
				else (t_snakeHead)->y = 1; 
				break;
		case 4: if ((t_snakeHead)->x > 2) (t_snakeHead)->x-=2;
				else (t_snakeHead)->x = COLS-2; 
				break;
	}
	
}

/* Funkcja sprawdza kolizje glowy weza 1 z wezem nr 2.
 * Mozna uzywac z samym soba
 */
void checkColSnkSnk( snakeElPnt t_1_snakeHead, snakeElPnt t_2_snakeHead){
	snakeElPnt tmp = t_2_snakeHead;
	if ( (t_1_snakeHead == t_2_snakeHead) && ( t_2_snakeHead->prev != NULL) ) tmp = tmp->prev;
	while( tmp != NULL ){
		if ( (t_1_snakeHead->x == tmp->x ) && (t_1_snakeHead->y == tmp->y) || 
			 (t_1_snakeHead->x-1 == tmp->x ) && (t_1_snakeHead->y == tmp->y) ||
			 (t_1_snakeHead->x == tmp->x-1 ) && (t_1_snakeHead->y == tmp->y) ||
			 (t_1_snakeHead->x-1 == tmp->x-1 ) && (t_1_snakeHead->y == tmp->y)) 
				GAME_STATE=0;
		tmp = tmp->prev;
	}
}


/* Allokuje pamiec dla struktury jedzenie dla weza i umieszcza 
 * ja w losowym miejscu na planszy */
void initSnakeEats( snakeEatsPnt* t_snakeEats){
	(*t_snakeEats) = (snakeEatsPnt) malloc (sizeof(TsnakeEats));
	(*t_snakeEats)->gs = GAME_STATE;
	
	/* GAME_STATE jest uzywany jesli symbole reprezentujace jedzenie dla weza
	 * maja byc rozne, wtedy sa wybierane odpowiednio z tablicy snakeEatsArray[].
	 * W normalnym trybie jedzenie zawsze ma symbol '*'
	 */ 
	if ( GAME_STATE <= sizeof(snakeEatsArray) )
		(*t_snakeEats)->z = snakeEatsArray[GAME_STATE-1];
	else 
		(*t_snakeEats)->z = '*';
	srand(time(NULL));
	(*t_snakeEats)->x = (rand()%(COLS-3)) + 2;
	(*t_snakeEats)->y = (rand()%(ROWS-3)) + 2;
}

void setSnakeEats( snakeEatsPnt t_snakeEats ){
	if ( t_snakeEats->gs != GAME_STATE ){
		srand(time(NULL));
		t_snakeEats->x = (rand()%(COLS-3)) + 2;
		t_snakeEats->y = (rand()%(ROWS-3)) + 2;
		if ( GAME_STATE > 0 && GAME_STATE < sizeof(snakeEatsArray) ){ 
			t_snakeEats->z = snakeEatsArray[GAME_STATE-1];
			++t_snakeEats->gs;
		}
		else {
			t_snakeEats->z = '*';
			++t_snakeEats->gs;
		}
	}
}

/* Funkcja sprawdzajaca kolizje weza z jedzeniem, sprawdza kolizje pierwszego znaku wezla 
 * lub kolizje drugiego znaku wezla
 */
void checkColSnkEats( snakeElPnt t_snakeHead, snakeEatsPnt t_snakeEats ){
	if ( (t_snakeHead->x == t_snakeEats->x && t_snakeHead->y == t_snakeEats->y) ||
		 (t_snakeHead->x-1 == t_snakeEats->x && t_snakeHead->y == t_snakeEats->y)) {
		incSnake( t_snakeHead, ']', '[');
		++GAME_STATE;
	}
}

void printSnakeEats( snakeEatsPnt t_snakeEats ){
	attron(COLOR_PAIR(3));
	mvaddch( t_snakeEats->y, t_snakeEats->x, t_snakeEats->z);
	attroff(COLOR_PAIR(3));
}

void freeSnake(snakeElPnt t_snakeHead){
	/* Zwalnianie pamieci po wezu */
	snakeElPnt tmp = t_snakeHead->prev;
	snakeElPnt tmp2;
	free(t_snakeHead);
	while ( tmp->prev != NULL){
		tmp2 = tmp->prev;
		free(tmp);
		tmp = tmp2;
	}
}

/* Wyswietla wynik snake'a */
void displayScore(snakeElPnt t_snakeHead, int pos_x, int pos_y){
	int i;
	for (i = 0; i <= t_snakeHead->l * (16-GAME_SPEED)  ; ++i) {
		mvprintw(pos_y, pos_x, "Player %d Score: %d", t_snakeHead->p_num, i);
		refresh();
		usleep(5000);
	}
}

/* Czeka na nacisniecie dowolnego klawisza */
void pressAnyKey(){
	while( getch() == ERR ){}
}

int main(int argc, char** argv){
	int i;
	for( i = 1; i < argc; ++i ){ 
		if ( (argv[i][0] >= '0') && (argv[i][0] <= '9') ){
			GAME_SPEED = 15-(argv[i][0]-'0');
		}
		if ( strcmp(argv[i], "tp") == 0 ){
			PLAYERS_MODE = TWO_PLAYERS;
		}
	}

		
	initNCurses(); /* Funkcja inicjujaca ncurses */
	getmaxyx(stdscr , ROWS, COLS);
	
	snakeElPnt P1_snakeHead, P2_snakeHead;
	snakeEatsPnt snakeEats;
	char playerInput;
	
	srand(time(NULL));
	initSnake ( &P1_snakeHead, (rand()%(COLS-6))+5, (rand()%(ROWS-6))+5, 1);
	if ( PLAYERS_MODE == TWO_PLAYERS )initSnake ( &P2_snakeHead, (rand()%(COLS-6))+5, (rand()%(ROWS-6))+5, 2);
	initSnakeEats( &snakeEats);
	
	switch(PLAYERS_MODE){
		case ONE_PLAYER:
			while(GAME_STATE){
				playerInput = getch();
				moveSnake( P1_snakeHead, playerInput); 
				checkColSnkEats( P1_snakeHead, snakeEats); /* Sprawdzanie kolizji waz - jedzenie */
				checkColSnkSnk( P1_snakeHead, P1_snakeHead);	/* Spr. kol. snake - snake */
				setSnakeEats( snakeEats );
				box(stdscr, 0 , 0);
				printSnake( P1_snakeHead);
				printSnakeEats( snakeEats );
				move( 0, 0); // przesuwa znak zachety (?)
				refresh();
				usleep(GAME_SPEED*10000); // 200000 =  1/5sekundy
			}
			clear();
			displayScore( P1_snakeHead, 2, 2); 
			pressAnyKey();
			freeSnake(P1_snakeHead);
		break;
		case TWO_PLAYERS:
			while(GAME_STATE){
				playerInput = getch();
				moveSnake( P1_snakeHead, playerInput); 
				moveSnake( P2_snakeHead, playerInput);	
				checkColSnkEats( P1_snakeHead, snakeEats); /* Sprawdzanie kolizji waz - jedzenie */
				checkColSnkSnk( P1_snakeHead, P1_snakeHead);	/* Spr. kol. snake - snake */
				checkColSnkEats( P2_snakeHead, snakeEats);
				checkColSnkSnk( P1_snakeHead, P2_snakeHead);
				checkColSnkSnk( P2_snakeHead, P1_snakeHead);
				setSnakeEats( snakeEats );
				box(stdscr, 0 , 0);
				printSnake( P1_snakeHead);
				printSnake( P2_snakeHead );
				printSnakeEats( snakeEats );
				move( 0, 0); // przesuwa znak zachety (?)
				refresh();
				usleep(GAME_SPEED*10000); // 200000 =  1/5sekundy
			}
			clear();
			displayScore( P1_snakeHead, 2, 2); 
			displayScore( P2_snakeHead, 2, 4);
			pressAnyKey();
			freeSnake(P1_snakeHead);
			freeSnake(P2_snakeHead);
		break;
	}
	deinitNCurses();
	return 0;
}

void initNCurses(){
	if ( initscr() == 0 ){					
		printf("Problemy z inicjalizacja ncurses\n");
		exit(1);
	}							
	noecho();
	raw();								
	//cbreak();
	nodelay(stdscr, true);
	curs_set(0);
	box(stdscr, 0 , 0);
	
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_GREEN);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	
	refresh();
}

void deinitNCurses(){
	endwin();			/* End curses mode		  */
}
