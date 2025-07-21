//=================================================================
// The main program file.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

// External libs
#include <cstdio>
#include <stdlib.h>
// Use these macros if you wanna read timer values
#define timer_read_f(x) chrono::duration<float>((x).elapsed_time()).count()
#define timer_read_s(x)                                                        \
  chrono::duration_cast<chrono::seconds>((x).elapsed_time()).count();
#define timer_read_ms(x)                                                       \
  chrono::duration_cast<chrono::milliseconds>((x).elapsed_time()).count()
#define timer_read_us(x) (x).elapsed_time().count()

// Project includes
//#include "enemy_private.h"
#include "enemy_public.h"
#include "globals.h"
#include "hardware.h"
#include "mbed.h"
#include "player_public.h"

#include "rtos.h"

// void playSound(char *wav) {
//   // open wav file
//   FILE *wave_file;
//   int count = 0;

// try_open:
//   wave_file = fopen(wav, "r");

//   if (wave_file != NULL) {
//     printf("File opened successfully\n");

//     // play wav file
//     printf("Sound playing...\n");
//     waver.play(wave_file);

//     // close wav file
//     printf("Sound stopped...\n");
//     fclose(wave_file);
//     return;
//   } else {
//     printf("Could not open file for reading - %s\n", wav);
//     count++;
//     if (count == 4) {
//       printf("File could not be opened. Skipping audio\n");
//       return;
//     } else
//       goto try_open;
//   }

//   return;
// }
int lang = 1;
int difficulty = 1;
int gameModeVar = 1;

void setDifficulty() {

  SetEnemySpeed_SpeedOfShooting(difficulty + 1, 100 / difficulty);
}

void set_random_seed(Timer t) {
  GameInputs inputs;
  t.start();
  uLCD.color(0xFFFFFF);
  //   uLCD.printf("Push the nav switch to start.\n");
  draw_main_screen(lang);
  bool ins = false;
  while (1) {
    inputs = read_inputs();
    if (inputs.fire) {
      break;
    } else if (!inputs.button3) {
      ins = true;
      break;
    }
  }
  if (ins) {
    uLCD.cls();
    while (1) {
      print_inst(lang);
      inputs = read_inputs();
      if (inputs.fire) {
        break;
      }
      if (!inputs.button3) {
        if (lang < 3) {
          lang++;
        } else {
          lang = 1;
        }
        uLCD.cls();
        continue;
      }
    }
  }

  uLCD.cls();
  char chosen1, chosen2, chosen3;
  while (1) {
    chosen1 = (difficulty == 1) ? 'V' : ' ';
    chosen2 = (difficulty == 2) ? 'V' : ' ';
    chosen3 = (difficulty == 3) ? 'V' : ' ';
    uLCD.locate(0, 0);
    switch (lang) {
    case 1: // English
      uLCD.printf("Easy mode: %c \n", chosen1);
      uLCD.printf("Medium mode: %c \n", chosen2);
      uLCD.printf("Hard mode: %c \n", chosen3);
      break;
    case 2: // Spanish
      uLCD.printf("Modo fácil: %c \n", chosen1);
      uLCD.printf("Modo medio: %c \n", chosen2);
      uLCD.printf("Modo difícil: %c \n", chosen3);
      break;
    case 3: // Portuguese
      uLCD.printf("Modo fácil: %c \n", chosen1);
      uLCD.printf("Modo médio: %c \n", chosen2);
      uLCD.printf("Modo difícil: %c \n", chosen3);
      break;
    }

    inputs = read_inputs();
    if (inputs.fire) {
      break;
    } else if (!inputs.button3) {
      if (difficulty < 3) {
        difficulty++;
      } else {
        difficulty = 1;
      }
    }
    wait_us(100000);
  }
  uLCD.cls();
  setDifficulty();
  while (1) {
    chosen1 = (gameModeVar == 1) ? 'V' : ' ';
    chosen2 = (gameModeVar == 2) ? 'V' : ' ';
    uLCD.locate(0, 0);
    switch (lang) {
    case 1:
      uLCD.printf(
          "Select Game Mode: \n Best time of 5 points: %c \n Survival: %c",
          chosen1, chosen2);
      break;
    case 2:
      uLCD.printf("Selecciona el Modo de Juego: \n Mejor tiempo de 5 puntos: "
                  "%c \n Supervivencia: %c",
                  chosen1, chosen2);
      break;
    case 3:
      uLCD.printf("Selecione o Modo de Jogo: \n Melhor tempo de 5 pontos: %c "
                  "\n Sobrevivencia: %c",
                  chosen1, chosen2);
      break;
    }
    inputs = read_inputs();
    if (inputs.fire) {
      break;
    } else if (!inputs.button3) {
      if (gameModeVar == 2) {
        gameModeVar = 1;
      } else {
        gameModeVar = 2;
      }
    }
    wait_us(10000);
  }
  if (gameModeVar == 2) {
    difficulty = 1;
    setDifficulty();
  }
  uLCD.cls();
  t.stop();
  int seed = timer_read_ms(t);
  srand(seed);
}

