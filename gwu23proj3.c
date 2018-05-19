#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int debugMode;



typedef struct mazeStruct
{
 char **dArr;  // dynamic 2D array
 int xSize, ySize;
 int xStart, yStart;
 int xEnd, yEnd;
} maze;

// create a stack with a link list
typedef struct NodeStack {
    int xPosition;                 // xcoordinate
    int yPosition;                 // y coordinate
    struct NodeStack *next;

} stack;




// this function will take the file and read them and store them into variables by pass by reference
// this will check for 3 valid lines of input..if none then the program will exit
// only reads file, not actually putting in maze struct yet
//----------------------------------------------------------------------------------------------------------------------------------------------
void readFile (FILE *mazeSpecs, int *xSize, int *ySize, int *xStart, int *yStart, int *xEnd, int *yEnd) {

    // used to keep track if file runs out before getting 3 lines of valid input.
    int validSize = 0;
    int validStart = 0;
    int validEnd = 0;

    // read in two values at the same time for maze dimenensions
    while (fscanf (mazeSpecs, "%d %d", &*xSize, &*ySize) != EOF) {

        // make sure the maze dimensions are 1 and above not 0
        if (*xSize > 0 && *ySize > 0) {
            validSize = 1;
            break;
        }
        else {
            fprintf (stderr, "Invalid maze dimensions %d, %d cannot be less than or equal to 0\n", *xSize, *ySize);
            continue;
        }
    }

    // check for proper x and y start positions
    while (fscanf (mazeSpecs, "%d %d", &*xStart, &*yStart) != EOF) {

        // check if start positions are within the maze size
        if (*xSize >= *xStart && *ySize >= *yStart) {
            validStart = 1;
            break;
        }
        else {
            fprintf (stderr, "Invalid start positions %d %d cannot be greater than maze dimensions\n", *xStart, *yStart);
            continue;
        }
    }

    // check for proper x and y end positions
    while (fscanf (mazeSpecs, "%d %d", &*xEnd, &*yEnd) != EOF) {

        // check if end positions is within the maze boarder
        if (*xSize >= *xEnd && *ySize >= *yEnd) {
            validEnd = 1;
            break;
        }
        else {
            fprintf(stderr, "Invalid end positions %d %d cannot be outside of maze dimensions\n", *xEnd, *yEnd);
            continue;
        }
    }


    // check if 3 proper valid lines were read from input
    if (validSize == 0 || validStart == 0 || validEnd == 0) {
        fprintf(stderr, "Filed ended before getting 3 lines of valid input...Exiting....");
        exit(1);
    }

    // print maze information for verification
    printf ("Maze Information:\n");
    printf ("size: %d, %d\n", *xSize, *ySize);
    printf ("start: %d, %d\n", *xStart, *yStart);
    printf ("end: %d, %d\n", *xEnd, *yEnd);


}


