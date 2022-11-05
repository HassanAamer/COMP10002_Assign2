/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Hassan Aamer 1235857
  Dated:     09/10/2021

*/

/* Skeleton program libraries */
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

/* My libraries */
#include <string.h>
#include <ctype.h>
#include <math.h>

/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE 8       // board size
#define ROWS_WITH_PIECES 3 // number of initial rows with pieces
#define CELL_EMPTY '.'     // empty cell character
#define CELL_BPIECE 'b'    // black piece character
#define CELL_WPIECE 'w'    // white piece character
#define CELL_BTOWER 'B'    // black tower character
#define CELL_WTOWER 'W'    // white tower character
#define COST_PIECE 1       // one piece cost
#define COST_TOWER 3       // one tower cost
#define TREE_DEPTH 3       // minimax tree depth
#define COMP_ACTIONS 10    // number of computed actions

/* My #defines ---------------------------------------------------------------*/
#define CELL_SIZE 3 // Cell-size when printing the boardn
#define ROW_SEP_LENGTH ((BOARD_SIZE + 1) + (CELL_SIZE * BOARD_SIZE))
// The row seperator length
#define ACTION_SEP_LENGTH 37 // Length of seperator between actions
#define PLAYER_B 1           // Black player
#define PLAYER_W -1          // White player
#define PIECE_T 1            // Tower piece
#define PIECE_R 0            // Regular Piece
#define PIECE_E -1           // Empty Piece
#define OFF_M 1              // Diagonal spaces for a move
#define OFF_C 2              // Diagonal spaces for a capture
#define PROCESSED_Y 1        // The action being printed is computer generated
#define PROCESSED_N 0        // The action being printed is user entered
/* one type definition from my sample solution -------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE]; // board type

/* My Structs ----------------------------------------------------------------*/

// This struct defines a space, with a coressponding row and column
typedef struct Space
{
    int row;
    int col;
} space_t;

typedef struct TreeNode tree_node_t;
typedef struct ListNode list_node_t;
typedef struct Move move_t;

// This struct defines a node in the minimax tree, it has:
struct TreeNode
{
    board_t board_state; // The current_board state of the node

    struct TreeNode *parent; // The parent_node

    int cost; // The cost of this node -- will be
              // potentially overwritten during minimax

    int depth; // The depth of the node in the tree

    int children_count; // The number of children this node has

    move_t *move; // What move was required to bring the node into
                  // this state.

    list_node_t *children_list; // The first child of this node -- Start of a
                                // linked list
};

struct ListNode
{
    tree_node_t *node; // Points to the node contained in this list_node

    struct ListNode *next; // Pointer to the next list_node (essentially
                           // a pointer to the sibling node)
};

struct Move
{
    board_t *return_board; // This attribute is only ever relevant when
                           // move_t is being returned from the stage 1 funcs
                           // back to stage 0 where it was called
                           // to transfer the board state

    int victory; // This attribute only ever relevant when move_t is being
                 // is being returned. 1 - BLACK WIN, -1 - WHITE win. 0 - NONE

    space_t start; // The start space of the move
    space_t end;   // The end space of the move
};

/* Function prototypes -------------------------------------------------------*/

void initialize_board(board_t board);
void print_game_info(board_t board);
void print_board(board_t board);
void print_col_titles(void);
void print_seperator(void);

int run_game_loop(board_t board);

int col2int(int col);
char int2col(int col);
int count_pieces(board_t board, int player);
int get_piece_type(board_t board, space_t space);
int is_player_piece(board_t board, space_t space, int player);
int switch_player(int player);
int get_player_from_piece(board_t board, space_t space);
int board_cost(board_t board);

int continue_playing(board_t current_board, int player, int action_count);
void process_next_move(board_t current_board, int player, int action_count);
void print_victory(int player);

int scan_errors(board_t board, space_t start, space_t end, int player);
void error_handler(int error_code);
int is_space_bounds(space_t space);

void print_action_info(board_t board, int action_no, int player, space_t start,
                       space_t end, int PROCESSED);
void print_action_seperator();

int process_action(board_t board, space_t start, space_t end, int player);

void process_move(board_t board, space_t start, space_t end);
void execute_move(board_t board, space_t start, space_t end);

int process_capture(board_t board, space_t start, space_t end, int player);
void execute_capture(board_t board, space_t start, space_t end, space_t cap);

void detect_promotion(board_t board, space_t start, space_t end, int player);
void promote_space(board_t board, space_t space);

