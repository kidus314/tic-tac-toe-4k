#include <Adafruit_NeoPixel.h>

#define neo_data_in 6
#define numpixel 13
#define game_mode 12
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numpixel, neo_data_in, NEO_GRB + NEO_KHZ800);

int buttonPins[9] = { 2, 3, 4, 5, 7, 8, 9, 10, 11 };
char game_board[3][3] = { { '-', '-', '-' }, { '-', '-', '-' }, { '-', '-', '-' } };
int current_player;
int button_index;

char winner_val;
int reset_pin = A2;
int button_state;
int button_pressed = LOW;
int current_state = 0;
int difficulty = A3;
int current_difficulty_value;
int past_difficulty_value;
uint32_t green = 0x00FF99;
uint32_t purple = 0xCC00CC;

//###########################################################################
void setup() {
  for (int i = 0; i < 9; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(reset_pin, INPUT_PULLUP);
  pinMode(difficulty, INPUT_PULLUP);
  pinMode(game_mode, INPUT_PULLUP);
  pinMode(neo_data_in, OUTPUT);
  pinMode(A0, INPUT);
  randomSeed(analogRead(A0));
  strip.begin();
  strip.show();
  // current_player = random(2);
  current_player = 0;
  Serial.begin(9600);
}

//###########################################################################
void loop() {

  button_state = digitalRead(game_mode);

  if (button_state == LOW && button_pressed == LOW) {




    current_state++;



    button_pressed = HIGH;
    Serial.println("button_state     ");
    Serial.print(button_state);
    Serial.println("button_pressed    ");
    Serial.print(button_pressed);
    Serial.print("current state ");
    Serial.print(current_state);

    delay(200);
  } else {
  }

  current_state % 2 == 0 ? multi_player() : single_player();

  if (analogRead(reset_pin) <= 600) {

    reset();

    delay(1000);
  }
}

//###########################################################################
void multi_player() {
  for (int i = 0; i < 9; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      int index = i;
      char result = check_results(false);




      if (result == '-' && game_board[index / 3][index % 3] == '-') {

        update_leds(index, false);

        update_board(index);
        toggle();
      } else {
        update_leds(index, true);
      }
    }
  }
}

//###########################################################################
void single_player() {

  int index;
  char result = check_results(false);
  int difficulty_level = analogRead(difficulty);

  Serial.println("difficulty  :");
  Serial.print(difficulty_level);


  if (difficulty_level >= 800 && difficulty_level <= 1024) {
    while (result == '-' && difficulty_level >= 800 && difficulty_level <= 1024) {
      difficulty_level = analogRead(difficulty);
      strip.setPixelColor(10, strip.Color(0x00, 0xFF, 0x00));
      strip.show();
      if (current_player == 1 && game_board[0][0] == '-' && game_board[0][1] == '-' && game_board[0][2] == '-' && game_board[1][0] == '-' && game_board[1][1] == '-' && game_board[1][2] == '-' && game_board[2][0] == '-' && game_board[2][1] == '-' && game_board[2][2] == '-') {
        int random_index = random(9);  // Generate a random index
        while (game_board[random_index / 3][random_index % 3] != '-') {
          random_index = random(9);
        }

        update_leds(random_index, false);
        update_board(random_index);

        toggle();  //////////////////////checkthis
      } else easy_ai_move();
    }
  }
  if (difficulty_level >= 230 && difficulty_level <= 750) {
    while (result == '-') {
      strip.setPixelColor(10, strip.Color(0x00, 0x00, 0x00));

      strip.setPixelColor(11, strip.Color(0xFF, 0xFF, 0x00));
      strip.show();
      if (current_player == 1 && game_board[0][0] == '-' && game_board[0][1] == '-' && game_board[0][2] == '-' && game_board[1][0] == '-' && game_board[1][1] == '-' && game_board[1][2] == '-' && game_board[2][0] == '-' && game_board[2][1] == '-' && game_board[2][2] == '-') {
        int random_index = random(9);  // Generate a random index
        while (game_board[random_index / 3][random_index % 3] != '-') {
          random_index = random(9);
        }

        update_leds(random_index, false);
        update_board(random_index);

        toggle();  //////////////////////checkths
      }
      medium_ai_move();
    }
  }
  if (difficulty_level >= 0 && difficulty_level <= 200) {
    while (result == '-') {
      strip.setPixelColor(10, strip.Color(0x00, 0x00, 0x00));

      strip.setPixelColor(12, strip.Color(0xFF, 0x00, 0x00));
      strip.show();

      if (current_player == 1 && game_board[0][0] == '-' && game_board[0][1] == '-' && game_board[0][2] == '-' && game_board[1][0] == '-' && game_board[1][1] == '-' && game_board[1][2] == '-' && game_board[2][0] == '-' && game_board[2][1] == '-' && game_board[2][2] == '-') {
        int random_index = random(9);  // Generate a random index
        while (game_board[random_index / 3][random_index % 3] != '-') {
          random_index = random(9);
        }

        update_leds(random_index, false);
        update_board(random_index);

        toggle();  //////////////////////checkths
      }
      hard_ai_move();
    }
  } else {
    update_leds(index, true);
  }
}

