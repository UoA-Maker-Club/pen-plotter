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
  Serial.begin(19200);

  Serial.println("\n\n\t~~~ WELCOME 😀 ~~~");
  Serial.println("Mecha X Maker pen plotter workshop!");
  Serial.println("Instructions: github.com/UoA-Maker-Club/plotter-arm");
  Serial.println("gcode generator: https://markroland.github.io/sand-table-pattern-maker");

  Serial.setTimeout(20000);
}



void loop() {
  
  if (Serial.available()) {
    String input = Serial.readString();
    Serial.print("Read string length is ");
    Serial.println(input.length());

    int i = 0;
    int nCommands = 0;

    while (i < input.length()) {
      readInstruction(i, input);
      nCommands++;
    }
    Serial.print("Processed ");
    Serial.print(nCommands);
    Serial.println(" commands");

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
      Serial.print("Breaking, buffer is '");
      Serial.print(buffer);
      Serial.println("'");
      return NAN;
    }

    i++;
  }
  Serial.println("Reading float reached the end of buffer");
  return NAN;
}

void readInstruction(int &i, String s) {
  Serial.print("Reading command, i is ");
  Serial.println(i);

  if (s.length() - i < 2) {
    Serial.println("Text is too short to be a command");
    i++;
    delay(100);
    return;
  };

  if (s[i] != 'G' || !isDigit(s[i + 1])) {
    i++;
    Serial.print((int) s[i]);
    Serial.print((int) s[i + 1]);
    Serial.println(" is not a command");
    delay(100);
    return;
  };

  int command = s[1] - '0';

  Serial.print("Command is ");
  Serial.println(command);

  delay(100);

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
            Serial.print("Command ");
            Serial.print(command);
            Serial.print(" X: ");
            Serial.print(x);
            Serial.print(", Y: ");
            Serial.println(y);
            i++;
            return;

          default:
            Serial.print("Breaking on char ");
            Serial.println(s[i]);

            return;
        }
      }

      Serial.println("readInstruction recahed end of input without completing");

      return;
    default:
      return;
  }
}
