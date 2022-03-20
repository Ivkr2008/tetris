#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>

#define ELEMENT_SIZE 4
char possible_elements[5][ELEMENT_SIZE][ELEMENT_SIZE] = {
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1 ,0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0},
        {0, 0, 0 ,0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    }
     
};

struct Element {
    int x,y;
    char body[ELEMENT_SIZE][ELEMENT_SIZE];
};




struct Game {
    char field[10][20];
    struct Element* current_element;
    int score;
    struct Element* stash;
};

int getch(){
    int ch;
    struct termios oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

void clear_memory(char* memory, int size){
    for(int i = 0; i < size; i++) *(memory + i) = 0;
}

void* get_new_memory(int size){
    char* memory = malloc(size);
    clear_memory(memory,size);
    return memory;
}


struct Element* new_element(){
    char element_number = rand()%5;
    struct Element* element_ptr = malloc(sizeof (struct Element));
    clear_memory(element_ptr, sizeof (struct Element));
    (*element_ptr).x = 5;
    (*element_ptr).y = 10;
    for(int i = 0; i < ELEMENT_SIZE; i++)
        for(int j = 0; j < ELEMENT_SIZE; j++)
          element_ptr->body[i][j]  = possible_elements[2][i][j];
    
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
    for(int i = 0; i < ELEMENT_SIZE; i++) {
        for(int j = 0; j < ELEMENT_SIZE; j++) {
            if (x+i < 0 || x+i >= 10 || y+j < 0 || y+j >= 20) continue;
            canvas[x+i][y+j] = game_ptr->current_element->body[i][j];
        }
    }
    drawing(canvas);
}


void turn_180_y(struct Element* element){
    char buffer;
    for(int i = 0; i < ELEMENT_SIZE; i++)
        for(int j = 0; j < ELEMENT_SIZE/2; j++){
            buffer = element->body[i][j];
            element->body[i][j] = element->body[i][ELEMENT_SIZE-j-1];
            element->body[i][ELEMENT_SIZE-j-1] = buffer;

        }

}


void turn_180_diagonal(struct Element* element){
    char buffer;
    for(int i = 0; i < ELEMENT_SIZE; i++)
        for(int j = 0; j < ELEMENT_SIZE; j++){
            if(i>j)continue;
            buffer = element->body[i][j];
            element->body[i][j] = element->body[j][i];
            element->body[j][i] = buffer;
        }
          

}

void turn_90_right_unsafe(struct Element* element){
    turn_180_y(element);
    turn_180_diagonal(element); 
}




void turn_90_left_unsafe(struct Element* element){
    turn_180_diagonal(element); 
    turn_180_y(element);
}

void turn_90_left(struct Game* game){
    turn_90_left_unsafe(game->current_element);
    if (!check_collisions(game))  turn_90_right_unsafe(game->current_element);
}

void turn_90_right(struct Game* game){
    turn_90_right_unsafe(game->current_element);
    if (!check_collisions(game))  turn_90_left_unsafe(game->current_element);
}

int check_collisions(struct Game* game){
    int x = game->current_element->x;
    int y = game->current_element->y;
    for(int i = 0; i < ELEMENT_SIZE; i++)
        for(int j = 0; j < ELEMENT_SIZE; j++)
            if (!game->current_element->body[i][j]) continue;
            else if (x+i < 0 || x+i >= 10 || y+j < 0 || y+j >= 20) return 0;
            else if (game->field[x+i][y+j]) return 0;
    return 1;        
}

void move_down(struct Game* game){
    game->current_element->y--;
    if (!check_collisions(game)) game->current_element->y++;
}

void move_right(struct Game* game){    
    
    game->current_element->x++;
    if (!check_collisions(game)) game->current_element->x--;
}

void move_left(struct Game* game){
    game->current_element->x--;
    if (!check_collisions(game)) game->current_element->x++;
}

void print_element(struct Element* element){
    for(int i = 0; i < ELEMENT_SIZE; i++){
        printf("\n");
        for(int j = 0; j < ELEMENT_SIZE; j++)
            if (element->body[i][j]) printf("#");
            else printf(" ");   
    }
             
}    
          
int main(){
    struct Game* game = new_game();
    while(1){
        char command = getch();
    
        switch(command){
            case 'w':
                turn_90_left(game);
                break;
            case 'a':
                move_left(game);
                break;
            case 'd':
                move_right(game);
                break;
            case 's':
                move_down(game);
                break;
        }
         
        render_game(game);
    }
}
