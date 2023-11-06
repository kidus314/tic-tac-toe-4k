

char game_board[3][3];
int current_player = 1;  // 1 for 'X' (blue LED), 0 for 'O' (red LED)
int blue_led[3][3] = { { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 } };
int red_led[3][3] = { { 9, 10, 11 }, { 12, 13, 14 }, { 15, 16, 17 } };
int buttons[3][3] = { { 25, 27, 29 }, { 31, 33, 35 }, { 37, 39, 41 } };
int indicator_blue_led = 18;
int indicator_red_led = 19;
int game_mode_button = 20;
int single_player_led = 21;
int multi_player_led = 23;
int reset_button = 43;
char winner;
void setup() {
  // Initialize hardware components and set up pin modes
  Serial.begin(9600);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      pinMode(blue_led[i][j], OUTPUT);
      pinMode(red_led[i][j], OUTPUT);
      pinMode(buttons[i][j], INPUT_PULLUP);
      game_board[i][j] = ' ';
    }
  }

  pinMode(indicator_blue_led, OUTPUT);
  pinMode(indicator_red_led, OUTPUT);
  pinMode(single_player_led, OUTPUT);
  pinMode(multi_player_led, OUTPUT);
  pinMode(game_mode_button, INPUT_PULLUP);
  pinMode(reset_button, INPUT_PULLUP);


  randomSeed(analogRead(A0));
}

void loop() {
  // Game logic
  //ai=0

  int current_player = random(2);
  char gameResult = checkGameResult();

  if (gameResult == ' ') {
    if (digitalRead(game_mode_button) == HIGH) {
      singlePlayerGame();
    } else {
      multiPlayerGame();
    }
  } else {
    handleGameResult(gameResult);
  }

  if (digitalRead(reset_button) == HIGH) {
    resetGame();
  }
}

void singlePlayerGame() {

  if (current_player == 1) {

    handlePlayerMove();
  } else {

    handleAIMove();
  }
  checkGameResult();
  updateLEDs();
}

void multiPlayerGame() {

  handlePlayerMove();
  checkGameResult();
  updateLEDs();
  togglePlayer();
}

void handlePlayerMove() {

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (digitalRead(buttons[i][j]) == LOW && game_board[i][j] == ' ') {
        makeMove(i, j);
      }
    }
  }
}

void handleAIMove() {
  int bestScore = -1000;
  int bestMove = -1;

  for (int i = 0; i < 9; i++) {
    int row = i / 3;
    int col = i % 3;

    if (game_board[row][col] == ' ') {
      game_board[row][col] = 'O';

      int score = minimax(game_board, 0, false);

      game_board[row][col] = ' ';

      if (score > bestScore) {
        bestScore = score;
        bestMove = i;
      }
    }
  }

  if (bestMove != -1) {
    int row = bestMove / 3;
    int col = bestMove % 3;
    makeMove(row, col);
  }
}


void makeMove(int row, int col) {

  if (current_player == 1) {
    game_board[row][col] = 'X';
  } else {
    game_board[row][col] = 'O';
  }
}

char checkGameResult() {




  if (win()) {
    return winner;
  }


  if (draw()) {
    return 'D';
  }


  return ' ';
}



void updateLEDs() {

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (game_board[i][j] == 'X') {
        digitalWrite(blue_led[i][j], HIGH);
        digitalWrite(red_led[i][j], LOW);
      } else if (game_board[i][j] == 'O') {
        digitalWrite(blue_led[i][j], LOW);
        digitalWrite(red_led[i][j], HIGH);
      } else {
        digitalWrite(blue_led[i][j], LOW);
        digitalWrite(red_led[i][j], LOW);
      }
    }
  }
}



void resetGame() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      game_board[i][j] = ' ';
      digitalWrite(blue_led[i][j], LOW);
      digitalWrite(red_led[i][j], LOW);
    }
  }



  current_player = random(2);
}






void togglePlayer() {
  current_player = 1 - current_player;
}







int minimax(char board[3][3], int depth, bool isMaximizing) {
  int result = checkGameResult();
  if (result != -1) {
    return result;
  }

  if (isMaximizing) {
    int bestScore = -1000;
    for (int i = 0; i < 9; i++) {
      int row = i / 3;
      int col = i % 3;

      if (board[row][col] == ' ') {
        board[row][col] = 'O';
        int score = minimax(board, depth + 1, false);
        board[row][col] = ' ';

        bestScore = max(score, bestScore);
      }
    }
    return bestScore;
  } else {
    int bestScore = 1000;
    for (int i = 0; i < 9; i++) {
      int row = i / 3;
      int col = i % 3;

      if (board[row][col] == ' ') {
        board[row][col] = 'X';
        int score = minimax(board, depth + 1, true);
        board[row][col] = ' ';

        bestScore = min(score, bestScore);
      }
    }
    return bestScore;
  }
}

bool draw() {

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (game_board[i][j] == ' ') {

        return false;
      }
    }
  }

  return true;
}



bool win() {
  //checking the horizontal
  for (int i = 0; i < 3; i++) {

    if (game_board[i][0] == game_board[i][1] && game_board[i][1] == game_board[i][2] && game_board[i][0] != ' ') {
      return true;
      winner = game_board[i][0];
    }
  }
  //checking the vertical
  for (int i = 0; i < 3; i++) {

    if (game_board[0][i] == game_board[1][i] && game_board[1][i] == game_board[2][i] && game_board[0][i] != ' ') {
      return true;
      winner = game_board[0][i];
    }
  }

  //checking the diagonal


  if (game_board[0][0] == game_board[1][1] && game_board[1][1] == game_board[2][2] && game_board[0][0] != ' ') {

    return true;
    winner = game_board[0][0];
  }

  if (game_board[2][0] == game_board[1][1] && game_board[1][1] == game_board[0][2] && game_board[0][0] != ' ') {
    return true;
    winner = game_board[2][0];
  }
  return false;
}




void handleGameResult(char result) {

  if (result == 'X') {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        digitalWrite(red_led[i][j], LOW);
        if (j == i || j == (3 - i - 1) || j == (3 + i - 1)) {
          digitalWrite(blue_led[i][j], HIGH);
        }
      }
    }
  }


  if (result == 'O') {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        digitalWrite(blue_led[i][j], LOW);
        if (j == 0 || j == (3 - 1) || i == (3 - 1) || i == 0) {
          digitalWrite(red_led[i][j], HIGH);
        }
      }
    }
  }
  if (result == 'D') {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        //draw
        if (j <= i) {
          digitalWrite(blue_led[i][j], HIGH);
        }
        if (j > i) {
          digitalWrite(red_led[i][j], HIGH);
        }
      }
    }
  }
}