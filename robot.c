#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "graphics.h"

//positioning variables for the grid
const int GRID_SIZE=400;
const int WIN_WIDTH=600, WIN_HEIGHT=600;
const int ROWS=10,COLUMNS=10;
const int EDGE_X=(WIN_WIDTH-GRID_SIZE)/2;
const int EDGE_Y=(WIN_HEIGHT-GRID_SIZE)/2;
const int RECT_SIZE= GRID_SIZE/COLUMNS;

//arrays that store position of markers or blocks in the grid
const int MARKER_NUM=8;
int markers[MARKER_NUM][2]={{0,9},{9,9},{0,1},{3,3},{6,7},{3,0},{8,0},{5,3}};
const int BLOCK_NUM=8;
int blocks[BLOCK_NUM][2]={{3,4},{4,5},{5,4},{7,7},{9,8},{1,1},{5,9},{8,8}};

//array to keep track of what has been visited
bool visited[ROWS][COLUMNS];

//directions represented as numbers
#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

struct robot
{
    int x;
    int y;
    int direction;
    int x_coords[3];
    int y_coords[3];
    int marker_count;
};

//fills empty space with rects to form grid
void draw_grid()
{
    background();
    for(int row=0;row<ROWS;row++)
    {
        for(int column=0;column<COLUMNS;column++)
        {
            drawRect(EDGE_X+(column*RECT_SIZE),EDGE_Y+(row*RECT_SIZE),RECT_SIZE,RECT_SIZE);
        }
    }
}

//colours in a rect at a specified position
void fill_square(int row, int column)
{
    int x=column*RECT_SIZE+EDGE_X;
    int y=row*RECT_SIZE+EDGE_Y;
    fillRect(x,y,RECT_SIZE,RECT_SIZE);
}

//draws the home square
void draw_home(int row,int column)
{
    background();
    setColour(blue);
    fill_square(row,column);
}

void draw_markers(int row,int column)
{
    background();
    setColour(gray);
    fill_square(row,column);
}

void draw_blocks(int row,int column)
{
    background();
    setColour(black);
    fill_square(row,column);
}

//gets coordinates for robot(triangle) so that it points in the direction it faces
void set_direction(struct robot *point)
{
    switch(point->direction) //robots direction
    {
        case RIGHT: 
            point->x_coords[0]=point->x;
            point->x_coords[1]=point->x;
            point->x_coords[2]=point->x + RECT_SIZE;
            point->y_coords[0]=point->y;
            point->y_coords[1]=point->y+RECT_SIZE;
            point->y_coords[2]=point->y +(RECT_SIZE)/2;
            break;
        case UP: 
            point->x_coords[0]=point->x;
            point->x_coords[1]=point->x +RECT_SIZE;
            point->x_coords[2]=point->x +(RECT_SIZE)/2;
            point->y_coords[0]=point->y+RECT_SIZE;
            point->y_coords[1]=point->y+RECT_SIZE;
            point->y_coords[2]=point->y;
            break;
        case LEFT: 
            point->x_coords[0]=point->x+RECT_SIZE;
            point->x_coords[1]=point->x+RECT_SIZE;
            point->x_coords[2]=point->x;
            point->y_coords[0]=point->y+RECT_SIZE;
            point->y_coords[1]=point->y;
            point->y_coords[2]=point->y+(RECT_SIZE)/2;
            break;
        case DOWN: 
            point->x_coords[0]=point->x;
            point->x_coords[1]=point->x+RECT_SIZE;
            point->x_coords[2]=point->x+(RECT_SIZE)/2;
            point->y_coords[0]=point->y;
            point->y_coords[1]=point->y;
            point->y_coords[2]=point->y+RECT_SIZE;
            break;
    }
}

void draw_bot(struct robot *point)
{
    foreground();
    setColour(green);
    set_direction(point);
    fillPolygon(3,point->x_coords,point->y_coords);
}

//checks home is a valid square in grid
bool is_home_on_grid(int row, int column)
{
    if(column<0 || column>9 || row<0 || row>9)
    {
        return(false);
    }
    return(true);
}

