#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "io.h"
#include "chess.h"
#include "ch_stddef.h"
#include "draggen.h"
#include "tree.h"

/* promena chyby */
ch_eerror ch_error=CH_NO_ERRORS;

/* inicializace struktury ch_board */
ch_board *ch_board_init(ch_board *board)
{
	int i=0, j=0;
	
	ch_error=CH_NO_ERRORS;

	/* jeslize neni struktura vytorena, tak jednu vytvor */
	if (board==NULL)
		if ((board=malloc(sizeof(ch_board))) == 0) {
			ch_error=CH_NOT_MEM;
			return NULL;
		}

	/* inicializujem vsechny hranice jako by to byly zdi */
	for (i=0; i<120; i++) {
		board->board[i].figure = NULL;
		board->board[i].value = CH_BORDER;
		board->board[i].danger = 0;
	}

	/* vytvor prazdou hraci plochu */
	for (j=20; j<100; j+=10)
		for (i=1; i<9; i++)
			board->board[j+i].value = CH_VAKAT;

	/* vytvor strom */
	board->node = tree_new(malloc(sizeof(ch_move)));
	if (board->node == NULL)
		return board;
	if (board->node->data == NULL){
		free(board->node);
		return board;
	}
	((ch_move*)board->node->data)->src = 0;
	((ch_move*)board->node->data)->trg = 0;
	((ch_move*)board->node->data)->cost = 0;
	((ch_move*)board->node->data)->change = 0;

	/* inicializace figur */
	for (i=0; i<8; i++) {
		ch_figure_init(board->white[i], CH_PAWN, PAWN, CH_A2+i, CH_PM, CH_FALSE);
		ch_figure_init(board->black[i], CH_BLACK_PAWN, BLACK_PAWN, CH_A7+i, CH_PM, CH_FALSE);
	}
	ch_figure_init(board->white[8], CH_ROOK, ROOK, CH_A1, CH_VH, CH_TRUE);
	ch_figure_init(board->white[9], CH_ROOK, ROOK, CH_H1, CH_VH, CH_TRUE);
	ch_figure_init(board->black[8], CH_BLACK_ROOK, BLACK_ROOK, CH_A8, CH_VH, CH_TRUE);
	ch_figure_init(board->black[9], CH_BLACK_ROOK, BLACK_ROOK, CH_H8, CH_VH, CH_TRUE);
	ch_figure_init(board->white[10], CH_KNIGHT, KNIGHT, CH_B1, CH_KM, CH_FALSE);
	ch_figure_init(board->white[11], CH_KNIGHT, KNIGHT, CH_G1, CH_KM, CH_FALSE);
	ch_figure_init(board->black[10], CH_BLACK_KNIGHT, BLACK_KNIGHT, CH_B8, CH_KM, CH_FALSE);
	ch_figure_init(board->black[11], CH_BLACK_KNIGHT, BLACK_KNIGHT, CH_G8, CH_KM, CH_FALSE);
	ch_figure_init(board->white[12], CH_BISHOP, BISHOP, CH_C1, CH_DIAG, CH_TRUE);
	ch_figure_init(board->white[13], CH_BISHOP, BISHOP, CH_F1, CH_DIAG, CH_TRUE);
	ch_figure_init(board->black[12], CH_BLACK_BISHOP, BLACK_BISHOP, CH_C8, CH_DIAG, CH_TRUE);
	ch_figure_init(board->black[13], CH_BLACK_BISHOP, BLACK_BISHOP, CH_F8, CH_DIAG, CH_TRUE);
	ch_figure_init(board->white[14], CH_QUEEN, QUEEN, CH_D1, CH_DIAG|CH_VH, CH_TRUE);
	ch_figure_init(board->white[15], CH_KING, KING, CH_E1, CH_DIAG|CH_VH, CH_FALSE);
	ch_figure_init(board->black[14], CH_BLACK_QUEEN, BLACK_QUEEN, CH_D8, CH_DIAG|CH_VH, CH_TRUE);
	ch_figure_init(board->black[15], CH_BLACK_KING, BLACK_KING, CH_E8, CH_DIAG|CH_VH, CH_FALSE);

	/* umist figury */
	for (i=0; i<16; i++) {
		ch_figure_set(board->board, &board->white[i]);
		ch_figure_set(board->board, &board->black[i]);
	}

	/* nastav hodnoty jednotlivych bunek desky pro stredovou hru */
	i=CH_A1;
	board->board[i++].value = 4;
	board->board[i++].value = 0;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	board->board[i++].value = 0;
	board->board[i++].value = 4;
	i=CH_A2;
	board->board[i++].value = 4;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	board->board[i++].value = 4;
	i=CH_A3;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 20;
	board->board[i++].value = 20;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	i=CH_A4;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 20;
	board->board[i++].value = 24;
	board->board[i++].value = 24;
	board->board[i++].value = 20;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	i=CH_A5;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 20;
	board->board[i++].value = 24;
	board->board[i++].value = 24;
	board->board[i++].value = 20;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	i=CH_A6;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 20;
	board->board[i++].value = 20;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	i=CH_A7;
	board->board[i++].value = 4;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	board->board[i++].value = 4;
	i=CH_A8;
	board->board[i++].value = 4;
	board->board[i++].value = 0;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	board->board[i++].value = 0;
	board->board[i++].value = 4;

	/* na zacatku nema sach nikdo */
	board->check = 0;

	/* prvni bude na tahu bila */
	board->on_move = CH_WHITE;

	board->wp = CH_HUMAN;	/* za bily bude hrat clovek */
	board->bp = CH_CPU;		/* za cerny pocitac */
	board->pl = board->wp;	/* na tahu je bily */

	board->move = 1;		/* prvni tah */

/*	ch_gen_cost(board);*/

	return board;
}

