//
// AED, August 2022 (Tomás Oliveira e Silva)
//
// First practical assignement (speed run)
//
// Compile using either
//   cc -Wall -O2 -D_use_zlib_=0 solution_speed_run.c -lm
// or
//   cc -Wall -O2 -D_use_zlib_=1 solution_speed_run.c -lm -lz
//
// Place your student numbers and names here
//   N.Mec. XXXXXX  Name: XXXXXXX
//


//
// static configuration
//

#define _max_road_size_  800  // the maximum problem size
#define _min_road_speed_   2  // must not be smaller than 1, shouldnot be smaller than 2
#define _max_road_speed_   9  // must not be larger than 9 (only because of the PDF figure)


//
// include files --- as this is a small project, we include the PDF generation code directly from make_custom_pdf.c
//

#include <math.h>
#include <stdio.h>
#include "../P02/elapsed_time.h"
#include "make_custom_pdf.c"


//
// road stuff
//

static int max_road_speed[1 + _max_road_size_]; // positions 0.._max_road_size_

static void init_road_speeds(void)
{
  double speed;
  int i;

  for(i = 0;i <= _max_road_size_;i++)
  {
    speed = (double)_max_road_speed_ * (0.55 + 0.30 * sin(0.11 * (double)i) + 0.10 * sin(0.17 * (double)i + 1.0) + 0.15 * sin(0.19 * (double)i));
    max_road_speed[i] = (int)floor(0.5 + speed) + (int)((unsigned int)random() % 3u) - 1;
    if(max_road_speed[i] < _min_road_speed_)
      max_road_speed[i] = _min_road_speed_;
    if(max_road_speed[i] > _max_road_speed_)
      max_road_speed[i] = _max_road_speed_;
  }
}


//
// description of a solution
//

typedef struct
{
  int n_moves;                         // the number of moves (the number of positions is one more than the number of moves)
  int positions[1 + _max_road_size_];  // the positions (the first one must be zero)
}
solution_t;


//
// the (very inefficient) recursive solution given to the students
//

static solution_t solution_1,solution_1_best;
static double solution_1_elapsed_time; // time it took to solve the problem
static unsigned long solution_1_count; // effort dispended solving the problem

static void solution_1_recursion(int move_number,int position,int speed,int final_position)
{
  int i,new_speed;

  // record move
  solution_1_count++;
  solution_1.positions[move_number] = position;



  // is it a solution?
  if(position == final_position && speed == 1)
  {
    // is it a better solution?
    if(move_number < solution_1_best.n_moves)
    {
      solution_1_best = solution_1;
      solution_1_best.n_moves = move_number;
    }
    return;
  }





  // no, try all legal speeds
  for(new_speed = speed - 1;new_speed <= speed + 1;new_speed++) {
    if(new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for(i = 0;i <= new_speed && new_speed <= max_road_speed[position + i];i++);
      if(i > new_speed)
        solution_1_recursion(move_number + 1,position + new_speed,new_speed,final_position);
    }
  }
}


static void solve_1(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_1: bad final_position\n");
    exit(1);
  }
  solution_1_elapsed_time = cpu_time();
  solution_1_count = 0ul;
  solution_1_best.n_moves = final_position + 100;
  solution_1_recursion(0,0,0,final_position);
  solution_1_elapsed_time = cpu_time() - solution_1_elapsed_time;
}



//
//  FUNC OLDER
//


static solution_t solution_2,solution_2_best;
static double solution_2_elapsed_time; // time it took to solve the problem
static unsigned long solution_2_count; // effort dispended solving the problem

static void solution_2_recursion(int move_number,int position,int speed,int final_position) {
  
  // Este é o código simples que verifica se vale a pena continuar o ramo ou não
  // Aqui ou depois de is it a solution??                                    // maxVelocidade[position] != 0
  // solution_2_count >= solution_2_best.n_moves

    if (solution_2.positions[solution_2_best.n_moves] != 0) {
        return;
    }


  int i,new_speed;

  // record move
  solution_2_count++;
  solution_2.positions[move_number] = position;

  // is it a solution?
  if(position == final_position && speed == 1) {
    // this solution is always the best
    solution_2_best = solution_2;
    solution_2_best.n_moves = move_number;
    return;
  }


  // no, try all legal speeds
  for(new_speed = speed + 1;new_speed >= speed - 1;new_speed--) {
      //new_speed >= 1 &&
    if( new_speed <= max_road_speed[position + new_speed] && position + new_speed <= final_position) {

      for(i = 0;i <= new_speed && new_speed <= max_road_speed[position + i];i++);
            
      if(i > new_speed) {
          // Como o ramo continuou, quer dizer que o numero de passos e a velocidade são o menor e mais rápida respetivamente,
          // logo podemos atualizar os valores do numero de saltos minimos e velocidade máxima desta position.
          solution_2_recursion(move_number + 1,position + new_speed,new_speed,final_position);
      }
    }
  }
}

