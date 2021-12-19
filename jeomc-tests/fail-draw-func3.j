/*
Fail: Trying to supply invalid arguments (int when it's supposed to be float)
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

  drawTriangle(x, y, f);

  jeomcRunAndSave();

  return 0;
}
