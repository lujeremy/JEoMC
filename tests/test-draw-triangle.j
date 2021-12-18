int main()
{
  float x;
  float y;
  float f;

  x = 0.5;
  y = 0.5;
  f = 0.5;

  jeomcInit();
  drawTriangle(x, y, f);
  drawTriangle(-0.5, -0.5, 0.25);
  drawTriangle(-0.5, 0.5, 0.25);
  jeomcRunAndSave();
  return 0;
}
