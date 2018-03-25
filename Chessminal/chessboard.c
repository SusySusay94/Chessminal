//
//  chessboard.c
//  Chessminal
//
//  Created by Susanna Ardigo' on 22.03.18.
//  Copyright © 2018 Susanna Ardigo'. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "chessboard.h"

// ############### EXTRA DECLARATIONS ###############
struct move {
    char col;
//    char row;
    int row;
};

char* get_input(char * input, int len);
int check_input(char * input, char * check);
enum mstatus move_piece(struct chessboard * cb, enum player p, const char * from, const char * to);


// ############### MAIN ###############
int main(int argc, const char * argv[]) {
    struct chessboard cb;
    init_chessboard(&cb);
    
    int game_on = 1;
    

    
    enum player current_player = WHITE;
    char* input = malloc(7*sizeof(char));
    do {
        print_chessboard(&cb);
        
        if (current_player == WHITE) printf("w: ");
        else printf("b: ");
        
        input = get_input(input, 6);
        
//        printf("\ninput: %s", input);
            // check draw and resign
        if (check_input(input, "draw")) {
            if (current_player == WHITE)
                printf("\nPlayer %c wants to draw, to accept type 'draw'\n", 'w');
            else printf("\nPlayer %c wants to draw, to accept type 'draw'\n", 'b');
            
            
            input = get_input(input, 6);
            
            if (!strcmp(input, "draw")) {
                printf("Game ended with DRAW\n");
                
                game_on = 0;
                break;
            }
            else {
                printf("Draw not accpeted, please enter a move\n");
                continue;
            }
            
        }
        
        if (check_input(input, "resign"))
            game_on = 0;
        
        // do move
//        printf("%s\n", input);
        int valid_input = 1;
        for (int i=0; i<6; ++i) {
//            printf("%c\n",input[i]);
            if ((i==0 || i==3) && ((int) input[i] < 97 || (int) input[i] > 104)) {
                valid_input = 0;
                break;
            }
            else if ((i==1 || i==4) && ((int) input[i] < 48 || (int) input[i] > 57)) {
                valid_input = 0;
                break;
            }
            else if ((i==2) && (int) input[i] != 32 ) {
                valid_input = 0;
                break;
            }
            else if ((i==5) && ((int) input[i] != 0 && ((int) input[i] != 32))) {
                    valid_input = 0;
                    break;
            }
        }
        if (!valid_input) {
            printf("Invalid input, please insert a valid move");
            continue;
        }

//        const char * from = strsep(&input, " ");
//        const char * to  = strsep(&input, " ");
//        free(input);
        enum mstatus status = move_piece(&cb, current_player, &input[0], &input[3]);
        printf("the move is: %d\n", status);
        
        // switch player
        if (current_player== WHITE) current_player = BLACK;
        else current_player = WHITE;
//        current_player = (++current_player % 2);
        
    }
    while (game_on);
    
    return 0;
}


// ############### CHESSBOARD ###############

/* print the current position of the pieces on the board */
void print_chessboard(struct chessboard * cb) {
    printf("\n%s\n", "      a   b   c   d   e   f   g   h");
    printf("%s\n", "    ┌───┬───┬───┬───┬───┬───┬───┬───┐");
    for (int i=0; i < 8; ++i) {
        printf(" %d  |", 8-i);
        for (int j=0; j<8; ++j) {
            printf(" %s |",  utf_8_pieces[cb->position[i][j]]);
        }
        if (i!=7) {
            printf("\n%s\n", "    ├───┼───┼───┼───┼───┼───┼───┼───┤");
        }
        else {
            printf("%s","\n");
        }
        
    }
    printf("%s\n", "    └───┴───┴───┴───┴───┴───┴───┴───┘");
    printf("%s\n", "      a   b   c   d   e   f   g   h");
    
}

/* set the initial position of the pieces on the board */
void init_chessboard(struct chessboard * cb) {
    cb->position[0][0] = BLACK_ROOK;
    cb->position[0][1] = BLACK_KNIGHT;
    cb->position[0][2] = BLACK_ROOK;
    cb->position[0][3] = BLACK_BISHOP;
    cb->position[0][4] = BLACK_QUEEN;
    cb->position[0][5] = BLACK_KING;
    cb->position[0][6] = BLACK_KNIGHT;
    cb->position[0][7] = BLACK_ROOK;
    
    for (int i = 0; i < 8; ++i) {
        cb->position[1][i] = BLACK_PAWN;
        
        cb->position[2][i] = EMPTY;
        cb->position[3][i] = EMPTY;
        cb->position[4][i] = EMPTY;
        cb->position[5][i] = EMPTY;
        
        cb->position[6][i] = WHITE_PAWN;
    }
    
    cb->position[7][0] = WHITE_ROOK;
    cb->position[7][1] = WHITE_KNIGHT;
    cb->position[7][2] = WHITE_ROOK;
    cb->position[7][3] = WHITE_BISHOP;
    cb->position[7][4] = WHITE_QUEEN;
    cb->position[7][5] = WHITE_KING;
    cb->position[7][6] = WHITE_KNIGHT;
    cb->position[7][7] = WHITE_ROOK;
    
}

// ####### MOVE FUNCTIONS #######
enum mstatus move_piece(struct chessboard * cb, enum player p, const char * from, const char * to) {
    struct move move_from = {from[0], (int) from[1] - '0'};
    struct move move_to = {to[0], (int) to[1] - '0'};
    printf("from: %c%d, to:%c%d\n", move_from.col, move_from.row, move_to.col, move_to.row);
    
    return INVALID;
}



// ####### OTHER HELPER FUNCTIONS #######

/* read the input */
char* get_input(char * input, int len) {
    char c;
    int i =0;
    while ((c=getchar()) !=  '\n') {
        if (i>len-1)
            continue;
        input[i] = c;
        ++i;
    }
    while (i < len) {
        input[i] = (char) 0;
        ++i;
    }
    return input;
}
/* checks if the input is equal to a string */
int check_input(char * input, char * check) {
    int len = 6;
    if (!strcmp(check, "draw"))
        len = 4;
    for (int i=0; i<len; ++i) {
        //    printf("%c = %c\n", input[i], draw[i]);
        if (input[i] != check[i])
            return 0;
    }
    return 1;
}



