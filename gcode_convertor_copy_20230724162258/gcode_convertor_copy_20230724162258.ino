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

inline bool isDigit(char ch) {
  return ('0' <= ch) && (ch <= '9');
}

inline bool isDigitOrDot(char ch) {
  return isDigit(ch) || ch == '.';
}

char incomingByte = 0;  // for incoming serial data

void setup() {
  Serial.begin(9600);

  Serial.println("\n\n\t~~~ WELCOME 😀 ~~~");
  Serial.println("Mecha X Maker pen plotter workshop!");
  Serial.println("Instructions: github.com/UoA-Maker-Club/plotter-arm");
  Serial.println("gcode generator: https://markroland.github.io/sand-table-pattern-maker");
}



void loop() {
  if (Serial.available()) {
    String input = Serial.readString();
    int i = 0;

    while (i < input.length()) {
      readInstruction(i, input);
    }
  }
}

float readFloat(int &i, String s) {
  String buffer = "";

  while (i < s.length()) {
    if (isDigitOrDot(s[i])) {
      buffer += String(s[i]);
    } else if ((s[i] == ' ' || s[i] == '\n' || s[i] == '>') && buffer.length() != 0) {
      if (s[i] == '>') i--; 
      return buffer.toFloat();
    } else {
      Serial.println("Breaking, buffer is '" + buffer + "'");
      return NAN;
    }

    i++;
  }

  return NAN;
}

void readInstruction(int &i, String s) {
  if (s.length() - i < 2) {
    i++;
    return;
  };

  if (s[0] != 'G' || !isDigit(s[1])) {
    i++;
    return;
  };

  int command = s[1] - '0';

  Serial.println("Command is " + String(command));
  Serial.flush();

  switch (command) {
    case 0:
    case 1:
      float x;
      float y;

      for (i += 3; i < s.length(); i++) {
        switch (s[i]) {
          case ' ':
            break;

          case 'X':
            i++;
            x = readFloat(i, s);
            if (isnan(x)) {
              Serial.println("X is NaN");
              return;
            };
            break;

          case 'Y':
            i++;
            y = readFloat(i, s);
            if (isnan(y)) {
              Serial.println("Y is NaN");
              return;
            }
            break;

          case '>':
            Serial.println("Command " + String(command) + " X: " + String(x) + ", Y: " + String(y));
            Serial.flush();
            i++;
            return;

          default:
            Serial.println("Breaking on char " + String(s[i]));

            return;
        }
      }

      return;
    default:
      return;
  }
}