//###########################################################################
void update_leds(int position, bool win) {
  char win_value = check_results(false);
  if (!win) {

    uint32_t color = (current_player == 0) ? purple : green;
    uint32_t invertcolor = (color == purple) ? green : purple;
    strip.setPixelColor(position, color);
    strip.setPixelColor(9, invertcolor);
    strip.show();
  } else {
    if (win_value == 'D') {

      strip.fill(purple, 0, 12);
      strip.show();
      delay(2000);
      strip.fill(green, 0, 12);
      strip.show();
    }

    else {
      if (win_value == 'X') {
        for (int i = 0; i < 9; i++) {
          if (i % 2 == 0) {
            strip.setPixelColor(i, green);
            strip.show();
          } else {
            strip.setPixelColor(i, 0);  // Turn off other LEDs

            strip.show();
          }
        }
      } else if (win_value == 'O') {
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            if (i != 1 || j != 1) {
              strip.setPixelColor(i * 3 + j, purple);
              strip.show();
            } else {
              strip.setPixelColor(i * 3 + j, 0);
              strip.show();  // Turn off center LED
            }
          }
        }
      }

      // Adjust the delay time as needed
    }
  }
}


//###########################################################################
void update_board(int position) {

  if (current_player == 1 && game_board[position / 3][position % 3] == '-') {

    game_board[position / 3][position % 3] = 'X';

    for (int i = 0; i < 9; i++) {
      Serial.print(game_board[i / 3][i % 3]);
    }
  }

  else if (current_player == 0 && game_board[position / 3][position % 3] == '-') {

    game_board[position / 3][position % 3] = 'O';
    for (int i = 0; i < 9; i++) {
      Serial.print(game_board[i / 3][i % 3]);
    }
  }
}




//###########################################################################
bool win() {
  // checking the horizontal
  for (int i = 0; i < 3; i++) {

    if (game_board[i][0] == game_board[i][1] && game_board[i][1] == game_board[i][2] && game_board[i][0] != '-') {

      winner_val = game_board[i][0];
      return true;
    }
  }

  for (int i = 0; i < 3; i++) {

    if (game_board[0][i] == game_board[1][i] && game_board[1][i] == game_board[2][i] && game_board[0][i] != '-') {

      winner_val = game_board[0][i];
      return true;
    }
  }

  if (game_board[0][0] == game_board[1][1] && game_board[1][1] == game_board[2][2] && game_board[0][0] != '-') {
    winner_val = game_board[0][0];
    return true;
  }

  if (game_board[2][0] == game_board[1][1] && game_board[1][1] == game_board[0][2] && game_board[0][2] != '-') {
    winner_val = game_board[2][0];
    return true;
  }
  return false;
}



//###########################################################################
bool draw() {

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (game_board[i][j] == '-') {

        return false;
      }
    }
  }

  return true;
}



//###########################################################################
char check_results(bool reset) {
  if (!reset) {
    if (win()) {

      return winner_val;
    }

    else if (draw()) {
      return 'D';
    }

    else
      return '-';
  }

  return '-';
}





//###########################################################################
int mini_max(char board[3][3], int depth, bool is_maximizing) {
    char result = check_results(false);

    if (result == 'X') {
        return 10 - depth;
    } else if (result == 'O') {
        return depth - 10;
    } else if (result == 'D') {
        return 0;
    }

    if (is_maximizing) {
        int best_score = -1000;
        int best_move = -1;
        for (int i = 0; i < 9; i++) {
            if (board[i / 3][i % 3] == '-') {
                board[i / 3][i % 3] = 'X'; // AI places 'X'
                int score = mini_max(board, depth + 1, false);
                board[i / 3][i % 3] = '-';
                if (score > best_score) {
                    best_score = score;
                    best_move = i;
                }
            }
        }
        if (depth == 0) {
            return best_move;
        } else {
            return best_score;
        }
    } else {
        int best_score = 1000;
        int best_move = -1;
        for (int i = 0; i < 9; i++) {
            if (board[i / 3][i % 3] == '-') {
                board[i / 3][i % 3] = 'O'; // Player places 'O'
                int score = mini_max(board, depth + 1, true);
                board[i / 3][i % 3] = '-';
                if (score < best_score) {
                    best_score = score;
                    best_move = i;
                }
            }
        }
        if (depth == 0) {
            return best_move;
        } else {
            return best_score;
        }
    }
}

void handle_ai_move() {
    int best_move = mini_max(game_board, 0, true); // AI is maximizing
    if (best_move != -1) {
        update_leds(best_move, false);
        game_board[best_move / 3][best_move % 3] = 'X';
    }
}

// gemini###############


//###########################################################################