bool crash() {
  if (get_enemy_list() == nullptr || getHead(get_enemy_list()) == nullptr) {
    return false;
  }
  llnode_t *current = getHead(get_enemy_list());
  llnode_t *next;
  ENEMY *enemy;
  while (current) {
    next = getNext(current);
    enemy = (ENEMY *)(current->data);
    if ((enemy->box.bottomRight.x >= player_get_info().x) &&
        (enemy->box.topLeft.x <=
         player_get_info().x + player_get_info().height) &&
        (enemy->box.bottomRight.y >= player_get_info().y) &&
        (enemy->box.topLeft.y <=
         player_get_info().y + player_get_info().height)) {
      // printf("CRASH\n");
      player_score_inc();
      return true;
    }
    current = next;
  }
  return false;
}
bool projectile_bye_bye() {
  if (get_projectile_list() == nullptr ||
      getHead(get_projectile_list()) == nullptr) {
    return false;
  }
  llnode_t *current = getHead(get_projectile_list());
  llnode_t *next;
  PROJECTILE *p;
  while (current) {
    next = getNext(current);
    p = (PROJECTILE *)(current->data);
    if ((p->box.bottomRight.x >= player_get_info().x) &&
        (p->box.topLeft.x <= player_get_info().x + player_get_info().height) &&
        (p->box.bottomRight.y >= player_get_info().y) &&
        (p->box.topLeft.y <= player_get_info().y + player_get_info().height)) {
      // printf("DEAD!\n");
      player_score_inc();
      return true;
    }
    current = next;
  }
  return false;
}

void player_missile_hit() {

  if (get_enemy_list() == nullptr || getHead(get_enemy_list()) == nullptr) {
    return;
  }
  llnode_t *current = getHead(get_enemy_list());
  llnode_t *next;
  ENEMY *enemy;
  while (current) {
    next = getNext(current);
    enemy = (ENEMY *)(current->data);
    int topXe = enemy->box.topLeft.x;
    int topYe = enemy->box.topLeft.y;
    int botXe = enemy->box.bottomRight.x;
    int botYe = enemy->box.bottomRight.y;
    MISSILE missileInfo = missile_get_info();
    int topXm = missileInfo.box.topLeft.x;
    int topYm = missileInfo.box.topLeft.y;
    int botXm = missileInfo.box.bottomRight.x;
    int botYm = missileInfo.box.bottomRight.y;

    if (botXm > topXe && topXm < botXe && botYm > topYe && topYm < botYe) {
      draw_nothing(enemy->box);
      deleteNode(get_enemy_list(), current);
      //  printf("HIT\n");
      player_missile_return();
      player_score_inc();
      return;
    }
    current = next;
  }
  if (missile_get_info().box.topLeft.y <= 0) {
    player_missile_return();
  }
}

// ===User implementations start===

