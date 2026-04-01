#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>

int change_top[3];
int change_bot[3];


void generate_hole(int walltype, int i){

  switch (walltype) {   
    case 1:
      change_top[i] = 300;
      change_bot[i] = 480;
      break;

    case 2:
      change_top[i] = 400;
      change_bot[i] = 600;
      break;

    case 3:
      change_top[i] = 200;
      change_bot[i] = 360;
      break;

    case 4:
      change_top[i] = 400;
      change_bot[i] = 620;
      break;

    case 5:
      change_top[i] = 100;
      change_bot[i] = 375;
      break;

    default:
    break;
  }
}

struct Character{
  int left;
  int top; 
  int right; 
  int bottom; 

};

struct Wall{
  int left;
  int top; 
  int right; 
  int bottom; 
};

int get_random_num(){

  int max = 5;
  int min = 1;

  int ran_num = rand() % (max - min + 1) + min;

  return ran_num;

}

void free_mem(struct Character* bird, struct Wall** walls, void* bg_img){
  free(bird);
  free(bg_img);

  for(int i=0; i<3; i++){
    free(walls[i]);
  }
}

void start_menu(){

  char start_msg[] = "PRESS ANY KEY TO START";

  settextstyle(3, HORIZ_DIR, 4);
  outtextxy(50,350, start_msg);
  getch();
  
  return;
}

void get_high_score(char* score){

  FILE *fp = fopen("HighScore.txt", "r");

  fgets(score, 30, fp);

  fclose(fp);
}

void set_high_score(char* score, char* high_score){

  int h_score = atoi(high_score);
  int curr_score = atoi(score);

  if(curr_score > h_score){

    FILE *fp = fopen("HighScore.txt", "w");
    fputs(score, fp);
    fclose(fp);

  }else{
    return;
  }

}

