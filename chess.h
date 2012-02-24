#ifndef __CH_CHESS_H__
#define __CH_CHESS_H__

#include "tree.h"

/* makro pro inicializaci figury */
#define ch_figure_init(fig, typ, val, pos, mov, multi) \
	(fig).type = (char)(typ); \
	(fig).value = (short int)(val); \
	(fig).position = (char)(pos); \
	(fig).move = (char)(mov); \
	(fig).multiply = (char)(multi)

/* makro pro umisteni figury na hraci pole 
 * pri uspechu vrati odkaz na figuru s kterou se tahlo
 * jinak pokud tam neco stalo vrati odkaz na figuru, ktera tam stoji
 * jinak vrati NULL */
#define ch_figure_set(b, f) ((b)[(int)(f)->position].figure != NULL || \
				(b)[(int)(f)->position].value == CH_BORDER || \
				(f)->position < 0 || (f)->position > 119) ? \
				(b)[(int)(f)->position].figure : \
			        ((b)[(int)(f)->position].figure = f)

/* makro pro tisk chyb */
#define ch_error_print() if (ch_error!=CH_NO_ERRORS) ch_print_error()
				
typedef int player;

/* struktura popisujici figuru */
typedef struct ch_figure {
	char type;	/* druh figury */
	short int value;	/* hodnota */
	char position;	/* aktualni pozice */
	unsigned char move;
	char multiply;	/* muze provadet vice kroku? */
} ch_figure;

/* jedno hraci pole sachovnice */
typedef struct ch_field {
	ch_figure *figure;	/* co tam stoji za figuru */
	char value;	/* hodnota pole */
	int danger;	/* zda je pole ohrozovano nepr. figurou */
} ch_field;

/* sturktura popisujici hru */
typedef struct ch_board {
	ch_figure white[16];	/* bile figury */
	ch_figure black[16];	/* cerne figury */
	ch_field board[120];	/* sachovnice */
	char on_move;			/* ktera barva na tahu */
	int move;				/* kolikaty pultah */
	int check;				/* kdo ma sach */
	player wp;				/* kdo hraje za bileho hrace */
	player bp;				/* kdo hraje za cerneho hrace */
	player pl;				/* ktery hrac je na tahu */
	int wcost;				/* jak dobre hraje bily hrac */
	int bcost;				/* jak dobre hraje cerny hrac */
	int cost;				/* kdo je na tom lip */
	tree *node;				/* strom tahu */
} ch_board;


/* chybove konstanty */
typedef enum {
	CH_NO_ERRORS, CH_NOT_MEM, CH_NULL_POINTER, CH_ILLEGAL_MOVE, CH_END_OF_FILE
} ch_eerror;

/* chybova promena */
extern ch_eerror ch_error;

/* inicializace struktury ch_board */
extern ch_board *ch_board_init(ch_board *board);

/* konec hry -- zrusi strom */
extern void ch_board_done(ch_board *board);

/* zacatek hry */
extern void ch_game_start(ch_board *board);

/* tah figurou */
extern inline ch_figure* ch_figure_move(ch_field board[], char src, char target, int flag);

/* vezmi tah zpet */
extern void ch_board_undo(ch_board *board);
	
#endif
