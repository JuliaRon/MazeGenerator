/*
 * ============================================================
 *  IY427 - Maze Generator  |  Skeleton Code
 * ============================================================
 *
 *  DATA STRUCTURE
 *  --------------
 *  The maze is a 2-D grid of Cell structs stored in a flat
 *  array (maze[row][col]).  Each Cell owns the wall on its
 *  RIGHT side and the wall on its BOTTOM side.  The left wall
 *  of cell (c) is the right wall of cell (c-1), and the top
 *  wall of cell (r) is the bottom wall of cell (r-1).
 *
 *  Player and Goal are both simple {x, y} coordinate pairs.
 *
 *  TEAMS & TODO
 *  ------------
 *  [GENERATION]  - generateMaze()       <-- already implemented
 *  [PLAYER]      - movePlayer()         <-- TODO
 *                  checkWin()           <-- TODO
 *  [VISUALS/UI]  - printMaze()          <-- TODO
 *                  printMenu()          <-- TODO stub provided
 *                  getUserInput()       <-- TODO stub provided
 *
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================
 *  CONSTANTS  –  change these to support difficulty levels
 * ============================================================ */
#define ROWS 10
#define COLS 10

/* ============================================================
 *  DATA STRUCTURES
 * ============================================================ */

/*
 * Cell - one square in the maze grid.
 *
 * wall_right  : is there a wall on the RIGHT edge of this cell?
 * wall_bottom : is there a wall on the BOTTOM edge of this cell?
 * visited     : used during maze generation (DFS); students can
 *               reuse this flag for the solver if they like.
 */
typedef struct {
    int wall_right;
    int wall_bottom;
    int visited;        /* generation / solver helper */
} Cell;

/*
 * The maze itself – a 2-D array of Cells.
 * Access with:  maze[row][col]
 */
Cell maze[ROWS][COLS];

/*
 * Player and Goal positions.
 * pos[0] = column (x),  pos[1] = row (y)
 */
int player[2];   /* { col, row } */
int goal[2];     /* { col, row } */

/* ============================================================
 *  MAZE GENERATION  (DFS / Recursive Back-tracker)
 * ============================================================
 *
 *  Algorithm overview:
 *   1. Start at (0,0), mark it visited.
 *   2. Pick a random unvisited neighbour.
 *   3. Remove the wall between current cell and that neighbour.
 *   4. Recurse into the neighbour.
 *   5. When no unvisited neighbours remain, backtrack.
 *
 *  Result: a perfect maze (exactly one path between any two
 *  cells → always solvable, no loops).
 * ============================================================ */

/* Helper: shuffle an array of ints (Fisher-Yates) */
static void shuffleArray(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

/* Core DFS function – do not call directly, use generateMaze() */
static void dfsMaze(int row, int col) {
    maze[row][col].visited = 1;

    /* The four directions: 0=UP, 1=RIGHT, 2=DOWN, 3=LEFT */
    int dirs[4] = {0, 1, 2, 3};
    shuffleArray(dirs, 4);   /* randomise order */

    for (int i = 0; i < 4; i++) {
        int d = dirs[i];
        int newRow = row, newCol = col;

        if      (d == 0) newRow--;   /* UP    */
        else if (d == 1) newCol++;   /* RIGHT */
        else if (d == 2) newRow++;   /* DOWN  */
        else             newCol--;   /* LEFT  */

        /* Skip out-of-bounds or already-visited cells */
        if (newRow < 0 || newRow >= ROWS) continue;
        if (newCol < 0 || newCol >= COLS) continue;
        if (maze[newRow][newCol].visited)  continue;

        /* Remove the shared wall */
        if (d == 1) maze[row][col].wall_right  = 0;   /* remove right wall  */
        if (d == 3) maze[row][newCol].wall_right = 0; /* remove right wall of left neighbour */
        if (d == 2) maze[row][col].wall_bottom = 0;   /* remove bottom wall */
        if (d == 0) maze[newRow][col].wall_bottom = 0;/* remove bottom wall of upper neighbour*/

        dfsMaze(newRow, newCol);   /* recurse */
    }
}

/*
 * generateMaze()
 *
 * Initialises every cell with all walls ON, then runs the DFS
 * to carve a perfect maze.  Also places the player at the
 * top-left and the goal at the bottom-right.
 *
 * Call this before any printing or player interaction.
 */
void generateMaze(void) {
    srand((unsigned int)time(NULL));

    /* Initialise all cells: all walls up, none visited */
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            maze[r][c].wall_right  = 1;
            maze[r][c].wall_bottom = 1;
            maze[r][c].visited     = 0;
        }
    }

    /* Carve the maze starting from the top-left corner */
    dfsMaze(0, 0);

    /* Place player and goal */
    player[0] = 0;        player[1] = 0;          /* top-left  */
    goal[0]   = COLS - 1; goal[1]   = ROWS - 1;   /* bottom-right */

    printf("Maze generated! (%d x %d)\n", ROWS, COLS);
}

