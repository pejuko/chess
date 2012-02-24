#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "draggen.h"
#include "ch_stddef.h"
#include "list.h"
#include "io.h"

char ch_offset[] = {
	-9, -11, 9, 11,	/* 0, 1, 2, 3, diagonal move */
	-1, 10, 1, -10,	/* 4, 5, 6, 7, horizontal and vertical move */
	19, 21, 12, -8, -19, -21, -12, 8, /* 8, 9, 10, 11, 12, 13, 14, 15 knight */
	/* part only for pawn */
	10,	/* 16 white pawn -- normal */
	20, /* 17 white pawn -- only if position equal CH_WHITE_BASE_LINE */
	11, /* 18 */
	9, /* 19 white pawn -- take alien figure */
	-10,	/* 20 black pawn -- normal */
	-20, /* 21 black pawn -- only if position equal CH_BLACK_BASE_LINE */
	-11, /* 22 */
	-9 /* 23 black pawn -- take alien figure */
};

/* oznaci ohrozena/neohrozena pole */
void ch_gen_dangers(ch_board *board)
{
	int i;
	ch_figure *figure=NULL;
	
	if (board==NULL || board->node==NULL) {
		ch_error=CH_NULL_POINTER;
		return;
	}

	/* prirazeni pole figur, ktere jsou na tahu */
	figure = (board->on_move==CH_WHITE) ? board->white : board->black;

	/* vynulovani aktualniho stavu at je jakykoliv */
	for (i=0; i<120; i++)
		board->board[i].danger = 0;
	
	board->node->actual = board->node->first;
	while (board->node->actual != NULL) {
		board->board[(int)((ch_move*)(board->node->actual->data))->trg].danger = board->on_move;
		board->node->actual = board->node->actual->next;
	}
}

