
char game_board[3][3];
int current_player;  //'O'
//char player2 = 'X';
int blue_led[3][3] = { { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 } };
int red_led[3][3] = { { 9, 10, 11 }, { 12, 13, 14 }, { 15, 16, 17 } };
int buttons[3][3] = { { 25, 27, 29 }, { 31, 33, 35 }, { 37, 39, 41 } };
int indicator_blue_led = 18;
int indicator_red_led = 19;
int game_mode_button = 20;
int single_player_led = 21;
int multi_player_led = 23;
int reset_button = 43;


void setup() {
  // put your setup code here, to run once:

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

  int randomSeed(analogRead(A0));
}

void loop() {
  // put your main code here, to run repeatedly:
  current_player = random(2);

  for (int i = 0; i < 3; i++) {

    for (int j = 0; j < 3; j++) {

      if (digitalRead(buttons[i][j] == LOW & game_board[i][j] == ' ')) {
        set_move(i, j);

        if  (win(i, j)) {
        }else

        if (draw(i, j)) {
        }else {toogle();}
      }
    }

  }
}
    void set_move(int row, int column) {

      char move = (current_player == 1) ? 'X' : 'O';

      game_board[row][column] = move;
      (move == 'X') ? digitalWrite(blue_led[row][column], HIGH) : digitalWrite(red_led[row][column], HIGH);


    }


    bool win(int row, int column) {}
    bool draw(int row, int column) {}


   void toogle(){

      (current_player==1)? 0:1 ;
    }
