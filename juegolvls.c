#include <stdlib.h>
#include <ncurses.h>

#define NUM_LEVELS 2 // define the number of levels

int victoria = 0;
int current_level = 0; // keep track of the current level
char semen;

typedef struct {
  int **data;
  int width;
  int height;
  int starcol;
  int starrow;
  int endcol;
  int endrow;
} maze_t;

maze_t *create_maze(int width, int height, char *maze_data, int endcol, int endrow) {
  maze_t *maze = malloc(sizeof(maze_t));
  maze->width = width;                            // lee el mapa de un string
  maze->height = height;                          // lo guarda como array de numeros
  maze->endcol = endcol;
  maze->endrow = endrow;                          // pared es 0, suelo es 1, objetivo es 2
  maze->data = malloc(sizeof(int *) * height);
  for (int i = 0; i < height; i++) {
    maze->data[i] = malloc(sizeof(int) * width);
    for (int j = 0; j < width; j++) {
      char c = maze_data[i * width + j];
      if (c == '#') {
        maze->data[i][j] = 0;
      } else if (c == 'O') {
        maze->data[i][j] = 2;
      } else {
        maze->data[i][j] = 1;
      }
    }
  }

  return maze;
}

void draw_maze(maze_t *maze) {
  for (int i = 0; i < maze->height; i++) {              //caga el mapa de vuelta
    for (int j = 0; j < maze->width; j++) {             //usando los 0,1 y 2 del array
      char c;                                           //se podrian usar otras cosas como cuadrados
      if (maze->data[i][j] == 0) {                      //de ascii pero no se hacer eso
        c = '#';
      } else if (maze->data[i][j] == 2) {
        c = 'O';
      } else {
        c = ' ';
      }
      mvprintw(i, j, "%c", c);
    }
  }
}

void handle_input(maze_t *maze, int *row, int *col) { //coge el input y chequea si la direccion a la
  int ch = getch();                                   //que quiere ir es mayor que 0 (seria pared)
  switch (ch) {
    case KEY_UP:
      if (*row > 0 && maze->data[*row - 1][*col] > 0) {
        *row -= 1;
      }
      break;
    case KEY_DOWN:
      if (*row < maze->height - 1 && maze->data[*row + 1][*col] > 0) {
        *row += 1;
      }
      break;
    case KEY_LEFT:
      if (*col > 0 && maze->data[*row][*col - 1] > 0) {
        *col -= 1;
      }
      break;
    case KEY_RIGHT:
      if (*col < maze->width - 1 && maze->data[*row][*col + 1] > 0) {
        *col += 1;
      }
      break;
  }
}

int siguientenivel(maze_t *maze, int row, int col) {
  clear();
  mvprintw(10, 32, "NIVEL COMPLETADO!");
  mvprintw(15, 25, "Presiona cualquier tecla para continuar.");
  const char colact = col + '0';
  const char rowact = row + '0';
  refresh();
  getch();

  free(maze->data);
  free(maze);
  
  return 0;
}

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  int level = 1;
  int row = 1;
  int col = 1;
  int max_levels = NUM_LEVELS;

  while (level <= max_levels) {
    char *maze_data;
    int endrow, endcol, starcol, starrow;
    int width = 20, height = 10;   

    switch (level) {
      case 1:
        maze_data = "####################"
                    "#              ##  #"
                    "# ### ### ##  ######"
                    "# # # # # #   #    #"
                    "# # # # # # # # ####"
                    "# ### ### # # #    #"
                    "# #       # # #### #"
                    "# ########## #    ##"
                    "#     O      # #####"
                    "####################";
        endrow = 8;
        endcol = 6;
        starrow = 1;
        starcol = 1;
        break;

      case 2:
        maze_data = "####################"
                    "#             ######"
                    "############# ######"
                    "############# #    #"
                    "############# ######"
                    "############# #    #"
                    "############# ######"
                    "############# ######"
                    "############      O#"
                    "####################";
        endrow = 8;
        endcol = 18;
        starrow = 2;
        starcol = 2;
        break;
    }

    maze_t *maze = create_maze(width, height, maze_data, endcol, endrow);

    while (victoria != 1) {
      clear();
      draw_maze(maze);
      mvprintw(row, col, "@");
      if (row == maze->endrow && col == maze->endcol){
        siguientenivel(maze, row, col);
        level++;
        row = maze->starrow;
        col = maze->starcol;
        break;
      }
      refresh();
      handle_input(maze, &row, &col);
    }
  }

  endwi89n();
  return 0;
}
//carlos (T04D) 665216275 