//checks its one of the four valid directions
bool is_valid_direction(int direction)
{ 
    if(direction<0 || direction>3)
    {
        return(false);
    }
    return(true);
}

//check home not on a marker
bool is_home_on_marker(int row, int column)
{
    for(int marker=0;marker<MARKER_NUM;marker++)
    {
        if (markers[marker][0]==row && markers[marker][1]==column)
        {
            return(true);
        }
    }
    return(false);
}

//check home not on a block
bool is_home_on_block(int row, int column)
{
    for(int block=0;block<BLOCK_NUM;block++)
    {
        if (blocks[block][0]==row && blocks[block][1]==column)
        {
            return(true);
        }
    }
    return(false);
}

//validates if command line inputs were valid
bool validate_cl(int row,int column,int direction)
{   
    if(!is_home_on_grid(row,column) || !is_valid_direction(direction) || is_home_on_marker(row,column) || is_home_on_block(row,column))
    {
        return(false);
    }

    //otherwise valid
    return(true);
}

//checks if the rect in front is the wall
bool blocked_by_wall(struct robot *point)
{//checks if both x and y coordinate match walls coordinate
    if((point->x==EDGE_X)&&(point->direction==LEFT))
    {
        return(true);
    }
    else if((point->x==EDGE_X+GRID_SIZE-RECT_SIZE)&&(point->direction==RIGHT))
    {
        return(true);
    }
    else if((point->y==EDGE_Y)&&(point->direction==UP))
    {
        return(true);
    }
    else if((point->y==EDGE_Y+GRID_SIZE-RECT_SIZE)&&(point->direction==DOWN))
    {
        return(true);
    }
    return(false);
}

//checks if the rect in front is a block
bool blocked_by_block(struct robot *point)
{
    for(int block=0;block<BLOCK_NUM;block++) //checks every block in the block array
    {   //checks if both x and y coordinate match blocks coordinate
        if((point->direction==RIGHT)&&((point->x+RECT_SIZE-EDGE_X)/RECT_SIZE==blocks[block][1])&&((point->y-EDGE_Y)/RECT_SIZE==blocks[block][0]))
        {
            return(true);
        }
        else if((point->direction==UP)&&((point->y-RECT_SIZE-EDGE_Y)/RECT_SIZE==blocks[block][0])&&((point->x-EDGE_X)/RECT_SIZE==blocks[block][1]))
        {
            return(true);
        }
        else if((point->direction==LEFT)&&((point->x-RECT_SIZE-EDGE_X)/RECT_SIZE==blocks[block][1])&&((point->y-EDGE_Y)/RECT_SIZE==blocks[block][0]))
        {
            return(true);
        }
        else if((point->direction==DOWN)&&((point->y+RECT_SIZE-EDGE_Y)/RECT_SIZE==blocks[block][0])&&((point->x-EDGE_X)/RECT_SIZE==blocks[block][1]))
        {
            return(true);
        }
    }
    return(false);
}

//checks if you are able to move forward
bool can_move_forward(struct robot *point)
{
    if(blocked_by_wall(point))
    {
        return(false);
    }
    if(blocked_by_block(point))
    {
        return(false);
    }
    return(true);
}

void forward(struct robot *point)
{   //increments x coordinate a pixel at a time until its moved forward one rect
    for(int displacement=0;displacement<RECT_SIZE;displacement++)
    {
            switch(point->direction)
            {
                case RIGHT: 
                    point->x++;
                    break;
                case UP: 
                    point->y--;
                    break;
                case LEFT: 
                    point->x--;
                    break;
                case DOWN: 
                    point->y++;
                    break;
            }
            //draw robot at new position,wait and then clear the robot so it can be redrawn at new position
            draw_bot(point);
            sleep(2);
            clear();
    }
    draw_bot(point); //keep robot on square after wiping it
}

void reverse_direction(struct robot *point,int direction)
{
    point->direction=(direction+2)%4;
}

void left(struct robot *point)
{   
    point->direction=(point->direction +1)%4; //direction set to be the one left of current direction
    clear();
    draw_bot(point);
}