/* generuje pro danou pozici vsechny mozne tahy */
void ch_gen_moves(ch_board *board)
{
	int start=0, end=0, i=0, j=0; /* pocitadla */
	char typ=0;	/* typ figury */
	ch_figure *ret=NULL;	/* ukazatel na branou figuru */
	ch_figure *figure=NULL;	/* pole figur barvy, ktera je na tahu */
	ch_move *move=NULL;	/* pom prom pro vytvoreni noveho tahu */
	

	ch_error=CH_NO_ERRORS;

	if (board==NULL || board->node==NULL) {
		ch_error=CH_NULL_POINTER;
		return;
	}
	/* jestli bylo pro tuto uroven uz nekdy driv generovano
	 * nema smysl znovu generovat */
	if (board->node->first != NULL)
		return;
#ifdef DEBUG
	printf("Searching for ``legal'' moves for %d\n", board->on_move);
#endif
	/* prirazeni pole figur, ktere jsou na tahu */
	figure = (board->on_move==CH_WHITE) ? board->white : board->black;

	/* pro vsechny figury v poli */
	for (i=0; i<16; i++) {
		int multi=1; /* pocitadlo kroku */
		
		/* byla li figura vyhozena tak pokracuj s dalsi */
		if (board->board[(int)figure[i].position].value == CH_BORDER)
			continue;

		/* zjisti typ figury (nezavisle na barve) */
		typ = (figure[i].type >= 0) ? figure[i].type : (-1*figure[i].type);
		/* zacatek v offsetove tabulce tahu */
		start = (typ==CH_QUEEN || typ==CH_KING || typ==CH_BISHOP) ? 0 :
				((typ==CH_ROOK) ? 4 : ((typ==CH_KNIGHT) ? 8 : 0));
		/* konec v offsetove tabulce tahu */
		end = (typ==CH_QUEEN || typ==CH_KING || typ==CH_ROOK) ? 7 :
				((typ==CH_BISHOP) ? 3 : ((typ==CH_KNIGHT) ? 15:0));
		
		/* nejedna-li se o pesce */
		if (typ!=CH_PAWN) {
			/* pro vsechny tahy prislusejici dane figure */
			for (j=start; j<=end; j++) {
				int multiply = figure[i].multiply;	/* muze delat figura vice jak jeden krok? */
				multi=1;	/* budeme provadet prvni krok */
				ch_error=CH_NO_ERRORS;
				
				/* delej dokud muze figura delat dalsi korky, nebo nedojde k chybe */
				do {
					/* otestuj jestli jde tahnout, tam kam chceme a zjisti branou figuru */
					ret = ch_figure_move(board->board, figure[i].position,
							figure[i].position + ch_offset[j]*multi, CH_TEST);
					/* pokud tam lze tahnout */
					if (ch_error==CH_NO_ERRORS) {
						/* vytvorime novy tah */
						move = (ch_move*)malloc(sizeof(ch_move));
						if (move==NULL) {
							ch_error=CH_NOT_MEM;
#ifdef DEBUG
							printf("\n\nIn ch_gen_moves();\nTyp: %d\nmove == NULL\n\n", typ);
#endif
							return;
						}
						move->src = figure[i].position;
						move->trg = figure[i].position+ch_offset[j]*multi;
						move->change = 0;
						move->take = ret;
						/* provizorni hodnota lepsi oceneni probiha pozdeji */
						move->cost = (int)(figure[i].value) +
							(int)(board->board[(int)move->trg].value);
						/* pridej do aktualni hloubky stromu novy tah */
						tree_push(board->node, (void*)move);
#ifdef DEBUG
						printf("F%dS%dT%dC%d,",typ,move->src, move->trg, move->cost);
#endif
					} else if (ch_error!=CH_ILLEGAL_MOVE) { /* pokud tam nelze tahnout */
#ifdef DEBUG
						printf("\n\nIn ch_gen_moves();\nLine: 68\nTyp: %d\nch_error == %d\n\n", typ, ch_error);
#endif
						return;
					}
					
					/* pokud stoji v ceste nejaka figura, 
					 * tak nelze pokracovat */
					if (ret!=NULL) {
						multiply=CH_FALSE;
					}
					/* chystame se provest dalsi krok */
					multi++;
				} while (multiply==CH_TRUE && ch_error==CH_NO_ERRORS);
			} /* konec vsem tahum prislusne figury */
		} else {	/* jedna-li se o pesce */
			char tmp_pos = figure[i].position; /* pomocna pozicni promena */
			int fw = (figure[i].type>0) ? 16:20,	/* tahni: do predu */
				fw2 = (figure[i].type>0) ? 17:21,	/* 	2x do predu */
				fwt1 = (figure[i].type>0) ? 18:22,	/* 	ber vpravo */
				fwt2 = (figure[i].type>0) ? 19:23,	/*	ber vlevo */
				basln = (figure[i].type>0) ?
					tmp_pos - CH_WHITE_BASE_LINE :
					tmp_pos - CH_BLACK_BASE_LINE;		/* urci zakladnu pescu */

			/* otestuj jestli lze tahnout danym smerem a zjisti branou figuru */
			ret = ch_figure_move(board->board, tmp_pos, tmp_pos+ch_offset[fw], CH_TEST);
			/* jestlize tam lze jit a nic nestoji v ceste */
			if (ch_error==CH_NO_ERRORS && ret==NULL) {
				/* vytvorime novy tah */
				move = (ch_move*)malloc(sizeof(ch_move));
				if (move==NULL) {
					ch_error=CH_NOT_MEM;
					printf("\n\nIn ch_gen_moves();\nTyp: %d\nmove == NULL\n\n", typ);
					return;
				}
				move->src = tmp_pos;
				move->trg = tmp_pos+ch_offset[fw]*multi;
				move->change = 0;
				move->take = ret;
				move->cost = (int)(figure[i].value) +
					(int)(board->board[(int)move->trg].value);
				/* novy tah ulozime do aktualni hloubky stromu */
				tree_push(board->node, (void*)move);
#ifdef DEBUG
				printf("F%dS%dT%dC%d,",typ,move->src, move->trg, move->cost);
#endif
			} else if (ch_error!=CH_NO_ERRORS && ch_error!=CH_ILLEGAL_MOVE) {
				/* doslo-li k nejake podivne chybe */
				printf("\n\nIn ch_gen_moves();\nLine: 100\nTyp: %d\nch_error == %d\n\n", typ, ch_error);
				return;
			}
			
			/* otestujeme jestli neni pesec jeste na zakladni care a muze tahnout */
			if (basln<10 && basln>0 && board->board[tmp_pos+ch_offset[fw]].figure==NULL) {
				/* je mozne tahnout o dva kroky? */
				ret = ch_figure_move(board->board, tmp_pos, tmp_pos+ch_offset[fw2], CH_TEST);
				/* lze a nic nestoji v ceste */
				if (ch_error==CH_NO_ERRORS && ret==NULL) {
					/* novy tah */
					move = (ch_move*)malloc(sizeof(ch_move));
					if (move==NULL) {
						ch_error=CH_NOT_MEM;
						printf("\n\nIn ch_gen_moves();\nTyp: %d\nch_error == %d\nin 0<basln<10\n\n", typ, ch_error);
						return;
					}
					move->src = tmp_pos;
					move->trg = tmp_pos+ch_offset[fw2]*multi;
					move->change = 0;
					move->take = ret;
					move->cost = (int)(figure[i].value) +
						(int)(board->board[(int)move->trg].value);
					/* pridani tahu do stromu na akt. patro */
					tree_push(board->node, (void*)move);
#ifdef DEBUG
					printf("F%dS%dT%dC%d,",typ,move->src, move->trg, move->cost);
#endif
				} else if (ch_error!=CH_NO_ERRORS && ch_error!=CH_ILLEGAL_MOVE) {
					/* doslo k podivne chybe */
					printf("\n\nIn ch_gen_moves();\nTyp: %d\nch_error == %d\nin 0<basln<10 -- second\n\n", typ, ch_error);
					return;
				}
			}
			
			/* taky zkusime jestli muze brat */
			for (j=fwt1; j<=fwt2; j++) {
				ret = ch_figure_move(board->board, tmp_pos, tmp_pos+ch_offset[j], CH_TEST);
				/* je tam nejaka figura? */
				if (ret!=NULL) {
					/* Ano, muze ju vzit! Vytvorime novy tah.*/
					move = (ch_move*)malloc(sizeof(ch_move));
					if (move==NULL) {
						ch_error=CH_NOT_MEM;
						printf("\n\nIn ch_gen_moves();\nTyp: %d\nch_error == %d\nin 0<basln<10 -- second\n\n", typ, ch_error);
						return;
					}
					move->src = tmp_pos;
					move->trg = tmp_pos+ch_offset[j];
					move->change = 0;
					move->take = ret;
					move->cost = (int)(figure[i].value) +
						(int)(board->board[(int)move->trg].value);
					/* Novy tah zaradime */
					tree_push(board->node, (void*)move);
#ifdef DEBUG
					printf("F%dS%dT%dC%d,",typ,move->src, move->trg, move->cost);
#endif
				}
			}
		}
		/* vsechny chyby pro danou figuru by meli byt osetreny, proto */
		/* pri prechodu na dalsi figuru je mozno vynulovat chybovy stav */ 
		ch_error=CH_NO_ERRORS;
	}

#ifdef DEBUG
	ch_board_print_figure_place(board);
#endif
}

