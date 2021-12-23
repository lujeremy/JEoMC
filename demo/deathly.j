int main()
{
    jeomcInit();

    /* draw the cloak */
    drawLine(0.0,0.8,-0.4,0.0, "FFFFFF");
    drawLine(-0.4,0.0,0.4,0.0, "FFFFFF");
    drawLine(0.4,0.0,0.0,0.8, "FFFFFF");

    /* draw the wand */
    drawLine(0.0,0.8,0.0,0.0,"FFFFFF");

    /* draw the snitch */
    drawCircle(0.0,0.25,0.25,"#ECECEC");

    jeomcRunAndSave();
    return 0;
}