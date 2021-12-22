int main(){
    float i;
    float j;

    jeomcInit();

    i = -0.8;
    j = 0.0;

    while (i < 0.8) {
      drawRectangle(i, j+0.05 , i+0.05,j+0.05,   i+0.05,j  ,  i,j   ,"#516091");
      i = i + 0.1;
    }

    i = 0.0;
    j = -0.8;

    while (j < 0.8) {
      drawRectangle(i, j+0.05 , i+0.05,j+0.05,   i+0.05,j  ,  i,j   ,"#74bec1");
      j = j + 0.1;
    }

    jeomcRunAndSave();

    return 0;
}
