/* test hex code translation when we pass in "bad" strings */

int main()
{

  jeomcInit();

  /* should only read the first 6 chars, not including # */
  drawCircle(0.0, -0.5, 0.4,"#FFFFFFFFFFFFFFFF");

  /* should be 0,0,0 white */
  drawCircle(0.0, -0.5, 0.4,"");

  /* should be 0,0,0 white */
  drawCircle(0.0, -0.5, 0.4,"            ");

  /* should only read the first 6 chars */
  drawCircle(0.0, -0.5, 0.4,"FFFFFF        HELLO WORLD");

  /* should be empty white */
  drawCircle(0.0, -0.5, 0.4,"HELLO WORLD");

  /* should be read same as #FFFF */
  drawCircle(0.0, -0.5, 0.4,"#FFFFHELLO WORLD");

  return 0;
}
