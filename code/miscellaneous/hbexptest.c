/* ------------------------------
   6.270 Handy Board Test Program
   by Mike Allen (mallen@mit.edu)
   7 January 2002
   ------------------------------ */

void main()
{
  printf("6.270 HB test   press start\n");
  wait_start();

  motor_test();
  servo_test();
  sensor_test();

  printf("done.\n");

}

void motor_test()
{
  int a,b=1;
  int i;

  printf("motor test      press start\n");
  wait_start();

  reset_system_time();

  while (!stop_button()) {
    a = (int)seconds()%2;

    if (a == 0 && b == 1) {
      printf("motor lights    should flash\n");
      for (i = 0; i<6; i++) fd(i);
    }
    else if (a == 1 && b==0) {
      printf("press stop      to continue\n");
      for (i = 0; i<6; i++) bk(i);
    }
   b = a;
  }
  while (stop_button());

  ao();
}

void servo_test()
{
  int a,b=1;
  int i;

  printf("servo test      press start\n");
  wait_start();

  reset_system_time();
  enable_servos();

  while (!stop_button()) {
    a = (int)seconds()%2;

    if (a == 0 && b == 1) {
      printf("servos should   move\n");
      for (i = 0; i<6; i++) servo(i,0);
    }
    else if (a == 1 && b == 0) {
      printf("press stop      to continue\n");
      for (i = 0; i<6; i++) servo(i,4000);
    }
   b = a;
  }
  while (stop_button());

  disable_servos();
}

void sensor_test()
{
  int port;
 
  printf("sensor test     press start\n");
  wait_start();

  while (!press_stop()) {
    port = knob()/8;
    if (port>=2)
      printf("use frob knob   port %d: %d\n",port,analog(port));
    else
      printf("use frob knob   port %d: n/a\n",port);
    msleep(100L);
  }
  while (stop_button());
}

void wait_start()
{
  while (!press_start());
}

int press_start()
{
  if (!start_button()) return 0;
  while (start_button());
  return 1;
}

int press_stop()
{
  if (!stop_button()) return 0;
  while (stop_button());
  return 1;
}
