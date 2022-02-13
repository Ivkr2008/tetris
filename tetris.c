#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <graphics>//
char possible_elements[2][4][4] = {
    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    }
};

struct Element {
    int x,y;
    char body[4][4];
};




struct Game {
    char field[10][20];
    struct Element* current_element;
    int score;
    struct Element* stash;
};
void clear_memory(char* memory, int size){
    for(int i = 0; i < size; i++) *(memory + i) = 0;
}

void* get_new_memory(int size){
    char* memory = malloc(size);
    clear_memory(memory,size);
    return memory;
}


struct Element* new_element(){
    char element_number = rand()%2;
    struct Element* element_ptr = malloc(sizeof (struct Element));
    clear_memory(element_ptr, sizeof (struct Element));
    (*element_ptr).x = 5;
    (*element_ptr).y = 20;
    for(int i; i < 4; i++)
        for(int j; j < 4; j++)
          element_ptr->body[i][j]  = possible_elements[element_number][i][j];
    
    return element_ptr;
}
struct Game* new_game(){
    struct Game* game_ptr = malloc(sizeof (struct Game)); 
    clear_memory(game_ptr, sizeof (struct Game));
    (*game_ptr).current_element = new_element();
    (*game_ptr).score = 0;
    (*game_ptr).stash = NULL;
}

    



void drawing(char canvas[10][20]){
    printf("\e[1;1H\e[2J");
    for(int j = 19; j >= 0; j--){
        for(int i = 0; i < 10; i++)
            if(canvas[i][j]) printf("#");
            else printf(" ");
        printf("\n");
    }
}

void render_game(struct Game* game_ptr){
    char canvas[10][20]; 
    for(int i = 0; i < 200; i++)
        *((char*)canvas + i) = *(*game_ptr->field + i);  
    int x = game_ptr->current_element->x;  
    int y = game_ptr->current_element->y;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            canvas[x+i][y+j] = game_ptr->current_element->body[i][j];
    drawing(canvas);
}



int main(){
    struct Game game = *new_game();
    game.field[0][0] = 1;
    game.current_element->x = 0;
    game.current_element->y = 10;
    render_game(&game);
}