static void solve_2(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_1: bad final_position\n");
    exit(1);
  }  
  memset( solution_2.positions, 0, final_position*sizeof(solution_2.positions[0]));
  solution_2_elapsed_time = cpu_time();
  solution_2_count = 0ul;
  solution_2_best.n_moves = final_position + 100;
  solution_2_recursion(0,0,0,final_position);
  solution_2_elapsed_time = cpu_time() - solution_2_elapsed_time;
}


//
//  FUNC FINAL
//


static solution_t solution_3,solution_3_best;
static double solution_3_elapsed_time; // time it took to solve the problem
static unsigned long solution_3_count; // effort dispended solving the problem

static int minSaltos[_max_road_size_];          // Array com o numero mínimo de passos precisos para chegar a cada posição
                                                // Ex: se chegar à posição 14 em 5 saltos, minSaltos[14] = 5;
                                                // Se nouta iteração chegar em 7 saltos, o programa acaba com o ramo;
                                                // Se chegar com 4 saltos o programa começa a usar esse ramo com o principal;

static int maxVelocidade[_max_road_size_];      // Mesma coisa do que o Array de cima, mas desta vez conta a velocidade a que se chega 
                                                // a cada posição. Só serve para "desempatar" ramos que podem ter chegado com o mesmo
                                                // número de saltos mas diferentes velocidades.
                                                // Ex: Se 2 ramos chegarem com 6 saltos à posição 16, o que tiver mais velocidade contínua,
                                                // caso sejam a mesma, ambos ramos continuam até se desempatarem noutra posição


static void solution_3_recursion(int move_number,int position,int speed,int final_position) {
  

  int i,new_speed;

  // record move
  solution_3_count++;
  solution_3.positions[move_number] = position;

  // Ver se é solução. Neste código chegar a uma solução implica que é sempre a melhor, pois náo foi cortada antes de lá chegar
  if(position == final_position && speed == 1) {
    // this solution is always the best
    solution_3_best = solution_3;
    solution_3_best.n_moves = move_number;
    return;
  }

  // Como náo é solução, podemos continuar o código
  for(new_speed = speed + 1;new_speed >= speed - 1;new_speed--) {

    if(new_speed > 0 && new_speed <= max_road_speed[position + new_speed] && position + new_speed <= final_position) {
      

      for(i = 0;i <= new_speed && new_speed <= max_road_speed[position + i];i++);


      if(i > new_speed) {
      
        //  Este é o código simples que verifica se vale a pena continuar o ramo ou não
        //  Apenas começa a cortar ramos se já houver pelo menos uma solução (assim garantimos sempre uma solução, 
        // sem isto pode haver problemas para alguns final_positions)

        //  Como o primeiro ramo vai sempre pelas velocidades mais altas possíveis (menos no fim), 
        // há uma grande chance de ser o melhor, mas não podemos assumir isso.

        //  O ramo é cortado caso já se tenha passado pela mesma posição com um número menor de 
        // movimentos ou maior velocidade num ramo anterior, logo qualquer solução nova é 
        // intrínsecamente melhor que a anterior, pois não foi cortada até chegar ao fim.
    
        if (solution_3.positions[solution_3_best.n_moves] != 0){
          if (new_speed <= maxVelocidade[position+new_speed] &&
              move_number+1 >= minSaltos[position+new_speed]) {
            continue;
          }
        }            

        //  Como o ramo continuou, podemos afirmar que o numero de passos utilizados para chegar a esta posição é o menor até agora e
        // que a velocidade com que lá chegamos é a menor de todas as iterações anteriores, logo podemos atualizar os valores do numero 
        // de saltos minimos e velocidade máxima desta posição e de todas as posições intermédias por que passa-mos.

        // Posição atual
        minSaltos[position] = move_number+1;
        maxVelocidade[position] = speed;

        // Posições intermédias
        for (int n = 1; n < new_speed; n++) {
          minSaltos[position+n] = move_number+1;
          maxVelocidade[position+n] = new_speed;
        }

        // próximos ramos
        solution_3_recursion(move_number + 1,position + new_speed,new_speed,final_position);
      }
    }
  }
}

