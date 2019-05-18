
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INIT_NODE_CONNECTION 10

typedef struct _point{
    int positionX;
    int positionY;
    int hash;
    int *connection;
    int connection_cnt;
    char active;
    char type;
}point;

struct _net{
    point **nodes;
    int nodes_active;
    int width;
    int hight;
}*grid;


char grid_init(struct _net *grid, char *filename){

    FILE *mfile = fopen(filename,"r");

    char linia[500];
    char tmp[500];
    int  linia_len = 0;
    int  linia_cnt = 0;
    while( fgets(linia, 500, mfile) ){
        
        for(linia_len=0; linia[linia_len] !=0xA; linia_len++);
      
        printf("Dlugosc linii:<%d>\n",linia_len);

        memcpy(tmp, linia, linia_len);
        tmp[linia_len]='\0';
        
        printf("Linia:[%s]\n",tmp);

        if( 0 == linia_cnt ) {
            grid->hight = atoi(tmp);
        }
        if( 1 == linia_cnt ) {
            grid->width = atoi(tmp);
        }        
        linia_cnt++;
    }
    fclose(mfile);

    grid->nodes = (point **) malloc( sizeof(point) * grid->hight );
    for(int i=0; i < grid->hight; i++){
        grid->nodes[i] = (point *) malloc( sizeof(point) * grid->width);
    }
    grid->nodes_active = 0;

    grid->nodes[0][0].positionX = 12;
    grid->nodes[0][0].connection_cnt = INIT_NODE_CONNECTION;                                           //initially we alocate 10 connections
    //grid->nodes[0][0].connection = (int*) malloc( sizeof(int) * grid->nodes[0][0].connection_cnt );

    return 1;
}

int grid_gen_hash(int Y, int X, int height, int width){

    return 0;
}

char grid_pulldata(struct _net *grid, char *filename){

    FILE *mfile = fopen(filename, "r");

    int  X=0;
    int  Y=0;
    char line[500];
    int  line_cnt = 0;
    int  line_len = 0;
    while( fgets(line, 500, mfile) ){

        if(line_cnt < 2){
            line_cnt++;
            continue;
        }else{

            for(line_len=0; line[line_len] != 0xA; line_len++ ){

                char mark = line[line_len];
                if( 's' ==  mark || 'g' == mark || '_' == mark ){
                    grid->nodes[Y][X].type = mark;
                    grid->nodes[Y][X].active = 1;
                    grid->nodes[Y][X].hash = grid_gen_hash(Y, X, grid->height, grid->width);                    
                }else{
                    grid->nodes[Y][X].type = 'x';
                    grid->nodes[Y][X].active = 0;
                    grid->nodes[Y][X].hash = 0;                                        
                }
                X++;
            }
            Y++;
        }
        line_cnt++;
    }
    
    fclose(mfile);

    return 1;
}


char grid_construct(struct _net *grid){

    

    return 1;
}


char grid_show(){

    return 1;
}


char grid_release(struct _net *grid) {

    for(int i=0; i < grid->hight; i++){
        free(grid->nodes[i]);
    }
    free(grid->nodes);

    return 1;
}


int main(){

    char filename[] = "../maps/2.txt";

    grid = (struct _net *) malloc(sizeof(struct _net));

    grid_init(grid,filename);

    grid_pulldata(grid,filename);
    
    if( grid->nodes[0][0].positionX == 12 ){
        printf("Grid height=%d\n",grid->hight);
        printf("Grid width=%d\n",grid->width);
        printf("Grid active nodes=%d\n",grid->nodes_active);
    }


    grid_release(grid);



  
    return 1;
}