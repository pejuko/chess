#ifndef __CH_STDDEF_H__
#define __CH_STDDEF_H__

/* Id of figures */
#define CH_VAKAT 0
#define CH_PAWN 1
#define CH_BLACK_PAWN -1
#define CH_KNIGHT 2
#define CH_BLACK_KNIGHT -2
#define CH_BISHOP 3
#define CH_BLACK_BISHOP -3
#define CH_ROOK 4
#define CH_BLACK_ROOK -4
#define CH_QUEEN 5
#define CH_BLACK_QUEEN -5
#define CH_KING 6
#define CH_BLACK_KING -6
#define CH_BORDER 100

/* Values of figures */
#define PAWN 100
#define BLACK_PAWN -100
#define KNIGHT 330
#define BLACK_KNIGHT -330
#define BISHOP 330
#define BLACK_BISHOP -330
#define ROOK 520
#define BLACK_ROOK -520
#define QUEEN 980
#define BLACK_QUEEN -980
#define KING 12000
#define BLACK_KING -12000

/* Values of vakats */
#define CH_A1 21
#define CH_A2 31
#define CH_A3 41
#define CH_A4 51
#define CH_A5 61
#define CH_A6 71
#define CH_A7 81
#define CH_A8 91
#define CH_B1 22
#define CH_B2 32
#define CH_B3 42
#define CH_B4 52
#define CH_B5 62
#define CH_B6 72
#define CH_B7 82
#define CH_B8 92
#define CH_C1 23
#define CH_C2 33
#define CH_C3 43
#define CH_C4 53
#define CH_C5 63
#define CH_C6 73
#define CH_C7 83
#define CH_C8 93
#define CH_D1 24
#define CH_D2 34
#define CH_D3 44
#define CH_D4 54
#define CH_D5 64
#define CH_D6 74
#define CH_D7 84
#define CH_D8 94
#define CH_E1 25
#define CH_E2 35
#define CH_E3 45
#define CH_E4 55
#define CH_E5 65
#define CH_E6 75
#define CH_E7 85
#define CH_E8 95
#define CH_F1 26
#define CH_F2 36
#define CH_F3 46
#define CH_F4 56
#define CH_F5 66
#define CH_F6 76
#define CH_F7 86
#define CH_F8 96
#define CH_G1 27
#define CH_G2 37
#define CH_G3 47
#define CH_G4 57
#define CH_G5 67
#define CH_G6 77
#define CH_G7 87
#define CH_G8 97
#define CH_H1 28
#define CH_H2 38
#define CH_H3 48
#define CH_H4 58
#define CH_H5 68
#define CH_H6 78
#define CH_H7 88
#define CH_H8 98

/* Colors */
/* CH_VAKAT == 0 */
#define CH_WHITE 1
#define CH_BLACK -1
/* CH_BORDER == 100 */
#define CH_GET_COLOR(x) (((x)<0)?CH_BLACK:CH_WHITE)

/* Move */
#define CH_DIAG 1
#define CH_VH 2
#define CH_KM 4
#define CH_PM 8
#define CH_WHITE_BASE_LINE 30
#define CH_BLACK_BASE_LINE 80
#define CH_WHITE_CHANGE_LINE 90
#define CH_BLACK_CHANGE_LINE 20

/* Boolean */
#define CH_TRUE 1
#define CH_FALSE 0

/* Flags */
#define CH_TEST 0
#define CH_DO_IT 1
#define CH_HUMAN 0
#define CH_CPU 1

#endif
