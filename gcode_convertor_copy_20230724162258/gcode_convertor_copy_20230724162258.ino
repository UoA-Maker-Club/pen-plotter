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
  Serial.println("gcode generator: https://markrol  and.github.io/sand-table-pattern-maker");
}



void loop() {
  if (Serial.available()) {
    String input = Serial.readString();

    Serial.println("'" + input + "'");
    Serial.println("Length is " + String(input.length()));
    Serial.println("input[0] is " + String(input[0]));
    Serial.println("input[1] is " + String(input[1]));
    Serial.println("isDigit(input[1]) is " + String(isDigit(input[1])));

    Serial.flush();

    if (input.length() < 2) return;
    if (input[0] != 'G' || !isDigit(input[1])) return;

    int command = input[1] - '0';

    Serial.println("Command is " + String(command));
    Serial.flush();

    switch (command) {
      case 0:
      case 1:
        float x;
        float y;

        for (int i = 2; i < input.length(); i++) {
          switch (input[i]) {
            case ' ':
              break;

            case 'X':
              i++;
              x = readFloat(i, input);
              if (isnan(x)) { Serial.println("X is NaN"); return; };
              break;

            case 'Y':
              i++;
              Serial.println(input[i]);
              y = readFloat(i, input);
              if (isnan(y)) { Serial.println("Y is NaN"); return; }
              break;
            
            default:
              break;
          }
        }

        Serial.println("Command " + String(command) + " X: " + String(x) + ", Y: " + String(y));
        Serial.flush();
        return;
      default:
        return;
    }
  }
}

float readFloat(int &i, String s) {
  String buffer = "";

  while (i < s.length()) {
    Serial.println(s[i]);

    if (isDigitOrDot(s[i])) {
      buffer += String(s[i]);
    } else if ((s[i] == ' ' || s[i] == '\n') && buffer.length() != 0) {
      return buffer.toFloat();
    } else {
      Serial.println("Breaking, buffer is '" + buffer + "'");
      return NAN;
    }

    i++;
  }
  
  return NAN;
}

/* 
Test GCode
G0 X236.00 Y190.00
*/
