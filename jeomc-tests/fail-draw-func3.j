/*
Fail: Trying to supply invalid arguments (float when the last arg needs to be string hexcode)
*/

int main()
{
  float x;
  float y;
  int f;

  x = 0.5;
  y = 0.5;
  f = 1;

  jeomcInit();

  drawTriangle(x, y, x, y, x, y, x);

  jeomcRunAndSave();

  return 0;
}