move_t *compute_next_action(board_t board, int player);
move_t *calculate_next_action(tree_node_t *root, int player);
int minimax(tree_node_t *node, int player);

void fill_tree(tree_node_t *node_ptr, int player);

int fill_child_list(tree_node_t *node_ptr, int player);
int fill_child_regular(tree_node_t *node_ptr, space_t start, int player);
int fill_child_tower(tree_node_t *node_ptr, space_t start, int player);
int add_child(tree_node_t *node_ptr, space_t space, space_t move, int player);

void free_tree(tree_node_t *node_ptr);

tree_node_t *initialize_root(board_t board, tree_node_t *root);
tree_node_t *initialize_node(board_t board, tree_node_t *node,
                             tree_node_t *parent, space_t start, space_t end);
list_node_t *get_list_node(tree_node_t *new_node);

void copy_board(board_t dest_board, board_t src_board);
list_node_t *find_tail(list_node_t *head);
int calculate_depth(tree_node_t *node);

/*----------------------------- MAIN -----------------------------------------*/

// Where the magic happens
int main(int argc, char *argv[])
{
    // The current game board to be used in the game
    board_t board;

    // Set-up the board
    initialize_board(board);

    // Start of the game printing
    print_game_info(board);
    print_board(board);

    // This is where the entire game logic is
    run_game_loop(board);

    return EXIT_SUCCESS; // exit program with the success code
}

/*------------------------     SETUP    --------------------------------------*/

void initialize_board(board_t board)
{
    int print_flag;
    char cell_piece;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        // For every row in the board
        print_flag = (i % 2 != 0); // First cell HAS piece for each ODD row

        if (i < ROWS_WITH_PIECES)
        {
            // For first 3 rows, store white pieces in cell_piece
            cell_piece = CELL_WPIECE;
        }
        else if (i >= BOARD_SIZE - ROWS_WITH_PIECES)
        {
            // For last 3 rows, store black pieces in cell_piece
            cell_piece = CELL_BPIECE;
        }
        else
        {
            // Store no pieces in cell_piece
            cell_piece = CELL_EMPTY;
        }

        for (int j = 0; j < BOARD_SIZE; j++)
        {
            // For each column
            if (print_flag)
            {
                // Store cell_piece at board[i][j]
                board[i][j] = cell_piece;
            }
            else
            {
                // Store empty cell at board[i][j]
                board[i][j] = CELL_EMPTY;
            }
            // This keeps alterneting for a checkerboard pattern to be achieved
            print_flag = !print_flag;
        }
    }
}

/* This function prints the game information at the start of the game*/
void print_game_info(board_t board)
{
    printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n", count_pieces(board, PLAYER_B));
    printf("#WHITE PIECES: %d\n", count_pieces(board, PLAYER_W));
}

/*------------------------- BOARD PRINTING -----------------------------------*/

//This function prints the board at a particular game state
void print_board(board_t board)
{
    print_col_titles();
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        // For each cell in the row
        print_seperator(); // Print the seperator
        for (int j = -1; j < BOARD_SIZE; j++)
        {
            // For each cell in the column
            // -1 because of the row titles cell
            for (int j_ = 0; j_ < CELL_SIZE; j_++)
            {
                // For the places inside the cell
                if (j_ == 1)
                {
                    if (j == -1)
                    {
                        // The cell with the row title
                        printf("%d", i + 1);
                    }
                    else
                    {
                        // The regular cells of the board
                        printf("%c", board[i][j]);
                    }
                }
                else
                {
                    printf(" "); // The rest of the cell
                }
            }
            printf("|"); // Seperator between cells
        }
    }
    print_seperator();
}

// This function prints the char column titles (A - H) at the start of every
// board
void print_col_titles(void)
{
    for (int i = -1; i < BOARD_SIZE; i++)
    {
        // -1 because of the empty cell
        for (int i_ = 0; i_ < CELL_SIZE; i_++)
        {
            if (i == -1)
            {
                printf(" ");
            }
            else
            {
                if (i_ == 1)
                {
                    // This ensures that the char title is centred
                    printf("%c", int2col(i));
                }
                else
                {
                    printf(" ");
                }
            }
        }
        printf(" "); // Delimiter
    }
}

// This function prints the seperator between each row of the board
void print_seperator(void)
{
    printf("\n");
    printf("   ");
    for (int i = 0; i < ROW_SEP_LENGTH; i++)
    {
        if (i % 4 == 0)
        {
            printf("+");
        }
        else
        {
            printf("-");
        }
    }
    printf("\n");
}

