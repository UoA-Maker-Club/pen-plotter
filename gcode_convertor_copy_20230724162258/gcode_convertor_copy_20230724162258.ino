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

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readString();
    if ((input.substring(0, 2) == "G1") || (input.substring(0, 2) == "G0")) {
      Serial.println("here");
      int xIndex = input.indexOf("X");
      int yIndex = input.indexOf("Y");
      double x = (input.substring(4, yIndex - 1)).toDouble();
      double y = (input.substring(yIndex + 1)).toDouble();

      Serial.println(x);
      Serial.println(y);

      // Move to line
    }
    Serial.println(input);
  }
}