/* konec hry -- zrusi strom */
void ch_board_done(ch_board *board)
{
	if (board==NULL)
		return;
	/* kdyz strom existuje */
	if (board->node != NULL) {
		/* vyhledej koren */
		while (board->node->root!=NULL)
			board->node=board->node->root;
		/* smaz vsechny vetve */
		tree_del(board->node);
		board->node=NULL;
	}
}

/* zacatek hry */
void ch_game_start(ch_board *board)
{
	char src=0, trg=0; /* odkud kam */
	char *str=NULL;	/* prikaz */
	int end=0;	/* stav urcujici jestli ma cenu jeste hrat nebo ne */
	int found=CH_FALSE;	/* jestli byl tah nalezen v seznamu moznych tahu */
	
	if (board==NULL || board->node==NULL) {
		ch_error=CH_NULL_POINTER;
		return;
	}
	
	/* zobrazime sachovnici */
	ch_board_print_figure_place(board);

	/* dokud ma cenu hrat */
	while (!end) {
		found=CH_FALSE;
		
		/* uzivatl v predchozi iteraci cyklu mohl zadat neplatny vstup/tah */
		if (ch_error==CH_NO_ERRORS) {
			/* vygeneruj vsechny mozne tahy pro danou pozici */
			ch_gen_moves(board);
			/* zjisti jak jsme na tom */
			ch_gen_cost(board);
			/* generuj dalsi urovne */
			ch_gen_submoves(board);
/*			ch_gen_costs(board); */
			ch_gen_dangers(board);
		}
		
		/* tah se bude cist ze vstupu */
		if (board->pl == CH_HUMAN) {
			str = ch_get_move(&src, &trg);
		} else {
			/* tah se bude pocitat. odkud kam se ulozi do src, trg
			 * a jako navratova hodnota je odkaz na dany tah ve stromu */
			board->node->actual = ch_gen_play(board, &src, &trg);
			/* nepodarilo se neco rozumneho vygenerovat */
			if (board->node->actual != NULL) {
				/* generujeme jenom platne tahy,
				 * proto neni potreba dale overovat platnost tahu */
				found=CH_TRUE;
			} else {
				/* kdyz se nepodarilo nic vygenerovat, tak sme prohrali */
				end=1;
				continue;
			}
		}
		
		/* konec vstupu nebo hrac nama jak hrat (prohral) */
		if (ch_error==CH_END_OF_FILE || board->node->first==NULL) {
			end=1;
			continue;
		} else if (ch_error!=CH_NO_ERRORS) {
			/* dosloli k nejake chybe, tak ji vypiseme */
			ch_error_print();
			continue;
		}
		
		/* na vstupu nebyl tah, ale prikaz */
		if (str!=NULL) {
			if (strcmp(str,"quit\n") == 0) {
				end = 1;
				ch_error=CH_NO_ERRORS;
			} else if (strcmp(str,"moves\n") == 0) {
				ch_print_moves(board);
			} else if (strcmp(str,"undo\n") == 0) {
				ch_board_undo(board);
				ch_board_undo(board);
				ch_board_print_figure_place(board);
			}
			/* odstran vygenerovane tahy at se nageneruji znovu */
			tree_del_nodes(board->node);
			continue;
		}
		
		/* jestlize tah pocitan automaticky */
		if (found!=CH_TRUE)
			/* nastav prvni tah */
			board->node->actual = board->node->first;
		/* budeme hledat, jestli je pozadovany tah na seznamu */
		while (found==CH_FALSE && board->node->actual!=NULL) {
			/* tohle je neco divneho, tak to radsi preskoc */
			if (board->node->actual->data == NULL) {
				board->node->actual = board->node->actual->next;
				continue;
			}
			/* vlastni test */
			if ((((ch_move*)(board->node->actual->data))->src == src) &&
				(((ch_move*)(board->node->actual->data))->trg == trg))
				found=CH_TRUE;
			else	/* kdyz to neni von, tak dalsi tah */
				board->node->actual = board->node->actual->next;
		}
		
		/* jestlize tah nebyl na seznamu */
		if (found==CH_FALSE) {
			board->node->actual = board->node->first;
			ch_error=CH_ILLEGAL_MOVE;
		} else {	/* tah byl na seznamu, tak nastavim pozici ve strome */
			tree *pom=NULL;
			/* uschovej nalezeny tah ze stromu */
			pom = tree_rem(board->node->actual);
			/* zrus ostatni tahy ze stromu -- zbytecne zabiraji pamet */
			tree_del_nodes(board->node);
			/* opet vloz uschovany tah */
			tree_add_first(board->node, pom);
			/* proved posum aktualniho patra na provedeny tah */
			board->node = pom;
			/* odmazej vsechny vetve daneho tahu -- budeme je generovat znovu */
/*			tree_del_nodes(board->node);*/
			found=CH_FALSE;
		}
		
		/* pokud doted nastala nejaka chyba a nebyla vyresena, tak ji vypis */
		if (ch_error!=CH_NO_ERRORS) {
			if (str==NULL && ch_error==CH_END_OF_FILE) {
				end=1;
			}
			ch_error_print();
		} else {
			/* kdyz nenastala zadna chyba, tak se jedna o platny tah a provedeme ho*/
			ch_figure_move(board->board, src, trg, CH_DO_IT);
			/* zmena barvy, ktera je na tahu */
			board->on_move = (board->on_move==CH_WHITE) ? CH_BLACK : CH_WHITE;
			/* zmena hrace, ktery je na tahu */
			board->pl = (board->on_move == CH_WHITE) ? board->wp : board->bp;
			/* pokud pri presunu doslo k nejake chyba, tak vypis k jake */
			if (ch_error!=CH_NO_ERRORS) {
				ch_error_print();
			} else {
				/* tah probehl uspesne, proto je mozno vykreslit novou sachovnici */
				ch_board_print_figure_place(board);
				/* kdyz dojde k chybe, tak to vypise k jake jinak nic */
				ch_error_print();
			}
		}
		
		/* prechazime k dalsimu pultahu */
		board->move++;
	}
}