/*-------------------------- GAME LOOP ---------------------------------------*/

/*
This function takes as input the checkers board and initializes it to game
start state
*/

int run_game_loop(board_t board)
{
    space_t start_space, end_space;
    char start_col, end_col;
    int start_row, end_row;
    int error_code, scanf_return;
    int player = PLAYER_B; // The black player starts the game
    int action_count = 0;

    // This is the entire game loop
    while ((scanf_return = scanf("%c%d-%c%d\n",
                                 &start_col, &start_row,
                                 &end_col, &end_row)) == 4)
    {
        // Convert the char columns and rows starting from 1 into standardized
        // integers starting from 0
        start_space.col = col2int(start_col);
        start_space.row = start_row - 1;
        end_space.col = col2int(end_col);
        end_space.row = end_row - 1;

        // If this is an invalid move
        if ((error_code = scan_errors(board, start_space, end_space, player)))
        {
            // Deal with errors by passing error code
            error_handler(error_code);
            exit(EXIT_FAILURE);
        }

        // NOTE: Process_action also executes the action on the board if it
        //       is legal
        if (!process_action(board, start_space, end_space, player))
        {
            // If this is an illegal move
            printf("ERROR: Illegal Action");
            exit(EXIT_FAILURE);
        }

        // Print after the action has been processed
        print_action_seperator();
        print_action_info(board, ++action_count, player, start_space,
                          end_space, PROCESSED_N);
        print_board(board);
        player = switch_player(player);
    }

    if (scanf_return == 1)
    {
        // After the game loop ends, there might still be an extra 'A' or 'P'
        // at the end, if so, this body is executed
        if (start_col == 'A')
        {
            // Process 1 move
            process_next_move(board, player, ++action_count);
        }
        else // Start_col == 'P'
        {
            // Keep playing for 10 moves or until game end
            continue_playing(board, player, ++action_count);
        }
    }

    return 0;
}

/*---------------------  HELPERS ---------------------------------------------*/
// This function returns an integer value (0 - 7) of a char column (A - H)
int col2int(int col)
{
    return col - 65;
}

// This function returns a char value (A - H) of an integer column (0 - 7)
char int2col(int col)
{
    char index = col + 65;
    return index;
}

// This function counts the number of pieces on the board belonging
// to a particular player
int count_pieces(board_t board, int player)
{
    int count = 0;
    space_t temp_space;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        temp_space.row = i;
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            temp_space.col = j;
            count += (is_player_piece(board, temp_space, player));
        }
    }
    return count;
}