int main(){


  srand(time(NULL));

  char key_input;
  int game_end = 0;
  int velocity = 1;
  int accleration = 1;

  char score_str[20];
  char high_score_str[20];

  char score_msg[50];
  char high_score_msg[50];

  int score = 0;

  int wall_reset[3] = {1,1,1};

  int walltype;

  int wl = 600;
  int wr = 650;

  int top_wall_top[3], top_wall_bottom[3], top_wall_left[3], top_wall_right[3];

  int bot_wall_top[3], bot_wall_bottom[3], bot_wall_left[3], bot_wall_right[3];

  char high_score[30];

  get_high_score(high_score);
  
  struct Character *bird; 

  struct Wall *walls[3];

  for(int i=0; i< 3; i++){
    walls[i] = (struct Wall *) malloc(sizeof(struct Wall));

    if(walls[i] == NULL){
      printf("Error creating wall");
      return -1;
    }
  }
  
  walls[0]->left = wl;
  walls[0]->top = 0;
  walls[0]->right = wr;
  walls[0]->bottom = 700;

  for(int i=1; i<3; i++){
    walls[i]->left = walls[i-1]->left+300;
    walls[i]->top = 0;
    walls[i]->right = walls[i-1]->right+300;
    walls[i]->bottom = 700;
  }

  bird = (struct Character *) malloc(sizeof(struct Character));


  if(bird == NULL){
    printf("Errror creating Character.");
    return -1;
  }

  //INITIAL BIRD POSITION
  bird->left = 100;
  bird->top = 325;
  bird->right = 125;
  bird->bottom = 300;

  //NOTE: bird width and height = 25

  initwindow(650,700,"Flappy Bird");

  //background imgage
  void *bg_img;
  int bg_size = imagesize(0,0,getmaxx(), getmaxy());
  bg_img = malloc(bg_size);

  readimagefile("sky8bit.bmp",0,0,getmaxx(),getmaxy());
  getimage(0,0,getmaxx(), getmaxy(), bg_img);

  setfillstyle(SOLID_FILL, WHITE);

  setcolor(WHITE);

  int page = 0;

  start_menu();

  while(true){

    setactivepage(page);
    setvisualpage(1-page);
    cleardevice();

    putimage(0, 0, bg_img, COPY_PUT);

    setfillstyle(SOLID_FILL, RED);
    setcolor(WHITE);

    rectangle(bird->left,bird->top,bird->right,bird->bottom);
    bar(bird->left,bird->top,bird->right, bird->bottom);


    //DRAW WALLS
    for(int i=0; i<3; i++){

      setcolor(WHITE);
      setfillstyle(SOLID_FILL, GREEN);

      if(wall_reset[i]){
        walltype = get_random_num();
        generate_hole(walltype, i);

        wall_reset[i] = 0;
      }

      top_wall_top[i] = walls[i]->top;
      // top_wall_bottom[i] = walls[i]->bottom - 500;
      top_wall_bottom[i] = change_top[i];
      top_wall_left[i] = walls[i]->left;
      top_wall_right[i] = walls[i]->right;

      bot_wall_top[i] = change_bot[i];
      bot_wall_bottom[i] = walls[i]->bottom;
      bot_wall_left[i] = walls[i]->left;
      bot_wall_right[i] = walls[i]->right;

      if(walls[i]->right >= 0 && walls[i]->left <= getmaxx()){
        rectangle(walls[i]->left, top_wall_top[i], walls[i]->right, top_wall_bottom[i]);
        bar(walls[i]->left, top_wall_top[i], top_wall_right[i], top_wall_bottom[i]);

        rectangle(walls[i]->left, bot_wall_top[i], walls[i]->right, bot_wall_bottom[i]);
        bar(walls[i]->left, bot_wall_top[i], bot_wall_right[i], bot_wall_bottom[i]);
      }

      if(walls[0]->left < 0-50 && walls[0]->right < 650){
        walls[0]->left = wl+300;
        walls[0]->right = wr+300;
        wall_reset[0] = 1;
      }

      if(walls[1]->left < 0-50 && walls[1]->right < 650){
        walls[1]->left = wl+300;
        walls[1]->right = wr+300;
        wall_reset[1] = 1;
      }

      if(walls[2]->left < 0-50 && walls[2]->right < 650){
        walls[2]->left = wl+300;
        walls[2]->right = wr+300;
        wall_reset[2] = 1;
      }

    }

    //score counting
    for(int i=0; i<3; i++){
      if(
        (top_wall_right[i] == bird->left)  
      ){
        score++;
        wall_reset[i] = 0;
      }
    }

    //user input
    if(kbhit()){
      key_input = getch();

      switch (key_input) {

        case 'q':
          game_end = 1;
          free_mem(bird, walls, bg_img);
          printf("score: %d ",score);
          snprintf(score_str, sizeof(score_str), "%d", score);
          set_high_score(score_str, high_score);
          return 0;
          break;

        case 32:
          if(bird->top > 25){
            bird->top -= 45;
            bird->bottom -= 45;
            velocity = 1;
          }

        default:
          break;
      }
    }

    //DETECT IF BIRD TOUCHES BOTTOM OF THE SCREEN
    if(bird->bottom < 670){
      bird->top += velocity;
      bird->bottom += velocity;
      if(velocity < 12){
        velocity += accleration;
      }
    }else{
      game_end = 1;
    }

    if(bird->top <= 0){
      game_end = 1;
    }

    //WALL COLLISION DETECTION

    for(int i=0; i<3; i++){
      if(
        (
          (bird->left + 25 >= top_wall_left[i]) &&
          (bird->left <= top_wall_left[i] + 50) &&
          (bird->top+ 25 >= top_wall_bottom[i]-10) &&
          (bird->bottom <= top_wall_top[i] + change_top[i]) 
        ) || 
        (
          (bird->left + 25 >= bot_wall_left[i]) &&
          (bird->left <= bot_wall_left[i] + 50) &&
          (bird->top + 25 >= bot_wall_top[i]+27) &&
          (bird->bottom <= bot_wall_bottom[i] + change_bot[i]) 
        )
      )
      {
        game_end = 1;
        page = 1 - page;
        delay(16);
        free_mem(bird, walls, bg_img);
        printf("score: %d ",score);
        snprintf(score_str, sizeof(score_str), "%d", score);
        set_high_score(score_str, high_score);
        return 0;
      }
    }


    for(int i=0; i<3; i++){
      walls[i]->left -= 5;
      walls[i]->right -= 5;
    }


    if(game_end){
      free_mem(bird, walls, bg_img);
      printf("score: %d ",score);
      snprintf(score_str, sizeof(score_str), "%d", score);
      set_high_score(score_str, high_score);
      return 0;
    }

    snprintf(score_str, sizeof(score_str), "%d", score);
    snprintf(score_msg, sizeof(score_msg), "score: %s", score_str);

    snprintf(high_score_msg, sizeof(high_score_msg), "High Score: %s", high_score);

    settextstyle(3, HORIZ_DIR, 4);
    outtextxy(0, 0, score_msg);

    settextstyle(3, HORIZ_DIR, 4);
    outtextxy(405, 0, high_score_msg);

    delay(16);
    page = 1 - page;
  }

  set_high_score(score_str, high_score);
  printf("score: %d ",score);
  getch();
  closegraph();
  
  return 0;
}
