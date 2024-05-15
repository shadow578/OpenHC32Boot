#pragma once

#define BOARD_VOXELAB_V101 1000
#define BOARD_CREALITY_E2P24S4 2000

#define IS_BOARD(board) (BOARD == BOARD_##board)