// Function returns -1 if the space is empty
// Returns 1 if space is tower
// Returns 0 if space is piece
int get_piece_type(board_t board, space_t space)
{
    if (board[space.row][space.col] == CELL_EMPTY)
    {
        return -1;
    }
    else if (board[space.row][space.col] == CELL_BTOWER ||
             board[space.row][space.col] == CELL_WTOWER)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Function returns 1 if the piece belongs to the specified player
int is_player_piece(board_t board, space_t space, int player)
{
    if (player == PLAYER_B)
    {
        return (board[space.row][space.col] == CELL_BPIECE ||
                board[space.row][space.col] == CELL_BTOWER);
    }
    else
    {
        return (board[space.row][space.col] == CELL_WPIECE ||
                board[space.row][space.col] == CELL_WTOWER);
    }
}

// This function switches the player and returns the switched player
int switch_player(int player)
{
    if (player == PLAYER_B)
    {
        return PLAYER_W;
    }
    else
    {
        return PLAYER_B;
    }
}

// Function returns the player a particular piece belongs to
// 0 if none
int get_player_from_piece(board_t board, space_t space)
{
    if ((board[space.row][space.col] == CELL_BPIECE) ||
        (board[space.row][space.col] == CELL_BTOWER))
    {
        return PLAYER_B;
    }
    if ((board[space.row][space.col] == CELL_WPIECE) ||
        (board[space.row][space.col] == CELL_WTOWER))
    {
        return PLAYER_W;
    }
    return 0;
}

// Computes the cost of the board
int board_cost(board_t board)
{
    int cost = 0;
    space_t space;
    for (space.row = 0; space.row < BOARD_SIZE; space.row++)
    {
        for (space.col = 0; space.col < BOARD_SIZE; space.col++)
        {
            if (get_piece_type(board, space) == PIECE_R)
            {
                cost += COST_PIECE * get_player_from_piece(board, space);
            }
            else if (get_piece_type(board, space) == PIECE_T)
            {
                cost += COST_TOWER * get_player_from_piece(board, space);
            }
        }
    }
    return cost;
}

/*--------------------------COMPUTER GENERATED ACTION-------------------------*/

// This function plays the next 10 moves or until game win
// Returns -1 if WHITE wins, 1 if BLACK wins, 0 if neither
int continue_playing(board_t current_board, int player, int action_count)
{
    int i = 0;
    while (i < COMP_ACTIONS) // or until game is not won
    {
        // PLay the next move
        process_next_move(current_board, player, action_count++);
        player = switch_player(player);
        i++;
    }
    return 0;
}

// This function processes and executes the next move after the 'A' char
// is read, if a player wins, victory is declared and the game ended.
void process_next_move(board_t current_board, int player, int action_count)
{
    move_t *next_move;
    space_t start, end;
    // Get the move_pointer of the best move the player ought to do
    next_move = compute_next_action(current_board, player);
    if (next_move->return_board != NULL)
    {
        // If all is well then:
        // Copy the board from the move to the actual game board
        copy_board(current_board, *next_move->return_board);
        free(next_move->return_board); // Free the move board

        // Copy the move required to achieve this board state
        start = next_move->start;
        end = next_move->end;

        // Print the required information
        print_action_seperator();
        print_action_info(current_board, action_count, player, start, end,
                          PROCESSED_Y);
        print_board(current_board);

        // If the player has won on the execution of this move (i.e the other
        // player cannot make a legal move on his turn)
        if (next_move->victory)
        {
            // VICTORY
            print_victory(player);
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        printf("ERROR -- NO MOVE CALCULATED\n");
        exit(EXIT_FAILURE);
    }
    free(next_move);
}

void print_victory(int player)
{
    if (player == PLAYER_B)
    {
        printf("BLACK ");
    }
    else
    {
        printf("WHITE ");
    }
    printf("WIN!");
}

/*---------------------------    ERRORS   ------------------------------------*/
/* Function takes the start and end spaces of a move and returns 0 if OK*/
// Otherwise the function returns an error-code to be used in tandem with the
// error_handler function.
int scan_errors(board_t board, space_t start, space_t end, int player)
{
    if (is_space_bounds(start))
    {
        // If the start space is out of bounds
        return 1;
    }
    if (is_space_bounds(end))
    {
        // If end space is out of bounds
        return 2;
    }
    if (get_piece_type(board, start) == PIECE_E)
    {
        // If the start space is empty
        return 3;
    }
    if (get_piece_type(board, end) != PIECE_E)
    {
        // If end space is not empty
        return 4;
    }
    if (!is_player_piece(board, start, player))
    {
        // If start space does not playing to the phasing player
        return 5;
    }
    return 0;
}

// This function prints the appropriate error message for an error code
void error_handler(int error_code)
{
    // Using a switch-case seemed best here instead of writing if statements
    // over and over again
    switch (error_code)
    {
    case 1:
        printf("ERROR: Source cell is outside of the board.\n");
        break;
    case 2:
        printf("ERROR: Target cell is outside of the board.\n");
        break;
    case 3:
        printf("ERROR: Source cell is empty.\n");
        break;
    case 4:
        printf("ERROR: Target cell is not empty.\n");
        break;
    case 5:
        printf("ERROR: Source cell holds opponent's piece/tower.\n");
        break;
    }
}

// Function returns 0 if the space is within the bounds of the board
int is_space_bounds(space_t space)
{
    return (space.row < 0 || space.row > (BOARD_SIZE - 1) ||
            space.col < 0 || space.col > (BOARD_SIZE - 1));
}

/*---------------------------- ACTION PRINTING -------------------------------*/

// This function prints the action information - the move, cost, moving player
// and whether or not it's processed
void print_action_info(board_t board, int action_no, int player, space_t start,
                       space_t end, int PROCESSED)
{
    if (PROCESSED)
    {
        printf("*** ");
    }

    if (player == PLAYER_B)
    {
        printf("BLACK ");
    }
    else
    {
        printf("WHITE ");
    }
    printf("ACTION #%d: ", action_no);
    printf("%c%d-%c%d\n", int2col(start.col), start.row + 1,
           int2col(end.col), end.row + 1);
    printf("BOARD COST: %d\n", board_cost(board));
}

// This function prints the seperator between two actions
void print_action_seperator()
{
    for (int i = 0; i < ACTION_SEP_LENGTH; i++)
    {
        printf("=");
    }
    printf("\n");
}

/*---------------------------- ACTION PROCESSING  ----------------------------*/

// Function processes a move and returns 1 if legal
// Returns 0 if illegal move
int process_action(board_t board, space_t start, space_t end, int player)
{
    // Determine the selected piece
    int piece_type = get_piece_type(board, start);
    // Determine if the offsets make sense or not
    int row_offset = start.row - end.row;
    int col_offset = start.col - end.col;
    if (piece_type == PIECE_R)
    {
        if (row_offset == (player * OFF_M) && abs(col_offset) == OFF_M)
        {
            // Legal regular move
            process_move(board, start, end);
            return 1;
        }
        else if (row_offset == (player * OFF_C) && abs(col_offset) == OFF_C)
        {
            // Possible legal regular capture
            // determine if legal capture (opp piece exists in captured space)
            if (process_capture(board, start, end, player))
            {
                return 1;
            }
        }
    }
    else if (piece_type == PIECE_T)
    {
        if (abs(row_offset) == OFF_M && abs(col_offset) == OFF_M)
        {
            // Legal tower move
            process_move(board, start, end);
            return 1;
        }
        else if (abs(row_offset) == OFF_C && abs(col_offset) == OFF_C)
        {
            // Possible legal tower capture
            // determine if legal capture (opp piece exists in captured space)
            if (process_capture(board, start, end, player))
            {
                return 1;
            }
        }
    }
    return 0;
}

/*---------------------------- MOVE PROCESSING  ------------------------------*/

// Processes a move, checks whether its a promotion or a non-promotion move
void process_move(board_t board, space_t start, space_t end)
{
    int piece = get_piece_type(board, start);
    int player = get_player_from_piece(board, start);
    if (piece == PIECE_T)
    {
        execute_move(board, start, end);
    }
    else
    {
        detect_promotion(board, start, end, player);
        execute_move(board, start, end);
    }
    return;
}

// Modifies the board to reflect the provided move (NOT CAPTURE)
void execute_move(board_t board, space_t start, space_t end)
{
    char moving_piece = board[start.row][start.col];
    board[end.row][end.col] = moving_piece;
    board[start.row][start.col] = CELL_EMPTY;
}

/*---------------------------- CAPTURE PROCESSING  ---------------------------*/

// Returns 0 if illegal, else returns 1 if legal
int process_capture(board_t board, space_t start, space_t end, int player)
{
    int row_offset = start.row - end.row;
    int col_offset = start.col - end.col;
    // Check validity of captured space
    space_t temp_cap_space;

    temp_cap_space.col = start.col + (col_offset < 0) - (col_offset > 0);
    temp_cap_space.row = start.row + (row_offset < 0) - (row_offset > 0);

    if (get_piece_type(board, temp_cap_space) != PIECE_E)
    {
        if (is_player_piece(board, temp_cap_space, switch_player(player)))
        {
            // Execute capture
            detect_promotion(board, start, end, player);
            execute_capture(board, start, end, temp_cap_space);
            return 1;
        }
    }
    return 0;
}

// Executes a fully valid capture.
void execute_capture(board_t board, space_t start, space_t end, space_t cap)
{
    board[cap.row][cap.col] = CELL_EMPTY;
    execute_move(board, start, end);
}

/*---------------------------- PROMOTION PROCESSING  -------------------------*/

// This function takes the start and end spaces of an assumed regular piece
// and the phasing player and promotes the piece if necessary
void detect_promotion(board_t board, space_t start, space_t end, int player)
{
    if (player == PLAYER_B)
    {
        if (end.row == 0)
        {
            promote_space(board, start);
            return;
        }
    }
    else
    {
        if (end.row == BOARD_SIZE - 1)
        {
            promote_space(board, start);
            return;
        }
    }
}

// Promotes a piece in the space from a regular to a tower
void promote_space(board_t board, space_t space)
{
    int player = get_player_from_piece(board, space);
    if (player == PLAYER_B)
    {
        board[space.row][space.col] = CELL_BTOWER;
    }
    else
    {
        board[space.row][space.col] = CELL_WTOWER;
    }
    return;
}

/*-------------------------- MINIMAX TREE IMPLEMENTAION ----------------------*/
/*----------------------------------------------------------------------------*/

/*--------------------------- MAIN LOGIC FUNCTIONS ---------------------------*/

// This function gets called when the 'A' command is read
// It returns the next move ought to be performed by the specified player
// -- IMPLEMENTATIONAL: It also returns the board state as a result of the move
//                      being returned.
move_t *compute_next_action(board_t board, int player)
{
    // root is a pointer to the start of the tree
    tree_node_t *root = (tree_node_t *)malloc(sizeof(tree_node_t));

    root = initialize_root(board, root);
    move_t *next_move = NULL;

    fill_tree(root, player); // The tree is filled

    if (root->children_count != 0) // If root has children
    {
        // Calculate the next action for the player to take
        next_move = calculate_next_action(root, player);
    }
    free_tree(root); // The tree is freed

    return next_move; // The next action is returned back to the game_loop
}

// This function calculates and returns the next move the player ought to take
move_t *calculate_next_action(tree_node_t *root, int player)
{
    // Get the minimax value from the tree given the player
    // NOTE: Minimax function also changes the cost of each non-leaf node into
    // a propagated cost and not the actual cost of the node
    int minimax_val = minimax(root, player);

    // Pointer that iterates through the list
    list_node_t *move_ptr = root->children_list;

    // Allocating memory for the return move (and board)
    move_t *return_ptr = (move_t *)malloc(sizeof(move_t));
    return_ptr->return_board = (board_t *)malloc(sizeof(board_t));

    while (move_ptr != NULL)
    {
        // For all children_nodes of the root -- LEFT-RIGHT TRAVERSAL
        if (move_ptr->node->cost == minimax_val)
        // If the propagated cost (not actual cost) of the node is equal to the
        // minimax value
        {
            // Copy the board and associated move into the return move&board
            copy_board(*(return_ptr->return_board),
                       move_ptr->node->board_state);
            return_ptr->start = move_ptr->node->move->start;
            return_ptr->end = move_ptr->node->move->end;

            if (move_ptr->node->children_count == 0)
            {
                // If on the next player's turn after this, he has no valid
                // moves left to play (hence the lack of children), then this is
                // a victory for the current player
                return_ptr->victory = player;
            }
            else
            {
                // No one wins on this move
                return_ptr->victory = 0;
            }

            return return_ptr;
        }
        move_ptr = move_ptr->next;
    }
    return NULL;
}

// This function calculates the minimax cost of the tree
// NOTE: This function also alters the cost of each node to be the
// propagated cost
int minimax(tree_node_t *node, int player)
{
    int max_cost, min_cost, value;
    if (node->depth == TREE_DEPTH || node->children_count == 0)
    {
        // If tree depth is reached or a node is a dead end
        return node->cost;
    }

    // Pointer that iterates through the list
    list_node_t *move_ptr = node->children_list;
    if (player == PLAYER_B)
    {
        // Black is the maximizing player
        max_cost = INT_MIN;
        while (move_ptr != NULL)
        {
            // Next move is white's so player is switched
            value = minimax(move_ptr->node, switch_player(player));
            if (value > max_cost)
            {
                max_cost = value;
            }
            move_ptr = move_ptr->next;
        }
        // Alter the cost of the node
        node->cost = max_cost;
        return max_cost;
    }
    else
    {
        // White is the minimzing player
        min_cost = INT_MAX;
        while (move_ptr != NULL)
        {
            // Next move is black's so player is switched
            value = minimax(move_ptr->node, switch_player(player));
            if (value < min_cost)
            {
                min_cost = value;
            }
            move_ptr = move_ptr->next;
        }
        // Alter the cost of the node
        node->cost = min_cost;
        return min_cost;
    }
}

/*-------------------------- FILLING THE TREE ---------------------------------*/

// This function completely recursively fills the tree
void fill_tree(tree_node_t *node_ptr, int player)
{
    if (node_ptr->depth == TREE_DEPTH)
    {
        return;
    }
    int child_flag;
    // Generate all possible legal children for this node
    child_flag = fill_child_list(node_ptr, player);

    if (child_flag == 0)
    {
        // If there are no children (possible legal moves)
        if (player == PLAYER_B)
        {
            // Black, the maximizing player  is not incentivized to lead to a
            // scenario where he has no moves left to play
            node_ptr->cost = INT_MIN;
        }
        else
        {
            // White, the minimizing player is not incentivized to lead to a
            // scenario where he has no moves left to play
            node_ptr->cost = INT_MAX;
        }

        return;
    }
    list_node_t *move_ptr = node_ptr->children_list;
    while (move_ptr != NULL)
    {
        // Recursively fill the tree for each child node generated
        fill_tree(move_ptr->node, switch_player(player));
        move_ptr = move_ptr->next;
    }
    return;
}

/*-------------------------- FILLING FUNCTIONS -------------------------------*/

// This function generates every possible child of the node (board) passed to it
// Returns 0 if no children generated
int fill_child_list(tree_node_t *node_ptr, int player)
{
    int child_flag = 0;
    int temp_flag;
    space_t start;
    int piece_type;
    for (start.row = 0; start.row < BOARD_SIZE; start.row++)
    {
        for (start.col = 0; start.col < BOARD_SIZE; start.col++)
        {
            // For every piece in the board state of the node
            piece_type = get_piece_type(node_ptr->board_state, start);
            if (piece_type == PIECE_E)
            {
                // Don't bother with empty pieces
                continue;
            }
            if (get_player_from_piece(node_ptr->board_state, start) != player)
            {
                // Don't bother with other player's pieces
                continue;
            }
            // For a piece that belongs to a player
            if (piece_type == PIECE_R)
            {
                // If this is a regular piece
                temp_flag = fill_child_regular(node_ptr, start, player);
            }
            else
            {
                // If this is a tower piece
                temp_flag = fill_child_tower(node_ptr, start, player);
            }
            if (child_flag == 0 && temp_flag == 1)
            {
                // These flags deal with whether a single child has been
                // generated for the entire node
                child_flag = 1;
            }
        }
    }
    // Return 0 if no child generated for this entire node
    return child_flag;
}

// This function generates and fills every possible move taken by a regular
// piece -- Returns 0 if no move generated
int fill_child_regular(tree_node_t *node_ptr, space_t start, int player)
{
    int move_flag = 0;
    int temp_flag;
    int row_offset, col_offset;
    space_t move;
    // This for loop executes FIRST once for all possible moves
    // THEN Once for all possible captures
    for (int type = OFF_M; type <= OFF_C; type++)
    {
        // type (1 or 2) is multiplied by player because regular pieces can only
        // move in one direction (row-wise)
        row_offset = player * type;

        // Negative column offset because of subtraction in the next loop,
        // - * - = +. The first column checked is positive, which is in line
        // with clockwise north-east checking
        col_offset = -type;
        for (int col_ = 0; col_ < 2; col_++)
        {
            // For the two possible columns the piece can end its move at
            move.row = start.row - row_offset;
            move.col = start.col - col_offset;
            // Change the column offset
            col_offset = -col_offset;
            if (scan_errors(node_ptr->board_state, start,
                            move, player))
            {
                // If this is an invalid move
                continue;
            }
            // Now we have established that it's a "possible" legal move
            // Determine if it is a legal move
            // Add it to the linked list now
            temp_flag = add_child(node_ptr, start, move, player);
            if (move_flag == 0 && temp_flag == 1)
            {
                // These flags are used to determine whether a single child
                // exists for this piece or not
                move_flag = 1;
            }
        }
    }
    // Returns 0 if no move generated
    return move_flag;
}

// This function generates (and fills) every possible legal move taken by a
// tower piece -- Returns 0 if no move generated
int fill_child_tower(tree_node_t *node_ptr, space_t start, int player)
{
    int move_flag = 0;
    int temp_flag;
    int row_offset, col_offset;
    space_t move;

    for (int type = OFF_M; type <= OFF_C; type++)
    {
        // This for loop executes FIRST once for all possible moves
        // THEN Once for all possible captures
        row_offset = player * type;
        col_offset = -type;
        for (int col_ = 0; col_ < 2; col_++)
        {
            // For the two possible cols the piece can end its
            // move at
            for (int row_ = 0; row_ < 2; row_++)
            {
                // For the two possible columns the piece can end
                // its move at -- NORTH EAST CLOCKWISE checking
                move.row = start.row - row_offset;
                move.col = start.col - col_offset;
                // Change row offset
                row_offset = -row_offset;
                if (scan_errors(node_ptr->board_state, start,
                                move, player))
                {
                    // If invalid move
                    continue;
                }
                // Now we have established that it's a possible legal move
                // Determine if it is a legal move
                // Add it to the linked list now
                temp_flag = add_child(node_ptr, start, move, player);
                if (move_flag == 0 && temp_flag == 1)
                {
                    move_flag = 1;
                }
            }
            // Change col_offset
            col_offset = -col_offset;
        }
    }
    return move_flag;
}

// This function adds a child with a specific move, returns 1 if legal move
// added successfully.
int add_child(tree_node_t *node_ptr, space_t space, space_t move, int player)
{
    // Have a temporary board to do changes on so that the actual node is not
    // altered with changes
    board_t temp_board;
    copy_board(temp_board, node_ptr->board_state);

    if (process_action(temp_board, space, move, player) == 0)
    {
        // If it is an illegal move
        return 0;
    }
    // The move is a legal move and has been executed on the temp_board
    // The process_action function has executed it already if it was legal

    // Add 1 to the children count
    int children_count = node_ptr->children_count + 1;
    node_ptr->children_count = children_count;

    // new_ptr is a new tree_node_t that will be used
    tree_node_t *new_ptr;
    // Allocate memory for the node new_ptr points to
    new_ptr = (tree_node_t *)malloc(sizeof(tree_node_t));

    // Initialize New_ptr
    new_ptr = initialize_node(temp_board, new_ptr, node_ptr, space, move);

    // Find the last child_node in the parent node's linked list
    list_node_t *last_list_node_ptr = find_tail(node_ptr->children_list);
    if (last_list_node_ptr == NULL)
    {
        // If there are no child_nodes in the parent node's linked list
        // Have this new child_node be the first one
        node_ptr->children_list = get_list_node(new_ptr);
    }
    else
    {
        // Have this new child node be pointed to by the last child_node in the
        // parent node's linked list
        last_list_node_ptr->next = get_list_node(new_ptr);
    }
    // Return 1 if successfully added
    return 1;
}

/*-------------------------- FREEING THE TREE ---------------------------------*/

// This function recursively frees the tree
void free_tree(tree_node_t *node_ptr)
{
    if (node_ptr->children_count == 0)
    {
        // If leaf_node, free it
        free(node_ptr);
        return;
    }
    list_node_t *move_ptr = node_ptr->children_list;
    while (move_ptr != NULL)
    {
        // Recursively free all children of this node
        free_tree(move_ptr->node);
        move_ptr = move_ptr->next;
    }
    // Free the current_node once all of it's children have been freed up
    free(node_ptr);
    return;
}

/*-------------------------- INITIALIZATION ----------------------------------*/

/* This function initializes the tree (root)*/
tree_node_t *initialize_root(board_t board, tree_node_t *root)
{
    // Initialize values
    copy_board(root->board_state, board);
    root->children_count = 0;
    root->cost = board_cost(board);
    root->depth = 0;
    root->children_list = NULL;
    root->parent = NULL;

    // Return pointer
    return root;
}

// This function initializes a new node and returns a pointer to it
tree_node_t *initialize_node(board_t board, tree_node_t *node,
                             tree_node_t *parent, space_t start, space_t end)
{
    // Copy the board state, specify parent and compute depth
    copy_board(node->board_state, board);
    node->parent = parent;
    node->depth = calculate_depth(node);

    // Allocate memory for and specify the move required to bring the board into
    // this state
    node->move = (move_t *)malloc(sizeof(move_t));
    node->move->start = start;
    node->move->end = end;

    // Initialize the cost and children related values
    node->cost = board_cost(node->board_state);
    node->children_count = 0;
    node->children_list = NULL;
    return node;
}

/*-------------------------- HELPERS -----------------------------------------*/

// This function returns a pointer to a new list_node to be added to the
// linked list with the tree node new_node as data for the list_node
list_node_t *get_list_node(tree_node_t *new_node)
{
    list_node_t *temp;
    // Allocate memory for the new node
    temp = (list_node_t *)malloc(sizeof(list_node_t));

    // Set values
    temp->node = new_node;
    temp->next = NULL;
    // Return the pointer
    return temp;
}

// This function copies the a board state to a new board;
void copy_board(board_t dest_board, board_t src_board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            dest_board[i][j] = src_board[i][j];
        }
    }
}

// This function returns a pointer to the final node in the linked list
list_node_t *find_tail(list_node_t *head)
{
    list_node_t *move_ptr = head;
    list_node_t *prev_ptr = NULL;
    while (move_ptr != NULL)
    {
        prev_ptr = move_ptr;
        move_ptr = move_ptr->next;
    }
    return prev_ptr;
}

// This function calculates the depth of a particular node in the tree by
// counting the number of ancestors it has (parents of parents...)
int calculate_depth(tree_node_t *node)
{
    int depth = 0;
    tree_node_t *move_ptr = node->parent;
    while (move_ptr != NULL)
    {
        depth++;
        move_ptr = move_ptr->parent;
    }
    return depth;
}

/* THE END -------------------------------------------------------------------*/