/* tah figurou */
inline ch_figure* ch_figure_move(ch_field board[120], char src, char target, int flag)
{
	ch_figure *tmp=NULL;
	ch_figure *figure=NULL;
	ch_figure* ret=NULL;	/* navratova hodnota -- brana figura */
	char s;	/* uschovana zdrojova pozice */

	ch_error=CH_NO_ERRORS;
	if (board==NULL || board[(int)src].figure==NULL) {
		ch_error=CH_NULL_POINTER;
		return ret;
	}
	
	/* zapis si co tam stoji za figuru */
	figure = board[(int)src].figure;
	/* uschovej puvodni pozici */
	s = figure->position;
	/* nastav pozici k presunu */
	figure->position = target;

	/* otestuje jestli kral nechce tahnout na ohrozene pole */
	if (figure->type==CH_KING || figure->type==CH_BLACK_KING) {
		if (board[(int)target].danger==0 ||
			(board[(int)target].danger<0 && figure->type<0) ||
			(board[(int)target].danger>0 && figure->type>0)) {
			tmp = ch_figure_set(board, figure);
		} else {
			ch_error=CH_ILLEGAL_MOVE;
			figure->position = s;
			return ret;
		}
	} else {
		tmp = ch_figure_set(board, figure);
	}
	
	/* pokud doslo k tahu mimo hraci desku */
	if (tmp==NULL) {
		ch_error=CH_ILLEGAL_MOVE;
		/* obnoveni pozice */
		figure->position = s;
		return ret;
	}
	/* kdyz tam neco je a nejsem to ja :) -- tedy nedoslo k presunu */
	if (tmp!=figure) {
		/* je to moje figura? */
		if ((tmp->type>0 && figure->type>0) || (tmp->type<0 && figure->type<0)) {
			/* byla to moje figura */
			ch_error=CH_ILLEGAL_MOVE;
			figure->position = s;
			return ret;
		} else {
			ret=tmp;
			if (flag & CH_DO_IT) { /* nebyla to moje figura, ale ma se opravdu sebrat? */
				tmp->position = 0;	/* vyhod nepritelovu figuru */
			}	/* jinak se jedna jenom o test */
		}
	}
	
	/* melo se opravdu tahnout? */
	if (flag & CH_DO_IT) {
		/* melo, proto proved dokonceni presouvaciho manevru */
		board[(int)target].figure = figure;	/* pole na hraci desce rika ze obsahuje figuru */
		board[(int)s].figure = NULL;	/* puvodni pole se vyprazdnilo */
	} else {	/* jenom testujeme */
		figure->position = s;	/* obnov pozici */
		if (tmp==figure) {	/* kdyz doslo k presunu */
			board[(int)target].figure = NULL;	/* vycistime cilove pole */
		}
	}

	/* vrat branou figuru */
	return ret;
}