void easy_ai_move() {
  int index;
  char result;


  // result = check_results(false);

  // Player's turn





  if (current_player == 0) {
    for (int i = 0; i < 9; i++) {
      if (digitalRead(buttonPins[i]) == LOW && game_board[i / 3][i % 3] == '-') {
        // Update LEDs and game board for the player's move
        update_leds(i, false);
        update_board(i);
        toggle();  // Toggle to AI's turn
                   // Exit loop after handling player's move
      }
    }
  }
  if (current_player == 1) {
    // AI's turn
    index = random(9);  // Generate a random index
    while (game_board[index / 3][index % 3] != '-') {
      index = random(9);  // Keep generating until finding an empty cell
    }
    // Update LEDs and game board for the AI's move
    update_board(index);
    update_leds(index, false);
    toggle();
    // Toggle to player's turn
    result = check_results(false);
  }
  if (result != '-') {

    update_leds(index, true);
  }
  // Check for game result after player's or AI's move
}


//###########################################################################
void medium_ai_move() {
  int index;
  char result = check_results(false);

  //   Serial.println("current player : in medium");

  //   Serial.println(current_player);
  // delay(300);

  // Player's turn
  if (current_player == 0) {
    for (int i = 0; i < 9; i++) {
      if (digitalRead(buttonPins[i]) == LOW && game_board[i / 3][i % 3] == '-') {
        index = i;
        update_leds(index, false);
        update_board(index);
        Serial.print(game_board[i / 3][i % 3]);

        toggle();
      }
    }
  }

  // AI's turn
  if (current_player == 1) {
    for (int i = 0; i < 9; i++) {
      if (game_board[i / 3][i % 3] == '-') {
        game_board[i / 3][i % 3] = 'X';  // Try placing O in an empty cell
        if (win()) {
          index = i;
          update_leds(index, true);
          update_board(index);
          toggle();  // Toggle to player's turn
          return;
        } else
          game_board[i / 3][i % 3] = '-';
        break;  // Undo the move
      }
    }

    index = random(9);
    while (game_board[index / 3][index % 3] != '-') {
      index = random(9);
    }

    update_board(index);
    update_leds(index, false);
    toggle();
  }

  if (result != '-') {

    update_leds(index, true);
  }
}


// //###########################################################################
// void hard_ai_move() {
//   char result;
//   int index;
//   if(result!='-'){
//   if (current_player == 0 ) {


//     for (int i = 0; i < 9; i++) {
//       if (digitalRead(buttonPins[i]) == LOW) {
//         index = i;
//         if (game_board[index / 3][index % 3] == '-') {
//           // Update LEDs and game board for the player's move
//           update_leds(index, false);
//           update_board(index);
//           toggle();
//         }
//       }
//     }

//   }
//   int count=0 ;
//    for (int i = 0; i < 9; i++) {

//     if (game_board[i / 3][i % 3] != '-'){
//       count++;
//     }
//    }

//   if (current_player == 1 && count==1 ){
//       int random_index = random(9);  // Generate a random index
//           while (game_board[random_index / 3][random_index % 3] != '-') {
//             random_index = random(9);
//           }

//           update_leds(random_index, false);
//           update_board(random_index);

//           toggle();


//            }
//   if (current_player == 1 && count!=1 ) {
// // if (current_player == 1 && game_board[0][0] == '-' && game_board[0][1] == '-' && game_board[0][2] == '-' && game_board[1][0] == '-' && game_board[1][1] == '-' && game_board[1][2] == '-' && game_board[2][0] == '-' && game_board[2][1] == '-' && game_board[2][2] == '-') {
// //           int random_index = random(9);  // Generate a random index
// //           while (game_board[random_index / 3][random_index % 3] != '-') {
// //             random_index = random(9);
// //           }
//     // Handle AI move
//     handle_ai_move();
//     toggle();  // player's turn
//   }}


// result = check_results(false);
//    if(result!='-'){

//   update_leds(index, true);
// }
// }


void hard_ai_move() {
    char result = check_results(false);
    if (result != '-') {
        update_leds(-1, true);
        return;
    }

    if (current_player == 0) {
        for (int i = 0; i < 9; i++) {
            if (digitalRead(buttonPins[i]) == LOW) {
                int index = i;
                if (game_board[index / 3][index % 3] == '-') {
                    update_leds(index, false);
                    update_board(index);
                    toggle();
                }
            }
        }
    } else {
        handle_ai_move(); // AI Move using minimax
        toggle();
    }

    result = check_results(false);
    if (result != '-') {
        update_leds(-1, true);
    }
}
// ###############################
//###########################################################################
void toggle() {
  current_player = (current_player == 0) ? 1 : 0;
  Serial.print("Toggling to player ");
  Serial.println(current_player);
}






//###########################################################################
void reset() {
  // Reset the game board
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      game_board[i][j] = '-';
    }
  }
  current_state = 0;
  Serial.println("Game board reset");

  // Update the LED strip to turn off all LEDs
  for (int i = 0; i < numpixel; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}