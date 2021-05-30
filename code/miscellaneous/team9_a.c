void main()
{
  move_forward();

}


void move_forward()
{
  while (digital(2) & digital(3)) {
    motor(0, 50);
    motor(1, 50);


  }

  motor(0, -50);
  motor(1, -50);

}


