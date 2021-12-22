/* test hex code translation with various ways of passing in strings
   You can lead with # or omit it. We take care of both cases
 */

int main()
{

  jeomcInit();

  /* working examples */
  drawCircle(0.0, -0.5, 0.4,"#FFFFFF");
  drawCircle(0.0, -0.5, 0.4,"FFFFFF");
  drawCircle(0.0, -0.5, 0.4,"#FFFFFF        ");

  /*
  Note that leading whitespace and whitespace in between strings will NOT work
  drawCircle(0.0, -0.5, 0.4,"       #FFFFFF");
  drawCircle(0.0, -0.5, 0.4,"#FFFF       FF");
  */

  return 0;
}