static void solve_3(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_1: bad final_position\n");
    exit(1);
  }  
  memset( minSaltos, _max_road_size_, final_position*sizeof(minSaltos[0]));
  memset( maxVelocidade, 0, final_position*sizeof(maxVelocidade[0]) );
  memset( solution_3.positions, 0, final_position*sizeof(solution_3.positions[0]));
  solution_3_elapsed_time = cpu_time();
  solution_3_count = 0ul;
  solution_3_best.n_moves = final_position + 100;
  solution_3_recursion(0,0,0,final_position);
  solution_3_elapsed_time = cpu_time() - solution_3_elapsed_time;
}








//
// FUNC KAUATI
//

static solution_t solution_4,solution_4_best;
static double solution_4_elapsed_time; // time it took to solve the problem
static unsigned long solution_4_count; // effort dispended solving the problem

static int saltos[_max_road_size_];  // Array com o numero mínimo de passos precisos para chegar a cada posição
                                                // Ex: se chegar à posição 14 em 5 saltos, minSaltos[14] = 5;
                                                // Se nouta iteração chegar em 7 saltos, o programa acaba com o ramo;
                                                // Se chegar com 4 saltos o programa começa a usar esse ramo com o principal;

static int velocidade[_max_road_size_];     // Mesma coisa do que o Array de cima, mas desta vez conta a velocidade a que se chega 
                                                // a cada posição. Só serve para "desempatar" ramos que podem ter chegado com o mesmo
                                                // número de saltos mas diferentes velocidades.
                                                // Ex: Se 2 ramos chegarem com 6 saltos à posição 16, o que tiver mais velocidade contínua,
                                                // caso sejam a mesma, ambos ramos continuam até se desempatarem noutra posição

static void solve_4(int final_position) {
  if(final_position < 1 || final_position > _max_road_size_) {
    fprintf(stderr,"solve_4: bad final_position\n");
    exit(1);
  }

  solution_4_count = 0ul;
  solution_4_best.n_moves = final_position;
  solution_4_elapsed_time = cpu_time();
  
  //Code after this point :
  //Issue with the iteration verification
  int move_number =  0, curr_position = 0 , i , speed = 0 , positions_till_end = final_position ;

    for( positions_till_end = final_position ; positions_till_end >= 0 ; positions_till_end - speed ){

        if (move_number > 10) {
            break;
        }

      printf("i > %i  positionsTillEnd > %i  speed > %i \n", move_number, positions_till_end, speed);
      //printf("Positions till end : %i \n", positions_till_end); printf("Move number : %i \n", move_number);printf("Speed : %i\n", speed);

      
      solution_1_count++; 
      solution_1.positions[move_number] = curr_position;

      if( (curr_position + speed) == final_position) {
          if (speed == 1) {
            printf("\nIf 1\n");  
            solution_1_best = solution_1;
            solution_1_best.n_moves = solution_1_count;
            break; 
          }
          else {
            printf("\nIf 2\n");  
                break;
          }
      }

      else {
        if (positions_till_end > _max_road_speed_) {
            if (speed < _max_road_speed_) {
                printf("\nIf 3\n");  
                speed++;
                move_number ++ ;
                curr_position = curr_position + speed ;
            }
        }

        else {
            if(curr_position + speed == final_position) {
                printf("\nif 4\n");
                for(i = 0;i <= speed+1 && speed+1 <= max_road_speed[curr_position+i];i++);
                if(i > speed+1) {
                    speed=speed+1;
                }
            }

            else if( speed > 1 && curr_position + speed <= final_position){
                printf("\nif 5\n");
                break;
            }

            else {
                speed=speed+1;
            }
        }     
      }
    
    }
    //the move_number should (supossly) 
    //be incremented after the solution_1.positions[move_number] updating
  
  //solution_1_recursion(0,0,0,final_position);
  solution_4_elapsed_time = cpu_time() - solution_4_elapsed_time;
  return;
}






//
// example of the slides
//

