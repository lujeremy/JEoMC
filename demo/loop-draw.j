int main(){
    float i;
    float j;
    int c;

    jeomcInit();

    i = -0.8;
    j = 0.0;
    c = 0;

    for (c = 0; c < 17; c = c + 1) {

      /* use continue to skip all odd positions */
      if (c == 1 || c == 3 || c == 5 || c == 7 || c == 9 || c == 11 || c == 13 || c == 15) {
        c = c + 1;
        i = i + 0.1;
        continue;
      }

      drawRectangle(i, j+0.05 , i+0.05,j+0.05,   i+0.05,j  ,  i,j   ,"#516091");
      i = i + 0.1;
    }

    i = 0.0;
    j = -0.8;
    c = 0;

    for (c = 0; c < 15; c = c + 1) {

      /* after drawing 12 rectangles, break */
      if (c == 12) {
        break;
      }

      drawRectangle(i, j+0.05 , i+0.05,j+0.05,   i+0.05,j  ,  i,j   ,"#74bec1");
      j = j + 0.1;
    }

    jeomcRunAndSave();

    return 0;
}
