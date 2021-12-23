int main(){
    
  jeomcInit();

  drawRectangle(-1.0,-1.0,-1.0,1.0,1.0,1.0,1.0,-1.0,"#A1CDCF");
  drawRectangle(-1.0,-1.0,-1.0,0.0,1.0,0.0,1.0,-1.0,"#FCFEFD");
  
  drawTriangle(0.0,-1.0,0.5,0.7,2.0,-1.0,"#C2CECF");
  drawCircle(0.5,0.3,0.8,"#C4C3B1");
  drawCircle(0.5,0.2,0.6,"#282923");
  drawCircle(0.5,0.05,0.5,"#FBE2CA");
  drawTriangle(0.30,-0.2,0.5,-0.30,0.7,-0.2,"#FDEDDE");

  drawTriangle(0.2,0.12,0.3,0.17,0.4,0.12,"#210F00");
  drawTriangle(0.6,0.12,0.7,0.17,0.8,0.12,"#210F00");

  drawCircle(-0.1,-0.5,0.25,"#FBE2CA");

  drawRectangle(0.0,-0.4,-0.1,-0.7,1.0,-1.0,1.0,-0.6,"#CED5D5");

  drawCircle(-0.4,-0.4,0.3,"#DB8200");
  drawRectangle(-0.75,-0.50,-0.75,-0.35,-0.05,-0.35,-0.05,-0.5,"#F3C519");

  drawLine(0.4,0.6,0.45,0.7,"FCFF53");
  drawLine(0.45,0.7,0.5,0.65,"FCFF53");
  drawLine(0.5,0.65,0.55,0.7,"FCFF53");
  drawLine(0.55,0.7,0.6,0.6,"FCFF53");



  jeomcRunAndSave();

}