static void example(void)
{
  int i,final_position;

  srandom(0xAED2022);
  init_road_speeds();
  final_position = 30;
  solve_1(final_position);
  make_custom_pdf_file("example.pdf",final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
  printf("mad road speeds:");
  for(i = 0;i <= final_position;i++);
  printf(" %d",max_road_speed[i]);
  printf("\n");
  printf("positions:");
  for(i = 0;i <= solution_1_best.n_moves;i++)
    printf(" %d",solution_1_best.positions[i]);
  printf("\n");
}


//
// main program
//

int main(int argc,char *argv[argc + 1])
{
# define _time_limit_  3600.0
  int n_mec,final_position,print_this_one;
  char file_name[64];

  // generate the example data
  if(argc == 2 && argv[1][0] == '-' && argv[1][1] == 'e' && argv[1][2] == 'x')
  {
    example();
    return 0;
  }
  // initialization
  n_mec = (argc < 2) ? 0xAED2022 : atoi(argv[1]);
  srandom((unsigned int)n_mec);
  init_road_speeds();
  // run all solution methods for all interesting sizes of the problem
  final_position = 1;
  solution_1_elapsed_time = 0.0;

  printf("      ╭─────────────────────────────────╮\n");
  printf("      │                plain recursion  │\n");
  printf(" ╭────┼──────────┬──────────┬───────────┤\n");
  printf(" │  n │ sol      │    count │  cpu time │\n");
  printf(" │────┼──────────┼──────────┼───────────┤\n");

  int sol = 3;

  while(final_position <= _max_road_size_/* && final_position <= 20*/)
  {
    print_this_one = (final_position == 10 || final_position == 20 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
    printf(" │%3d │",final_position);
    // first solution method (very bad)

    


    if(sol == 1) {
        if(solution_1_elapsed_time < _time_limit_)
        {
        solve_1(final_position);
        if(print_this_one != 0)
        {
            sprintf(file_name,"%03d_1.pdf",final_position);
            make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
        }
        printf(" %3d %16lu %9.3e │",solution_1_best.n_moves,solution_1_count,solution_1_elapsed_time);
        }
        else
        {
        solution_1_best.n_moves = -1;
        printf("                                |");
        }
    }
    
    if(sol == 2) {

        if(solution_2_elapsed_time < _time_limit_)
        {
        solve_2(final_position);
        if(print_this_one != 0)
        {
            sprintf(file_name,"%03d_1.pdf",final_position);
            make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_2_best.n_moves,&solution_2_best.positions[0],solution_2_elapsed_time,solution_2_count,"Plain recursion");
        }
        printf(" %3d %16lu %9.3e |",solution_2_best.n_moves,solution_2_count,solution_2_elapsed_time);
        }
        else
        {
        solution_2_best.n_moves = -1;
        printf("                                |");
        }
    }

    if(sol == 3) {

        if(solution_3_elapsed_time < _time_limit_)
        {
        solve_3(final_position);
        if(print_this_one != 0)
        {
            sprintf(file_name,"%03d_1.pdf",final_position);
            make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_3_best.n_moves,&solution_3_best.positions[0],solution_3_elapsed_time,solution_3_count,"Plain recursion");
        }
        printf(" %8d │ %8lu │ %9.3e │",solution_3_best.n_moves,solution_3_count,solution_3_elapsed_time);
        }
        else
        {
        solution_3_best.n_moves = -1;
        printf("                                 │");
        }
    }

    if(sol == 4) {

        if(solution_4_elapsed_time < _time_limit_)
        {
        solve_4(final_position);
        if(print_this_one != 0)
        {
            sprintf(file_name,"%03d_1.pdf",final_position);
            make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_4_best.n_moves,&solution_4_best.positions[0],solution_4_elapsed_time,solution_4_count,"Plain recursion");
        }
        printf(" %3d %16lu %9.3e |",solution_4_best.n_moves,solution_4_count,solution_4_elapsed_time);
        }
        else
        {
        solution_4_best.n_moves = -1;
        printf("                                |");
        }
    }

    // done
    printf("\n");
    fflush(stdout);
    // new final_position
    if(final_position < 50)
      final_position += 1;
    else if(final_position < 100)
      final_position += 5;
    else if(final_position < 200)
      final_position += 10;
    else
      final_position += 20;
  }
  printf(" ╰────┴──────────┴──────────┴───────────╯\n");
  return 0;
# undef _time_limit_
}
