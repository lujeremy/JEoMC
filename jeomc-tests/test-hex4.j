/* test hex code translation when we pass in "bad" strings */

int main()
{

  jeomcInit();

  /* Once we encounter an invalid hex character, we stop reading even if valid characters come after */
  /* these two should produce identical RGBs */
  drawCircle(0.0, -0.5, 0.4,"#Fz0000");
  drawCircle(0.0, -0.5, 0.4,"#F");

  return 0;
}