int main() {
  bool winner;
  while (1) {
    GameInputs inputs;
    player_score_reset();
    winner = false;
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    printf("Program Starting\n");

    // ===User implementations start===
    // Game state variables
    // Timer to measure game update speed (secondarily used to generate
    // random seed)
    Timer t;
    Timer theta;
    float seconds;
    int dt; // delta time
    // set seed for random enemy generation
    set_random_seed(t);
    // initialize functions
    theta.reset();
    theta.start();
    enemy_init();
    player_init();
    printf("Initialization complete\n");
    //   printf("mounting %d\n", fs.mount(&sd_block));
    //   playSound("/fs/Prelude1.wav");
    while (1) {

      t.start();
      seconds = theta.elapsed_time().count() / 1000000.0;
      // Draw enemys first
      enemy_generator();
      // projectile_update_position();
      // Put score on screen
      // NOTE: Text is 8 pixels tall
      //   PLAYER player = player_get_info();
      //   MISSILE missile = missile_get_info();
      uLCD.locate(0, 0);
      uLCD.color(0xFFFFFF);
      switch (lang) {
      case 1: // English
        uLCD.printf("Score: %i Time:%.1f s\n", player_get_info().score,
                    seconds);
        break;
      case 2: // Spanish
        uLCD.printf("Puntuación: %i Tiempo:%.1f s\n", player_get_info().score,
                    seconds);
        break;
      case 3: // Portuguese
        uLCD.printf("Pontuação: %i Tiempo: %.1f s\n", player_get_info().score,
                    seconds);
        break;
      }
      // player_draw(0x0000FF);
      player_draw_sprite();
      // read inputs
      inputs = read_inputs();
      // printf("player motions\n");
      if (inputs.up) {
        player_draw(0x000000);
        player_moveUp();
      }
      // fill the rest of it out
      if (inputs.left) {
        player_draw(0x000000);
        player_moveLeft();
      }
      if (inputs.right) {
        player_draw(0x000000);
        player_moveRight();
      }
      if (inputs.down) {
        player_draw(0x000000);
        player_moveDown();
      }
      if (player_get_info().y != PLAYER_INIT_Y ||
          player_get_info().x != PLAYER_INIT_Y && inputs.fire) {

        player_throw();
      }
      if (!inputs.button3) {
        uLCD.cls();
        uLCD.locate(0, 0);
        switch (lang) {
        case 1: // English
          uLCD.printf("PAUSED\n Press fire to continue");
          break;
        case 2: // Spanish
          uLCD.printf("PAUSADO\n Presione disparar para continuar");
          break;
        case 3: // Portuguese
          uLCD.printf("PAUSADO\n Pressione fogo para continuar");
          break;
        }
        while (1) {
          inputs = read_inputs();
          if (inputs.fire) {
            uLCD.cls();
            break;
          }
        }
      }
      player_missile_draw_blank();
      player_missile_draw();

      // TODO: Check if missile hit enemy
      player_missile_hit();
      if (player_get_info().score >= 5 && gameModeVar == 1) {
        winner = true;
        break;
      }
      if (gameModeVar == 2) {

        difficulty = 1 + player_get_info().score / 3;
        setDifficulty();
      }
      // Compute update time
      t.reset();

      // draw player again to increase smoothness
      // player_draw(0x0000FF);
      player_draw_sprite();
      if (crash() || projectile_bye_bye()) {
        break;
      }

      // uLCD.filled_rectangle(54, 54, 64, 64, RED);
    }
    printf("out of main loop\n");
    uLCD.cls();
    destroyList(get_enemy_list());
    destroyList(get_projectile_list());
    theta.stop();
    theta.reset();
    if (!winner) {
      uLCD.locate(0, 0);
      uLCD.color(0xFFFFFF);
      switch (lang) {
      case 1: // English
        uLCD.printf("GAME OVER :( \n \n Press button 2 to continue \n \n You "
                    "lasted: %.2f s \n \n You scored: %i",
                    seconds, player_get_info().score);
        break;
      case 2: // Spanish
        uLCD.printf("FIN DEL JUEGO :( \n \n Presione botón 2 para continuar \n "
                    "\n Duraste: %.2f s \n \n Puntuacion: %i",
                    seconds, player_get_info().score);
        break;
      case 3: // Portuguese
        uLCD.printf("FIM DE JOGO :( \n \n Pressione botão 2 para continuar \n "
                    "\n Durou: %.2f s \n \n Pontuacao: %i",
                    seconds, player_get_info().score);
        break;
      }
      // printf("GAME OVER :(\n");
      while (1) {
        inputs = read_inputs();
        if (!inputs.button3)
          break;
      }
      uLCD.cls();
      continue;
    } else {
      uLCD.locate(0, 0);
      uLCD.color(0xFFFFFF);
      switch (lang) {
      case 1: // English
        uLCD.printf("You are the GOAT \n Press button 2 to continue");
        break;
      case 2: // Spanish
        uLCD.printf("Eres el MEJOR \n Presione botón 2 para continuar");
        break;
      case 3: // Portuguese
        uLCD.printf("Você é o MELHOR \n Pressione botão 2 para continuar");
        break;
      }

      printf("goated\n");
      while (1) {
        inputs = read_inputs();
        if (!inputs.button3)
          break;
      }
      uLCD.cls();
      continue;
    }
  }
  // destroy more lists as needed
  return 0;
  // ===User implementations end===
}

// ===User implementations start===

// Write your helper methods here to deal with game loop objectives
// Feel free to do whatever you need to implement the game

// /* Wanna hear some annoying sounds?*/
// void playNotes(void)
// {
//     int i;
// // generate a 500Hz tone using PWM hardware output
//     speaker.period(1.0/500.0); // 500hz period
//     speaker =0.5; //50% duty cycle - max volume
//     wait(3);
//     speaker=0.0; // turn off audio
//     wait(2);
// // generate a short 150Hz tone using PWM hardware output
// // something like this can be used for a button click effect for feedback
//     for (i=0; i<10; i++) {
//         speaker.period(1.0/150.0); // 500hz period
//         speaker =0.25; //25% duty cycle - mid range volume
//         wait(.02);
//         speaker=0.0; // turn off audio
//         wait(0.5);
//     }

// // sweep up in frequency by changing the PWM period
//     for (i=0; i<8000; i=i+100) {
//         speaker.period(1.0/float(i));
//         speaker=0.25;
//         wait(.1);
//     }
//     wait(2);

// // two tone police siren effect -  two periods or two frequencies
// // increase volume - by changing the PWM duty cycle
//     for (i=0; i<26; i=i+2) {
//         speaker.period(1.0/969.0);
//         speaker = float(i)/50.0;
//         wait(.5);
//         speaker.period(1.0/800.0);
//         wait(.5);
//     }
//     // decrease volume
//     for (i=25; i>=0; i=i-2) {
//         speaker.period(1.0/969.0);
//         speaker = float(i)/50.0;
//         wait(.5);
//         speaker.period(1.0/800.0);
//         wait(.5);
//     }
//     speaker =0.0;
//     wait(2);
// }

// ===User implementations end===