// this function will set up the maze struct
// dynamically allocate 2d array for maze board
// make * boarder and mark start and end are from the code that we were given to start off with
//-----------------------------------------------------------------------------------------------------------------------------------------
void setUpMaze (FILE *mazeSpecs, maze *maze, int xSize, int ySize, int xStart, int yStart, int xEnd, int yEnd) {

    int i;
    int j;

    // use to store the * for blocking points
    int xPosition;
    int yPosition;

    // allocate the 2D array
    maze->dArr = malloc (sizeof (char*) * (xSize + 2));

    for (i=0; i < xSize + 2; i++) {
        maze->dArr[i] = malloc (sizeof (char) * (ySize + 2));
    }

    // set up maze values from read file
    maze->xSize = xSize;
    maze->ySize = ySize;
    maze->xStart = xStart;
    maze->yStart = yStart;
    maze->xEnd = xEnd;
    maze->yEnd = yEnd;

    // make the maze empty or filled with dots
    // . means empty
    for (i = 0; i < maze->xSize + 2; i++) {

        for (j = 0; j < maze->ySize + 2; j++) {

            maze->dArr[i][j] = '.';
        }
    }

    // make * boarders
    for (i=0; i < maze->xSize + 2; i++){

     maze->dArr[i][0] = '*';
     maze->dArr[i][maze->ySize + 1] = '*';
    }

    for (i=0; i < maze->ySize + 2; i++)
    {
     maze->dArr[0][i] = '*';
     maze->dArr[maze->xSize + 1][i] = '*';
    }

    // mark start (S) and end (E)
    maze->dArr[xStart][yStart] = 'S';
    maze->dArr[xEnd][yEnd] = 'E';

    // mark block positions
    while (fscanf (mazeSpecs, "%d %d", &xPosition, &yPosition) != EOF) {

        // check if trying to put a block outside of maze
        if (xPosition > xSize || yPosition > ySize) {
            fprintf(stderr, "Invalid spot %d %d : cannot place a block outside of maze\n", xPosition, yPosition);
            continue;
        }

        // check if trying to place ontop of start position
        if (xPosition == xStart && yPosition == yStart) {
            fprintf(stderr, "Invalid: attempting to block starting position %d %d\n", xPosition, yPosition);
            continue;
        }

        // check if trying to place ontop of end position
        if (xPosition == xEnd && yPosition == yEnd) {
            fprintf(stderr, "Invalid: attempting to block ending position %d %d\n", xPosition, yPosition);
            continue;
        }

        // if passes all the if statements conditionals we put a * as a block in that spot
        maze->dArr[xPosition][yPosition] = '*';

    }

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
// passed by value to return head which makes head to null
stack* initializeStack (stack *head ){

    // make it point to null to begin with
    head = NULL;
    return head;
}
// check if stack is empty...aka if link list has nothing in it
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
int isEmpty (stack *head) {

    // return 1 if empty
    if (head == NULL) {
        return 1;
    }
    else {
        return 0;
    }

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// push by pass by reference
// similar to pat troy insert at front in lab 5
void push (stack **head, int x, int y) {

    // create a new node
    stack* ptr = (stack*) malloc (sizeof(stack));
    ptr->xPosition = x;
    ptr->yPosition = y;
    ptr->next = *head;
    *head = ptr;

    if (debugMode) {
        printf ("%d, %d has been pushed onto the stack!\n", x, y);
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// pop node by pass by reference
// similar to pat troy remove from front in lab 5
void pop (stack **head) {

    // create a new node pointer to point to head
    stack* ptr = *head;

    if (debugMode) {
        printf ("%d, %d has been popped off the stack\n", ptr->xPosition, ptr->yPosition);
    }

    if (ptr != NULL) {

        *head = ptr->next;
        free (ptr);
   }

   // if stack is empty
   else {
       fscanf (stderr, "stack is empty. error\n");
       return;
   }

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// grab the values at top of stack
void getTopValues (stack *head, int *x, int *y) {

    // check if stack is empty
    if (head == NULL) {
        fprintf(stderr, "stack is empty cannot grab top values error\n");
        return;
    }

    // pass by reference to get x and y pos of top of stack
    *x = head->xPosition;
    *y = head->yPosition;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// remove all nodes in the link list properly to avoid memory leak
// make it point to null at the end
void resetStack (stack **head) {

    // create node pointers
    stack *temp = *head;
    stack *nextNode;

    while (temp != NULL) {

        nextNode = temp->next;       // mark a spot in the next node
        free (temp);                 // deallocate memory
        temp = nextNode;             // move on to the next node to free
    }

    *head = NULL;                    // after everything deallocates, we make head point to null

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// recursive function to print out the link list in reverse
void printStack (stack *head) {

    if (head == NULL){
        return;
    }

    printStack(head->next);

    printf("%d %d\n", head->xPosition, head->yPosition);

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main (int argc, char **argv) {

    // hold maze dimeensions
    int xSize;
    int ySize;

    // hold starting position of maze
    int xStart;
    int yStart;

    // hold ending position of maze
    int xEnd;
    int yEnd;

    // loop variables
    int i;
    int j;

    debugMode = 0;

    // for argv to see if user enters multple file names
    char *fileName;

    // create maze
    maze m1;

    // create stack and initlize it
    stack *head;
    head = initializeStack(head);

     FILE *mazeSpecs;  // declare file variable

    // see if the command line -d was found
    int flag = 0;
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            printf("DebugMode Activated\n");
            debugMode = 1;
            flag = 1;
        }
        else {
            fileName = argv[i];
        }
    }

    // allows -d to go in front or back
    argv[1] = fileName;

    /* verify the proper number of command line arguments were given */
    if(argc < 2) {
        printf("Usage: %s <input file name>\n", argv[0]);
        exit(-1);
    }

    // too many arguments
    if (argc > 3) {
        printf("Too many arguments..exiting\n");
        exit(-1);
    }

    // more than 2 files
    if (argc == 3 && flag == 0) {
        printf("Too many names were given, please enter one name only\n");
        exit(-1);

    }

    /* Try to open the input file. */
    if ( ( mazeSpecs = fopen( argv[1], "r" )) == NULL )
    {
        printf ( "Can't open input file: %s", argv[1] );
        exit(-1);
    }

    // read file and check for errors
    readFile(mazeSpecs, &xSize, &ySize, &xStart, &yStart, &xEnd, &yEnd);

    // set up maze
    setUpMaze(mazeSpecs, &m1, xSize, ySize, xStart, yStart, xEnd, yEnd);

    /* print out the initial maze */
    for (i = 0; i < m1.xSize+2; i++)
    {
        for (j = 0; j < m1.ySize+2; j++)
            printf ("%c", m1.dArr[i][j]);
            printf("\n");
        }

    // push xstart and y start onto stack
    push(&head, xStart, yStart);

    // mark start position as visited
    m1.dArr[xStart][yStart] = 'V';

    // continue to loop while stack is not empty and end has not been found yet
    // 0 means not empty. 1 means empty
    // i did the order:
    // down, up, right, left
    while (isEmpty(head) == 0 ) {

        // end position has been reached
        if (xStart == xEnd && yStart == yEnd) {
            break;
        }

        // check down
        if (m1.dArr[xStart+1][yStart] == '.' || m1.dArr[xStart+1][yStart] == 'E' ) {         // dot means an open space

            xStart = xStart + 1;               // update value
            push(&head, xStart, yStart);       // push coordinates onto the stack
            m1.dArr[xStart][yStart] = 'V';     // mark as visited
        }

        // check up
        else if (m1.dArr[xStart-1][yStart] == '.' || m1.dArr[xStart+1][yStart] == 'E') {

            xStart = xStart - 1;                  //update location
            push(&head, xStart, yStart);          // push onto stack
            m1.dArr[xStart][yStart] = 'V';        // update to visited
        }

        // check right
        else if (m1.dArr[xStart][yStart + 1] == '.' || m1.dArr[xStart+1][yStart] == 'E') {

            yStart = yStart + 1;
            push(&head, xStart, yStart);
            m1.dArr[xStart][yStart] = 'V';
        }

        // check left
        else if (m1.dArr[xStart][yStart - 1] == '.' || m1.dArr[xStart+1][yStart] == 'E') {

            yStart = yStart - 1;
            push(&head, xStart, yStart);
            m1.dArr[xStart][yStart] = 'V';
        }

        // no more avaible space to move so we go back..pop
        else {

            pop(&head);

            // update x and y to match top of stack
            getTopValues(head, &xStart, &yStart);
        }

    } // end of while loop

    // check if stack is esmpty
    if (isEmpty(head)) {
        printf("The maze has NO SOLUTIONS\n");
    }
    else {
        printf("Here is the solution starting from START points to END:\n");
        printStack(head);   // print in reverse by recurssion
    }

    // free maze
    for (i-0; i<m1.xSize+2; i++) {
        free (m1.dArr[i]);
    }

    free (m1.dArr);

    // dellacote memory for head
    resetStack(&head);

    return 0;


}
