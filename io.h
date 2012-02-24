#ifndef __CH_IO_H__
#define __CH_IO_H__

#include "chess.h"

#define CH_MAX_MOV_LEN 80

/* precte ze stdin radek a rozhodne zda se jedna o platny vstup */
extern char *ch_get_move(char *src, char *trg);

/* vytiskne sachovnici, na miste figur je jejich hodnota */
extern void ch_board_print_figure_place(ch_board* board);

/* vytiskne mozne tahy */
extern void ch_print_moves(ch_board* board);

/* funkce podavajici chybove hlaseni */
extern void ch_print_error(void);


/* Nasledujici funkce jsou vhodne spise pro ladici ucely */

/* vypise hodnoty samotnych hracich poli */
extern void ch_board_print_center_table(ch_board *board);

/* vypise jednoduchou tabulku hracich poli */
/* kde je figurka, tak vypise jejih hodnotu */
extern void ch_board_print_figure_value(ch_board *board);

/* vypise typy figur cernych a bilych figur */
/* vhodne pro otestovani spravne inicializace */
extern void ch_board_figure_print(ch_board *board);

#endif
