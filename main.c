#include <stdlib.h>
#include <time.h>

#include "chess.h"
#include "io.h"

int main(int argc, char **argv)
{
	ch_board board;

	ch_board_init(&board);
	
/*	ch_board_print_center_table(&board);
	ch_error_print();
	ch_board_print_figure_place(&board);
	ch_error_print();
	ch_board_print_figure_value(&board);
	ch_error_print(); */

	srandom(time(NULL));
	ch_game_start(&board);
	ch_error_print();
	
	ch_board_done(&board);

	return 0;
}