void right(struct robot *point)
{   
    point->direction=(point->direction +3)%4; //direction set to be the one right of current direction
    clear();
    draw_bot(point);
}

//checks if robot is at home square
bool at_home(struct robot *point,int row,int column)
{
    int home_x=EDGE_X+column*RECT_SIZE;
    int home_y=EDGE_Y+row*RECT_SIZE;
    if(point->x==home_x && point->y==home_y)
    {
        return(true);
    }
    return(false);
}

//goes through each marker to check if robot is at a marker
bool at_marker(struct robot *point,int markers[MARKER_NUM][2])
{   
    for(int marker=0;marker<MARKER_NUM;marker++)
    {
        int marker_x=(markers[marker][1])*RECT_SIZE+EDGE_X;
        int marker_y=(markers[marker][0])*RECT_SIZE+EDGE_Y;
        if(point->x==marker_x && point->y==marker_y)
        {
            return(true);
        }
    }
    return(false);
}

//if at the marker pick up the marker
void pick_up_marker(struct robot *point,int markers[MARKER_NUM][2])
{
    if(at_marker(point,markers))
    {   //fill given rect white
        point->marker_count++;
        background();
        setColour(white);
        fillRect(point->x,point->y,RECT_SIZE,RECT_SIZE);
        //redraw rect border lines
        setColour(black);
        drawRect(point->x,point->y,RECT_SIZE,RECT_SIZE);
        foreground();
    }
}

bool is_carrying_a_marker(struct robot *point)
{   //if robot has a marker return true
    if(point->marker_count>=1)
    {
        return(true);
    }
    return(false);
}

void drop_marker(struct robot *point)
{   //check carrying a marker
    if(is_carrying_a_marker(point))
    {   
        int x=(point->x - EDGE_X)%RECT_SIZE;
        int y=(point->y - EDGE_Y)%RECT_SIZE;
        //check robot fully inside the rect first
        if(x==0 && y==0)
        {   
            background();
            //then drop marker on rect
            setColour(gray);
            fillRect(point->x,point->y,RECT_SIZE,RECT_SIZE);
            foreground();
        }
    }
}

//stores the move performed
void record_moves(int moves[(ROWS*COLUMNS)],struct robot *point, int *stack_ptr)
{   //push last move onto the stack
    moves[*stack_ptr]=point->direction;
    //increment stack pointer
    (*stack_ptr)++;
}

//Goes backwards along the recorded path 
void reverse_moves(int moves[(ROWS*COLUMNS)],struct robot *point, int *stack_ptr)
{   //goes from last move to first move
    for(int move=*stack_ptr - 1;move>=0;move--)
    {   //opposes the original movement made 
        reverse_direction(point,moves[move]);
        forward(point);
    }
}

//fill visited with false everywhere except blocks
void initialise_visited(bool visited_list[ROWS][COLUMNS],int blocks[BLOCK_NUM][2])
{
    for(int row=0;row<ROWS;row++)
    {
        for(int column=0;column<COLUMNS;column++)
        {
            visited_list[row][column]=false;
        }
    }
    for (int block=0;block<BLOCK_NUM;block++)
    {
        int block_row=blocks[block][0];
        int block_column=blocks[block][1];
        visited_list[block_row][block_column]=true;
    }
}

//sets a given rect to visited
void update_visited(int row,int column,bool visited_list[ROWS][COLUMNS])
{
    visited_list[row][column]=true;
}

//checks if a rect has been visited
bool is_visited(int row,int column,bool visited_list[ROWS][COLUMNS])
{
    if(visited_list[row][column]==true)
    {
        return(true);
    }
    return(false);
}

//checks if every rect has been visited
bool is_finished_visiting(bool visited_list[ROWS][COLUMNS])
{
    for(int row=0;row<ROWS;row++)
    {
        for(int column=0;column<COLUMNS;column++)
        {
            if(!is_visited(row,column,visited_list))
            {
                return(false);
            }
        }
    }
    return(true);
}

