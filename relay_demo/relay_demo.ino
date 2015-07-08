
int row;
int col;

void setup()
{
    for (int thisPin = 2; thisPin < 8; thisPin++)  {
      pinMode(thisPin, OUTPUT);      
    }
    for (int thisPin = 2; thisPin < 8; thisPin++)  {
      digitalWrite(thisPin, HIGH);   
    }
    row = 1;
    col = 1;
}

void loop()
{
  
  ElementOn(row, col);
  delay(1000);
  ElementOff(row, col);
  Serial.print("row ");
  Serial.print(row);
  Serial.print(" col ");
  Serial.println(col);
  col = col + 1;
  if (col > 2) {
    col = 1;
    row = row + 1;
    if (row > 1) {
      row = 1;
    }
  }
}

void ElementOn(int row, int col) {
    for (int thisPin = 2; thisPin < 8; thisPin++)  {
      digitalWrite(thisPin, HIGH);
    }
    digitalWrite(1+row, LOW);
    digitalWrite(5+col, LOW);
}
void ElementOff(int row, int col) {
    digitalWrite(1+row, HIGH);
    digitalWrite(5+col, HIGH);
}
