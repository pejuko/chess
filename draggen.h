#ifndef __CH_DRAG_GENERATOR_H__
#define __CH_DRAG_GENERATOR_H__

#include "chess.h"

typedef struct ch_move {
	char src;	/* odkud */
	char trg;	/* kam */
	int cost;	/* oceneni tahu */
	char change;	/* dojde pri tahu ke zmene figury? */
	ch_figure *take;	/* jakou figuru pri tahu vezme */
} ch_move;

/* mozne smery a tahy figur */
extern char ch_offset[];

/* oznaci ohrozena a neohrozena pole nepritelem */
extern void ch_gen_dangers(ch_board *board);

/* generuje pro danou pozici vsechny mozne tahy */
extern void ch_gen_moves(ch_board *board);

/* generuje pro mozne tahy protitahy */
extern void ch_gen_submoves(ch_board *board);

/* provadi bilanci hry */
extern void ch_gen_cost(ch_board *board);

/* urci pro tahy hodnoty tahu */
extern void ch_gen_costs(ch_board *board);

/* automaticky hrac */
extern tree *ch_gen_play(ch_board *board, char *s, char *t);

#endif