/* ============================================================
 *  VISUALS / UI  –  TODO for the Visuals & UI team
 * ============================================================ */

/*
 * printMaze()
 *
 * TODO: Print the maze to the console using characters like:
 *
 *   '#' or '|' for vertical walls
 *   '-'        for horizontal walls
 *   ' '        for open passages
 *   'P'        for the player  (use player[0], player[1])
 *   'G'        for the goal    (use goal[0],   goal[1])
 *
 * Hint: loop over rows and columns.  For each cell, check
 * wall_right and wall_bottom to decide what to draw.
 * Remember that the TOP wall of a cell is the BOTTOM wall of
 * the cell above it.
 */
void printMaze(void) {
    /* TODO – Visuals team */
    printf("[printMaze] Not yet implemented.\n");
}

/*
 * printMenu()
 *
 * TODO: Display the main menu options to the user.
 *
 * Must-have options:
 *   1. Generate new maze
 *   2. Play maze (WASD movement)
 *   3. Solve maze  (should-have)
 *   4. Exit
 *
 * Could-have additions: show hints, timer, choose difficulty...
 */
void printMenu(void) {
    /* TODO – Visuals team */
}

/*
 * getUserInput()
 *
 * TODO: Read a single character from stdin and return it.
 * Make sure to flush extra characters (e.g. the newline) so
 * the next read works correctly.
 */
char getUserInput(void) {
    char c;
    /* TODO – Visuals team */
    return c;
}

/* ============================================================
 *  PLAYER LOGIC  –  TODO for the Player Logic team
 * ============================================================ */

/*
 * canMove(fromRow, fromCol, direction)
 *
 * Returns 1 if movement in `direction` is allowed from the
 * given cell, 0 if a wall blocks the way.
 *
 * direction: 'w'=UP, 's'=DOWN, 'a'=LEFT, 'd'=RIGHT
 *
 * Hint: Check wall_right / wall_bottom of the correct cell.
 * Remember the wall ownership rules at the top of this file.
 */
int canMove(int row, int col, char direction) {
    /* TODO – Player Logic team */
    (void)row; (void)col; (void)direction; /* suppress warnings */
    return 0;
}

/*
 * movePlayer(direction)
 *
 * Moves the player one cell in the given direction IF allowed.
 * Update player[0] (col) and player[1] (row) accordingly.
 * Print a message if movement is blocked by a wall.
 *
 * direction: 'w'=UP, 's'=DOWN, 'a'=LEFT, 'd'=RIGHT
 */
void movePlayer(char direction) {
    /* TODO – Player Logic team */
    (void)direction; /* suppress warning until implemented */
    printf("[movePlayer] Not yet implemented.\n");
}

/*
 * checkWin()
 *
 * Returns 1 if the player has reached the goal, 0 otherwise.
 */
int checkWin(void) {
    /* TODO – Player Logic team */
    return (1);
}

/* ============================================================
 *  MAZE SOLVER  –  STRETCH GOAL
 * ============================================================ */

/*
 * solveMaze()
 *
 * Should-have feature: find and display the solution path.
 *
 * Suggested approach: BFS or DFS from player position to goal.
 * Mark cells that are part of the solution (you could add a
 * `solution` flag to the Cell struct, or use a separate array).
 * Then call printMaze() so it renders the solution path.
 */
void solveMaze(void) {
    /* TODO – stretch goal */
    printf("[solveMaze] Not yet implemented.\n");
}

/* ============================================================
 *  MAIN GAME LOOP
 * ============================================================ */

int main(void) {
    int running = 1;
    int mazeReady = 0;

    while (running) {
        printMenu();
        char choice = getUserInput();

        switch (choice) {
            case '1':
                generateMaze();
                mazeReady = 1;
                break;

            case '2':
                if (!mazeReady) {
                    printf("Please generate a maze first!\n");
                    break;
                }
                /* Inner play loop */
                printf("Use W/A/S/D to move. Press Q to quit.\n");
                printMaze();
                {
                    char move;
                    int playing = 1;
                    while (playing) {
                        printf("Move: ");
                        move = getUserInput();
                        if (move == 'q' || move == 'Q') {
                            playing = 0;
                        } else {
                            movePlayer(move);
                            printMaze();
                            if (checkWin()) {
                                /* TODO – Visuals team: print a win message */
                                printf("*** You reached the goal! Well done! ***\n");
                                playing = 0;
                            }
                        }
                    }
                }
                break;

            case '3':
                if (!mazeReady) {
                    printf("Please generate a maze first!\n");
                    break;
                }
                solveMaze();
                printMaze();
                break;

            case '4':
                printf("Goodbye!\n");
                running = 0;
                break;

            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}