int main()
{

  jeomcInit();

  drawCircle(0.0, -0.5, 0.4,"#FFFFFF");
  drawCircle(0.0, 0.1, 0.30,"#FFFFFF");
  drawCircle(0.0, 0.6, 0.20,"#FFFFFF");

  drawRectangle(-0.25,0.32,0.15,0.5,"B49EC9");

  drawCircle(-0.08,0.65,0.03, "#000000");
  drawCircle(0.08,0.65,0.03, "#000000");

  drawTriangle(0.0,0.6,0.15,0.58,0.0,0.56, "E0B949");

  drawCircle(0.0, 0.1, 0.03, "#000000");
  drawCircle(0.0, -0.1, 0.03, "#000000");
  drawCircle(0.0, -0.3, 0.03, "#000000");

<<<<<<< Updated upstream
=======
  drawLine(-0.75,0.55,-0.18,0.3, "9A5415");
  drawLine(0.75,0.55,0.18,0.3, "9A5415");

>>>>>>> Stashed changes
  jeomcRunAndSave();
  return 0;
}
