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

/* Raw mode: Unix/Mac only (Linux too).
 * On Windows, remove this block and replace getUserInput()
 * with:  char getUserInput(void) { return (char)_getch(); }
 * and add  #include <conio.h>  at the top instead. */
#ifdef _WIN32
  #include <conio.h>
#else
  #include <termios.h>
  #include <unistd.h>

  static struct termios orig_termios;

  static void enableRawMode(void) {
      tcgetattr(STDIN_FILENO, &orig_termios);
      struct termios raw = orig_termios;
      raw.c_lflag &= ~(ECHO | ICANON);  /* turn off echo & line buffering */
      raw.c_cc[VMIN]  = 1;              /* read returns after 1 byte      */
      raw.c_cc[VTIME] = 0;              /* no timeout                     */
      tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  }

  static void disableRawMode(void) {
      tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
  }
#endif

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
    int visited;
    int solution;   /* 1 if this cell is on the solution path */
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
            maze[r][c].solution    = 0;
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

    /*  the TOP border of the maze */
    for (int c = 0; c < COLS; c++) {
        printf("⋆｡°✩");
    }
    printf("+\n");

    for (int r = 0; r < ROWS; r++) {

        /* cells with vertical walls */
        for (int c = 0; c < COLS; c++) {

            if (c == 0)
                printf("|");   // left outer wall

            /* Player / Goal / Empty space */
            if (player[0] == c && player[1] == r)
                printf(" P ");
            else if (goal[0] == c && goal[1] == r)
                printf(" G ");
            else if(maze[r][c].solution)
                printf(" * ");
            else
                printf("   ");

            /* Right wall */
            if (maze[r][c].wall_right)
                printf("|");
            else
                printf(" ");
        }

        printf("\n");

        /* Print bottom walls */
        for (int c = 0; c < COLS; c++) {

            printf("+");

            if (maze[r][c].wall_bottom)
                printf("---");
            else
                printf("   ");
        }

        printf("+\n");
    }
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

    printf("\n");
    printf("=================================\n");
    printf("        MAZE GENERATOR GAME      \n");
    printf("=================================\n");
    printf("1. Generate new maze\n");
    printf("2. Play maze (W/A/S/D movement)\n");
    printf("3. Solve maze\n");
    printf("4. Exit\n");
    printf("=================================\n");
    printf("Choose an option: ");
}

char getUserInput(void) {
#ifdef _WIN32
    return (char)_getch();
#else
    enableRawMode();
    char c = (char)getchar();
    disableRawMode();
    return c;
#endif
}

/* ============================================================
 *  PLAYER LOGIC
 * ============================================================ */

/*
 * canMove - checks wall between current cell and neighbour.
 *
 * Wall ownership reminder:
 *   Moving RIGHT  -> check wall_right  of (row, col)
 *   Moving LEFT   -> check wall_right  of (row, col-1)
 *   Moving DOWN   -> check wall_bottom of (row, col)
 *   Moving UP     -> check wall_bottom of (row-1, col)
 */
int canMove(int row, int col, char direction) {
    if (direction == 'd') {                          /* RIGHT */
        if (col >= COLS - 1) return 0;               /* outer wall */
        return !maze[row][col].wall_right;
    }
    if (direction == 'a') {                          /* LEFT  */
        if (col <= 0) return 0;
        return !maze[row][col - 1].wall_right;
    }
    if (direction == 's') {                          /* DOWN  */
        if (row >= ROWS - 1) return 0;
        return !maze[row][col].wall_bottom;
    }
    if (direction == 'w') {                          /* UP    */
        if (row <= 0) return 0;
        return !maze[row - 1][col].wall_bottom;
    }
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
    int col = player[0];
    int row = player[1];

    if (!canMove(row, col, direction)) {
        printf("Ouch! There's a wall there.\n");
        return;
    }

    if      (direction == 'd') player[0]++;
    else if (direction == 'a') player[0]--;
    else if (direction == 's') player[1]++;
    else if (direction == 'w') player[1]--;
}

/*
 * checkWin()
 *
 * Returns 1 if the player has reached the goal, 0 otherwise.
 */
int checkWin(void) {
    return (player[0] == goal[0] && player[1] == goal[1]);
}

/* ============================================================
 *  MAZE SOLVER  (BFS from player to goal)
 * ============================================================
 *
 *  BFS guarantees the shortest path.  We store a `parent`
 *  array so we can trace back the path once the goal is found.
 */
void solveMaze(void) {
    /* Clear any previous solution */
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            maze[r][c].solution = 0;

    /* parent[r][c] stores the cell we came from: {col, row}
       (-1,-1) means unvisited */
    int parent[ROWS][COLS][2];
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++) {
            parent[r][c][0] = -1;
            parent[r][c][1] = -1;
        }

    /* BFS queue: store {col, row} pairs */
    int queue[ROWS * COLS][2];
    int head = 0, tail = 0;

    int startCol = player[0], startRow = player[1];
    queue[tail][0] = startCol;
    queue[tail][1] = startRow;
    tail++;
    parent[startRow][startCol][0] = startCol; /* mark start as visited */
    parent[startRow][startCol][1] = startRow;

    int found = 0;

    while (head < tail && !found) {
        int c = queue[head][0];
        int r = queue[head][1];
        head++;

        char dirs[4] = {'w', 'd', 's', 'a'};
        int dc[4]    = { 0,   1,   0,  -1};
        int dr[4]    = {-1,   0,   1,   0};

        for (int i = 0; i < 4; i++) {
            if (!canMove(r, c, dirs[i])) continue;
            int nc = c + dc[i];
            int nr = r + dr[i];
            if (parent[nr][nc][0] != -1) continue; /* already visited */

            parent[nr][nc][0] = c;
            parent[nr][nc][1] = r;
            queue[tail][0] = nc;
            queue[tail][1] = nr;
            tail++;

            if (nc == goal[0] && nr == goal[1]) {
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        printf("No solution found!\n");
        return;
    }

    /* Trace path back from goal to start, mark solution cells */
    int c = goal[0], r = goal[1];
    while (!(c == startCol && r == startRow)) {
        maze[r][c].solution = 1;
        int pc = parent[r][c][0];
        int pr = parent[r][c][1];
        c = pc; r = pr;
    }
    /* Don't mark the player's current cell */

    printf("Solution found! Follow the '*' path.\n\n");
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
                printMaze();
                break;

            case '2':
                if (!mazeReady) {
                    printf("Please generate a maze first!\n\n");
                    break;
                }
                printf("Use W/A/S/D to move. Press Q to quit to menu.\n\n");
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
                                printf("*** You reached the goal! Well done! ***\n\n");
                                playing = 0;
                            }
                        }
                    }
                }
                break;

            case '3':
                if (!mazeReady) {
                    printf("Please generate a maze first!\n\n");
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
                printf("Invalid option. Please try again.\n\n");
        }
    }

    return 0;
}