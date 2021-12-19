/*
Fail: Trying to supply too few arguments when func expects 3
*/

int main()
{
  float x;
  float y;
  float f;

  x = 0.5;
  y = 0.5;
  f = 0.5;

  jeomcInit();

  drawTriangle(x, y);

  jeomcRunAndSave();

  return 0;
}