/* generuje pro mozne tahy protitahy */
void ch_gen_submoves(ch_board *board)
{
	int best=0;	/* nejlepsi tah */
	ch_move *move=NULL;	/* pom prom pro tah */
	
	/* nechceme pracovat s prazdnejma ukazatelama */
	if (board==NULL || board->node==NULL || board->node->first==NULL) {
		ch_error=CH_NULL_POINTER;
		printf("ch_gen_submoves: null pointer %s\n", (board) ? ((board->node) ? "board->node->first" : "board->node") : "board");
		return;
	}

	/* nejdrive si ocenime jednotlive tahy */
	ch_gen_costs(board); 

	/* zmenime docasne stranu */
	board->on_move = (board->on_move == CH_WHITE) ? CH_BLACK : CH_WHITE;

	/* vezmeme prvni mozny tah */
	board->node->actual = board->node->first;

	/* odsimulujeme vsechny mozne tahy */
	while (board->node->actual != NULL) {
		/* provedem tah ve stromu */
		board->node = board->node->actual;
		/* zjistime co mame provest za tah na sachovnici */
		move = (ch_move*)board->node->data;
		/* provedeme tah, netreba zjistovat, co berem
		 * to uz zjistil generator, ktery tento tah vygeneroval */
		ch_figure_move(board->board, move->src, move->trg, CH_DO_IT);
		/* pro provedeny tah vygenerujeme vsechny mozne protitahy */
		ch_gen_moves(board);
		/* upravime hodnoty tahu */
		ch_gen_costs(board);

		if (board->node->first == NULL) {
			/* spoluhrac nema protitah -- vyhrali jsme! */
			break;
		}

		/* vyhleda tah s nejnizsi hodnotou
		 * -- predpokladame, ze nepritel tahne nejlip jak muze */
		board->node->actual = board->node->first;
		best = ((ch_move*)board->node->actual->data)->cost;
		while (board->node->actual != NULL) {
			if (best < ((ch_move*)board->node->actual->data)->cost)
				best = ((ch_move*)board->node->actual->data)->cost;
			board->node->actual = board->node->actual->next;
		}
		
		/* undo */
/*		ch_board_undo(board); */
		ch_figure_move(board->board, move->trg, move->src, CH_DO_IT);
		/* jestlize jsme neco brali, je treba figuru znova umistit */
		if (move->take != NULL) {
			move->take->position = move->trg;
			board->board[(int)move->take->position].figure = move->take;
		}
		/* vratime se na puvodni uroven stromu */
		board->node = board->node->root;
		
		/* nalezenou hodnotu zapiseme jako cenu tahu */
		((ch_move*)board->node->actual->data)->cost += -1*best;
		
		/* dalsi tah */
		board->node->actual = board->node->actual->next;
	}

	/* navrat k puvodni strane */
	board->on_move = (board->on_move == CH_WHITE) ? CH_BLACK : CH_WHITE;

	/* protoze board->node->actual je nyni NULL, tak ho nastavime na prvni tah */
	board->node->actual = board->node->first;
/*	ch_board_print_figure_place(board); */
}

