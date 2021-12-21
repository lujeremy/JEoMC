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

  setActiveColor(0.686, 0.933, 0.933, 1.0);
  drawRectangle(-0.5, -0.5, 0.25, 0.25);

  setActiveColor(0.867, 0.627, 0.867, 1.0);
  drawCircle(-0.5, 0.5, 0.25);

  jeomcRunAndSave();
  return 0;
}
