int main()
{

  jeomcInit();

  drawCircle(0.0, -0.5, 0.4,1.0,1.0,1.0);
  drawCircle(0.0, 0.1, 0.30,1.0,1.0,1.0);
  drawCircle(0.0, 0.6, 0.20,1.0,1.0,1.0);

  drawRectangle(-0.25,0.32,0.15,0.5,1.0,0.0,0.0);

  drawCircle(-0.08,0.65,0.03,0.0,0.0,0.0);
  drawCircle(0.08,0.65,0.03,0.0,0.0,0.0);

  drawTriangle(0.0,0.6,0.15,0.58,0.0,0.56,1.0,0.8,0.0);

  
  drawCircle(0.0, 0.1, 0.03,0.0,0.0,0.0);
  drawCircle(0.0, -0.1, 0.03,0.0,0.0,0.0);
  drawCircle(0.0, -0.3, 0.03,0.0,0.0,0.0);

  jeomcRunAndSave();
  return 0;
}