/* provadi bilanci hry */
void ch_gen_cost(ch_board *board)
{
	int i=0;
	
	if (board==NULL) {
		ch_error=CH_NULL_POINTER;
		return;
	}
	
	/* vynulujem stare hodnoty */
	board->wcost = 0;
	board->bcost = 0;
	
	/* postupne nacteme aktualni stav do promenych */
	for (i=0; i<16; i++) {
		if (board->white[i].position != 0)
			board->wcost += board->white[i].value +
				CH_WHITE * board->board[(int)board->white[i].position].value;
		if (board->black[i].position!=0)
			board->bcost += board->black[i].value +
				CH_BLACK * board->board[(int)board->black[i].position].value;
	}

	/* rozdil hodnot (bcost je zaporne cislo) */
	board->cost = board->wcost + board->bcost;

#ifdef DEBUG
	printf("\nWhite cost: %d\nBlack cost: %d\nCost: %d\n", board->wcost, board->bcost, board->cost);
#endif
}

/* urci pro tahy hodnoty tahu */
void ch_gen_costs(ch_board *board)
{
	tree *node=NULL;	/* pomocny ukazatel do stromu */
	ch_move* move=NULL;	/* pomocny ukazatel na tah */
	int color=0;		/* barva hrace */
	ch_field *b=NULL;	/* ukazatel na hraci pole */

	if (board==NULL || board->node==NULL || board->node->first==NULL ||
			board->node->first->data == NULL) {
		ch_error=CH_NULL_POINTER;
		printf("ch_gen_costs: %s\n", (board) ? ((board->node) ? "board->node->first" : "board->node") : "board");
		return;
	}

	node = board->node->first;
	b = board->board;

	color = board->on_move;

	/* dokud neprojdes vsechny tahy */
	while (node!=NULL) {
		move = (ch_move*)(node->data);
		/* hodnota ciloveho pole - hodnota zdrojoveho pole = zisk (nebo strata) */
		move->cost = b[(int)(move->trg)].value - b[(int)(move->src)].value;
		/* budes-li brat figuru tak je treba pricist jeste jeji hodnotu */
		if (b[(int)(move->trg)].figure != NULL) {
			/* bila ma hodnotu kladnou a cerna zapornou */
			/* ja mam jinou barvu nez protivnik, proto zneguji svoji barvu */
			move->cost += (-1*color) * b[(int)(move->trg)].figure->value;
		}
#ifdef DEBUG
		printf("F%dS%dT%dC%d,",b[(int)(move->src)].figure->type,move->src, move->trg, move->cost);
#endif
		/* dalsi tah na holeni, prosim :-) */
		node = node->next;
	}
}

/* automaticky hrac */
tree *ch_gen_play(ch_board *board, char *s, char *t)
{
	tree *node=NULL; /* pomocny ukazatel do stromu */
	tree *best=NULL; /* uzel s nejlepsim tahem */
	
	if (board==NULL || board->node==NULL || board->node->first==NULL) {
		ch_error=CH_NULL_POINTER;
		return best;
	}

	node = board->node;
	node->actual = node->first;	/* zacnem prvnim tahem */
	best = node->actual;
	
	/* projdem vsechny tahy na seznamu */
	while (node->actual != NULL) {
		/* tak, co, kdo je lepsi? :-) */
		if (((ch_move*)(best->data))->cost < ((ch_move*)(node->actual->data))->cost) {
			best = node->actual;
		} else if (((ch_move*)(best->data))->cost == ((ch_move*)(node->actual->data))->cost &&
				random()%2 == 0) {
			best = node->actual;
		}
		node->actual = node->actual->next;
	}
	/* nenechame ukazatel zbytecne ukazovat na NULL */
	board->node->actual = node->first;
	/* byl to prazdnej seznam? */
	if (best==NULL) {
		*s=0;
		*t=0;
	} else {
		/* nastav vyvolene souradnice */
		*s = ((ch_move*)(best->data))->src;
		*t = ((ch_move*)(best->data))->trg;
	}

	return best;
}