//finds what rect is in front of current rect
void find_rect_in_front(struct robot *point,int row, int column,int next[2])
{
    int next_row,next_column;
    switch(point->direction)
    {
        case RIGHT:
            next_row=row;
            next_column=column+1;
            break;
        case UP:
            next_row=row-1;
            next_column=column;
            break;
        case LEFT:
            next_row=row;
            next_column=column-1;
            break;
        case DOWN:
            next_row=row+1;
            next_column=column;
            break;    
    }
    //saves the next row and next column to an next rect array
    next[0]=next_row;
    next[1]=next_column;
}

//recursively traverses every rect and stops when done
void traverse(struct robot *point,bool visited_list[ROWS][COLUMNS],int markers[MARKER_NUM][2],int moves[(ROWS*COLUMNS)],int *move_ptr,int home_row,int home_column)
{   //find current rect and next rect
    int current_row=(point->y-EDGE_Y)/RECT_SIZE;
    int current_column=(point->x-EDGE_X)/RECT_SIZE;
    int next[2];
    find_rect_in_front(point,current_row,current_column,next);
    int next_row=next[0];
    int next_column=next[1];

    //add current rect to visited list if not already visited
    if(!is_visited(current_row,current_column,visited_list))
    {
        update_visited(current_row,current_column,visited_list);
    }

    //if the grid has not been completely visited
    if(!is_finished_visiting(visited_list))
    {   //check each possible direction once for a path
        for(int direction=0;direction<4;direction++)
        {
            if(can_move_forward(point) && !is_visited(next_row,next_column,visited_list))
            {
                forward(point);
                record_moves(moves,point,move_ptr);
                //checks if currently at a marker or home
                pick_up_marker(point,markers);
                if (at_home(point,home_row,home_column))
                {
                    drop_marker(point);
                    draw_home(home_row,home_column);
                    foreground();
                }
                //calls itself to try traversing again
                traverse(point,visited_list,markers,moves,move_ptr,home_row,home_column);
                break;
            }
            //if a given direction doesnt work go left and try again
            left(point);    
            find_rect_in_front(point,current_row,current_column,next);
            next_row=next[0];
            next_column=next[1];        
        }
        //if unable to traverse anywhere then backtrack a rect and try again
        if(!is_finished_visiting(visited_list)) //neccessary to terminate recursion when unwinding calls
        {   //move back a square
            reverse_direction(point,moves[*move_ptr -1]);
            forward(point);
            (*move_ptr)--; //take out last move of path that causes dead end(as invalid move)
            reverse_direction(point,point->direction); //put back to original direction
            traverse(point,visited_list,markers,moves,move_ptr,home_row,home_column);
        }
    }   
}

int main(int argc, char **argv)
{
    setWindowSize(WIN_WIDTH,WIN_HEIGHT);
    draw_grid();
    int home_column,home_row,home_direction;
    //assigns command line inputs if only 4
    if(argc==4)
    {
    home_row=atoi(argv[1]);
    home_column=atoi(argv[2]);
    home_direction=atoi(argv[3]);
    }
    //does not let program run if inputs were invalid
    if((validate_cl(home_row,home_column,home_direction))==false)
    {
        return(1);
        printf("error with command line arguments!");
    }
    //draws out the grid with markers and blocks
    draw_home(home_row,home_column);
    for(int marker=0;marker<MARKER_NUM;marker++)
    {
        draw_markers(markers[marker][0],markers[marker][1]);
    }
    for(int block=0;block<BLOCK_NUM;block++)
    {
        draw_blocks(blocks[block][0],blocks[block][1]);
    }
    struct robot bot={.x=(home_column)*RECT_SIZE+EDGE_X,.y=(home_row)*RECT_SIZE+EDGE_Y,.direction=home_direction,.marker_count=0};
    int moves[ROWS*COLUMNS]; //array size set to absolute maximum amount of moves (number of rects)
    int move=0;
    initialise_visited(visited,blocks);

    //traverses grid and returns to home when done
    traverse(&bot,visited,markers,moves,&move,home_row,home_column);
    reverse_moves(moves,&bot,&move);
}
