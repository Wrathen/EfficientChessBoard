#include <iostream>

#define uchar unsigned char
#define ull unsigned long long
#define boardSize 64

// 0th bit    : white/black
// <1, 3> bits: piece type
enum tile_flags : uchar
{
    WHITE           = 0b0001,
    EMPTY           = 0b0010,
    PAWN            = 0b0100,
    KNIGHT          = 0b0110,
    BISHOP          = 0b1000,
    ROOK            = 0b1010,
    QUEEN           = 0b1100,
    KING            = 0b1110,

    // EASE OF USE
    WPAWN           = WHITE | PAWN,
    WKNIGHT         = WHITE | KNIGHT,
    WBISHOP         = WHITE | BISHOP,
    WROOK           = WHITE | ROOK,
    WQUEEN          = WHITE | QUEEN,
    WKING           = WHITE | KING,
};

// [To-do]: implement
enum game_flags
{
    GAME_ENDED      = 0b0001,
    WHITE_TURN      = 0b0010,
    WHITE_WON       = 0b0100
};

ull* parse(uchar* arr)
{
    ull* board = (ull*)malloc(4 * sizeof(ull));
    
    // 0b0000'0000  =  1byte
    // ull = unsigned long long
    // 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000
    for (size_t i = 0; i < 4; ++i)
    {
        board[i] = 0;
        
        for (size_t j = 0; j < 16; ++j)
        {
            ull num_to_add = arr[i * 16 + j];
            ull shifted_num = num_to_add << (j * 4);
            board[i] |= shifted_num;
            //printf("Adding into ull: %d, the number: %d, shifted num: %llu\n", i, num_to_add, shifted_num);
        }
    }

    return board;
}

void fill_start_position(uchar* arr)
{
    arr[56] = ROOK;  arr[57] = KNIGHT;  arr[58] = BISHOP;  arr[59] = QUEEN;  arr[60] = KING;  arr[61] = BISHOP;  arr[62] = KNIGHT;  arr[63] = ROOK;
    arr[48] = PAWN;  arr[49] = PAWN;    arr[50] = PAWN;    arr[51] = PAWN;   arr[52] = PAWN;  arr[53] = PAWN;    arr[54] = PAWN;    arr[55] = PAWN;
    arr[40] = EMPTY; arr[41] = EMPTY;   arr[42] = EMPTY;   arr[43] = EMPTY;  arr[44] = EMPTY; arr[45] = EMPTY;   arr[46] = EMPTY;   arr[47] = EMPTY;
    arr[32] = EMPTY; arr[33] = EMPTY;   arr[34] = EMPTY;   arr[35] = EMPTY;  arr[36] = EMPTY; arr[37] = EMPTY;   arr[38] = EMPTY;   arr[39] = EMPTY;
    arr[24] = EMPTY; arr[25] = EMPTY;   arr[26] = EMPTY;   arr[27] = EMPTY;  arr[28] = EMPTY; arr[29] = EMPTY;   arr[30] = EMPTY;   arr[31] = EMPTY;
    arr[16] = EMPTY; arr[17] = EMPTY;   arr[18] = EMPTY;   arr[19] = EMPTY;  arr[20] = EMPTY; arr[21] = EMPTY;   arr[22] = EMPTY;   arr[23] = EMPTY;
    arr[8]  = WPAWN; arr[9]  = WPAWN;   arr[10] = WPAWN;   arr[11] = WPAWN;  arr[12] = WPAWN; arr[13] = WPAWN;   arr[14] = WPAWN;   arr[15] = WPAWN;
    arr[0]  = WROOK; arr[1]  = WKNIGHT; arr[2]  = WBISHOP; arr[3]  = WQUEEN; arr[4]  = WKING; arr[5]  = WBISHOP; arr[6]  = WKNIGHT; arr[7] = WROOK;
}

// lowerbound (inclusive), higherbound (exclusive)
size_t random(size_t l, size_t h)
{
    if (l >= h) return l;
    return l + rand() % (h - l);
}

void fill_random_position(uchar* arr)
{
    size_t black_piece_count = random(0, 16);
    size_t white_piece_count = random(0, 16);
    size_t random_index = random(0, 64);

    for (size_t i = 0; i < boardSize; ++i)
    {
        arr[i] = EMPTY;
    }

    for (size_t i = 0; i < black_piece_count; ++i)
    {
        while (arr[random_index] != EMPTY)
        {
            random_index = random(0, 64);
        }

        arr[random_index] = 4 + random(0, 6) * 2;
    }
    
    random_index = random(0, 64);
    for (size_t i = 0; i < white_piece_count; ++i)
    {
        while (arr[random_index] != EMPTY)
        {
            random_index = random(0, 64);
        }

        arr[random_index] = 4 + random(0, 6) * 2;
        arr[random_index] |= WHITE;
    }
}

char* format_piece(tile_flags flags)
{
    char* str = (char*)malloc(2*sizeof(char));
    str[1] = '\0';

    if (flags == EMPTY)
    {
        str[0] = '.';
        return str;
    }

    bool is_white = flags & WHITE;
    if (is_white) flags = (tile_flags)(0b1111'1110 & flags);

    str[0] = flags == ROOK ? 'R' : flags == PAWN ? 'P' : flags == KNIGHT ? 'N' : flags == BISHOP ? 'B' : flags == KING ? 'K' : 'Q';
    str[0] += is_white ? 0 : 32;

    return str;
}

void print_board_ulls(ull* board)
{
    for (int i = 3; i >= 0; --i)
    {
        printf("%llu ", board[i]);
    }
    printf("\n");
}
void print_board(ull* board)
{
    for (int i = 3; i >= 0; --i)
    {
        for (size_t j = 8; j < 16; ++j)
            printf("%s ", format_piece(tile_flags(board[i] >> (j * 4) & (0b1111))));

        printf("\n");

        for (size_t j = 0; j < 8; ++j)
            printf("%s ", format_piece(tile_flags(board[i] >> (j * 4) & (0b1111))));

        printf("\n");
    }
    printf("\n");
}

int main()
{
    /*for (size_t i = 0; i < 1; ++i)
    {
        // Allocate 64 bytes and fill it with starting positions.
        uchar* arr = (uchar*)malloc(boardSize * sizeof(char));
        fill_random_position(arr);

        // Parse it to 4-long, 32 bytes.
        ull* board = parse(arr);

        // Print the board and the parsed numbers.
        print_board(board);
        print_board_ulls(board);
    }*/

    uchar* arr = (uchar*)malloc(boardSize * sizeof(char));
    fill_start_position(arr);

    // Parse it to 4-long, 32 bytes.
    ull* board = parse(arr);

    // Print the board and the parsed numbers.
    print_board(board);
    print_board_ulls(board);

    return 0;
}