/* vezmi tah zpet */
void ch_board_undo(ch_board *board)
{
	ch_move* move; /* vraceny tah */
	
	if (board==NULL || board->node==NULL || board->node->root==NULL) {
		ch_error=CH_NULL_POINTER;
		return;
	}

	move = (ch_move*)board->node->data;
	/* provet zpetny presun -- musi se povest, protoze pultah pred tim tam stala */
	ch_figure_move(board->board, move->trg, move->src, CH_DO_IT);
	/* pokud se neco bralo */
	if (move->take != NULL) {
		printf("ahoj");
		/* proved vraceni figury */
		move->take->position = move->trg;
		board->board[(int)move->take->position].figure = move->take;
	}

	/* provede navrat ve stromu o uroven zpet */
	board->node = board->node->root;
	/* odmaznem vygenerovane tahy */
	tree_del_nodes(board->node);
}

/*
 * duplikace sachove hry az na strom
ch_board *ch_board_dup(ch_board *board)
{
	ch_board *new = (board==NULL) ? return(NULL) : malloc(sizeof(ch_board));
	int i=0;

	if (new==NULL)
		return new;

	for (i=0; i<120; i++) {
		new->board[i].figure = NULL;
		new->board[i].value = board->board[i].value;
	}

	for (i=0; i<16; i++) {
		new->white[i].type = board->white[i].type;
		new->white[i].value = board->white[i].value;
		new->white[i].position = board->white[i].position;
		new->white[i].move = board->white[i].move;
		new->white[i].multiply = board->white[i].multiply;
		new->black[i].type = board->black[i].type;
		new->black[i].value = board->black[i].value;
		new->black[i].position = board->black[i].position;
		new->black[i].move = board->black[i].move;
		new->black[i].multiply = board->black[i].multiply;
		ch_figure_set(new->board, &new->white[i]);
		ch_figure_set(new->board, &new->black[i]);
	}

	new->on_move = board->on_move;
	new->wp = board->wp;
	new->bp = board->bp;
	new->pl = board->pl;
	new->wcost = board->wcost;
	new->bcost = board->bcost;
	new->cost = board->cost;
	new->node = board->node;

	return new;
}
*/
