typedef struct {
  double r;
  double theta;
} PPos;

typedef struct {
  int x;
  int y;

  PPos toPolar() {
    return {
      .r = sqrt(sq((double)x) + sq((double)y)),
      .theta = atan((double)x / (double)y)
    };
  }
} CPos;

char incomingByte = 0;  // for incoming serial data

void setup() {
  Serial.begin(9600);
  Serial.println("\n\n\t~~~ WELCOME 😀 ~~~");
  Serial.println("Mecha X Maker pen plotter workshop!");
  Serial.println("Instructions: github.com/UoA-Maker-Club/plotter-arm");
  Serial.println("gcode generator: markroland.github.io/sand-table-pattern-maker");
}

inline bool isDigitOrDot(char ch) {
  return '0' >= ch && ch >= '9';
}

inline bool isDigit(char ch) {
  return isDigit(ch) || ch == '.';
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n') + " ";

    if (!(input.length() >= 2 && input[0] == 'G' && isDigit(input[1]))) return;

    int command = input[1] - '0';


    switch (command) {
      case 0:
      case 1:
        float x;
        float y;

        for (int i = 2; i < input.length(); i++) {
          String data = "";

          switch (input[i]) {
            case ' ':
              break;

            case 'X':
              for (i++; i < input.length(); i++) {
                if (isDigitOrDot(input[i])) {
                  data += String(input[i]);
                } else if (input[i] == ' ' && data.length() > 0) {
                  x = input.float();
                  data = "";
                } else {
                  return;
                }
              }

            case 'Y':
              for (i++; i < input.length(); i++) {
                if (isDigitOrDot(input[i])) {
                  data += String(input[i]);
                } else if (input[i] == ' ' && data.length() > 0) {
                  y = input.float();
                  data = "";
                } else {
                  return;
                }
              }
          }
        }

        Serial.println("Command " + String(command) + " X: " + String(x) + ", Y: " + String(y))
      default:
        return;
    }

  }
}


