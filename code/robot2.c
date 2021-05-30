/*
6.270
Team 9
David Tsai
Min Zhang
Andy Lin
*/



/*
sensors:  
sensor port 2:  front left, white on outside, 
sensor port 3:  front middle, yellow on outside
sensor port 4:  front right, blue on outside
sensor port 6:  ball color, purple on outside
sensor port 18:  back left, brown on outside
sensor port 20:  back right, blue on outside

sensor port 22: start light sensor

sensor port 7-10: beacon direction LEDs
                  all colors on Signal (4th row of holes)
                  purple on 7
sensor port 11: beacon 

motors:  
motor port 1:  orange on bottom, left wheel
motor port 2:  green on bottom, right wheel

servos:
black on outside

servo 1: front wheel
         300 is straight position,
         2150 is 90-degree turn position, 
         0 for left small turn,
         600 for right small turn
servo 2: ramp 
         4000 is up position,
         1300 is down-position
servo 3: back gate
         0 is down position, 
         2000 is up position
*/


#define left_side 1
#define right_side 2
#define startlight 22

void main() {
  int starting_side = 0;

/*
  setup();
  start_machine(startlight);
*/

  if (knob() > 8) {
    setup();
  } else {
    
    /* Initialize Servo positions */
    enable_servos();
    
    sleep(0.5);
    servo(2, 1500);
    sleep(0.5);
    servo(3, 0);
    sleep(0.5);
    
    /* Determine Starting Position */
    starting_side = determine_side();

    if (starting_side == left_side){
      printf("LEFT side path\n");
      left_side_path();
    }
    else {
      printf("RIGHT side path\n");
      right_side_path();
    }

    disable_servos();
  }
}

void setup() {
  int blackest_18;

  ao();

  /* CALIBRATE BLACK */
  printf("START to cal BLACK, STOP when done\n");
  wait_start();

  reset_system_time();

  while (!stop_button()) {
	color_black_2 = analog(2);
	color_black_3 = analog(3);
	color_black_4 = analog(4);
	color_black_18 = analog(18);
	color_black_20 = analog(20);

	if (color_black_18 > blackest_18)
	  blackest_18 = color_black_18;
  }
 
  color_black_18 = blackest_18;

  while (stop_button());
  beep();


  /* CALIBRATE WHITE*/
  printf("START to cal WHITE, STOP when done\n");
  wait_start();
  
  reset_system_time();

  while (!stop_button()) {
	color_white_2 = analog(2);
	color_white_3 = analog(3);
	color_white_4 = analog(4);
	color_white_18 = analog(18);
	color_white_20 = analog(20);
  }

  threshold_2 = (color_black_2 + color_white_2) / 2;
  threshold_3 = (color_black_3 + color_white_3) / 2;
  threshold_4 = (color_black_4 + color_white_4) / 2;
  threshold_18 = (color_black_18 + color_white_18) / 2;
  threshold_20 = (color_black_20 + color_white_20) / 2;

  while (stop_button());
  beep();


  /* CALIBRATE OUR BALL COLOR */
  printf("START to cal OUR BALL, STOP when done\n");
  wait_start();

  reset_system_time();

  while (!stop_button()) {
	color_balls_ours = analog(6);
  }
  while (stop_button());


  /* CALIBRATE THEIR BALL COLOR */
  printf("START to cal THEIR BALL, STOP when done\n");
  wait_start();

  reset_system_time();

  while (!stop_button()) {
	color_balls_ours = analog(6);
  }
  while (stop_button());


  printf("calibration DONE\n");
  beep();
  beep();
  beep();

  ao();	
}


int determine_side() {
  if (analog(2) < threshold_2)
    return left_side; 
  else
    return right_side;
}


void left_side_path() {
  float start_time;
  float start_time_2;

  left_90();
  sleep(0.5);
  printf("forward next\n");

  servo(1, 300);
  sleep(0.5);

  while (!((analog(2) > threshold_2) &&
	   (analog(4) > threshold_4))) {
    move_forward();
  }

  sleep(0.5);


  start_time = seconds();
  while (seconds() < (start_time + 0.5)) {
    move_forward();
  }

  ao();

  sleep(2.0);
 
  servo(1, 300);
  sleep(0.5);
 

  /* backup for [x] seconds */
  start_time_2 = seconds();
  while (seconds() < (start_time_2 + 0.8)) { 
    bk(1);
    bk(2);
  }
  
  
  sleep(0.5);
  ao();


  servo(2, 4000);
  sleep(5.0);
  servo(2, 1500);

  sleep(0.5);
  ao();
}


void right_side_path() {
  float start_time;
  float start_time_2;

  right_90();
  sleep(0.5);
  printf("forward next\n");

  servo(1, 300);
  sleep(0.5);

  while (!((analog(2) > threshold_2) &&
	   (analog(4) > threshold_4))) {
    move_forward();
  }

  sleep(0.5);


  start_time = seconds();
  while (seconds() < (start_time + 0.8)) {
    move_forward();
  }

  ao();

  sleep(2.0);
 
  servo(1, 300);
  sleep(0.5);
 

  /* backup for [x] seconds */
  start_time_2 = seconds();
  while (seconds() < (start_time_2 + 1.0)) { 
    bk(1);
    bk(2);
  }
  
  
  sleep(0.5);
  ao();


  servo(2, 4000);
  sleep(5.0);
  servo(2, 1500);

  sleep(0.5);
  ao();
}



void ball_pickup() {
  if ((analog(6) < (color_balls_ours + 50)) &&
      (analog(6) > (color_balls_ours - 50))) {
    servo(2, 4000);
    servo(3, 0);
  } else if((analog(6) < (color_balls_theirs + 50)) &&
	     (analog(6) > (color_balls_theirs - 50))) {
    servo(2, 4000);
    servo(3, 2000);
  }
}

void move_forward() {
  fd(1);
  fd(2);

  if (analog(2) > threshold_2) {
    /* turn left */
    servo(1, 0);
  } else if (analog(4) > threshold_4) {
    servo(1, 600);
  } else {
    servo(1, 300);
  }
}


void move_forward_left() {
  fd(1);
  fd(2);

	
  if ((analog(20) < threshold_20) &&
      (analog(4) > threshold_4) &&
      (analog(3) < threshold_3)) {
    /* turn left */
    servo(1, 0);
  } else if ((analog(20) < threshold_20) &&
	     (analog(4) < threshold_4) &&
	     (analog(3) < threshold_3)) {
    /* turn right */
    servo(1, 600);
  } else if ((analog(20) > threshold_20) &&
	     (analog(4) < threshold_4)) {
    /* turn left/straight */
    servo(1, 300);
  }
}



void move_forward_right() {
  fd(1);
  fd(2);

  if ((analog(18) < threshold_18) &&
      (analog(2) > threshold_2) &&
      (analog(3) < threshold_3)) {
    /* turn right */
    servo(1, 600);
  } else if ((analog(18) < threshold_18) &&
	     (analog(2) < threshold_2) &&
	     (analog(3) < threshold_3)) {
    /* turn left */
    servo(1, 0);
  } else if ((analog(18) > threshold_18) &&
	     (analog(2) < threshold_2)) {
    /* turn right/straight */
    servo(1, 300);
  }
    
}




void move_backward() {
  bk(1);
  bk(2);
  

  if (analog(2) > threshold_2) {
    /* turn left */
    servo(1, 0);
  } else if (analog(4) > threshold_4) {
    servo(1, 600);
  } else {
    servo(1, 300);
  }
 
}


void left_90_cross() {
  float init_time;

  servo(1, 2150);
  bk(1);
  fd(2);
  
  init_time = seconds();
  
  while (!((analog(2) < threshold_2) && 
	   (analog(3) > threshold_3) && 
	   (analog(4) < threshold_4) &&
	   (seconds() > (.8 + init_time)))) {
    sleep(.01); 
  }
  
  ao();
  sleep(0.5);
  
  if ((analog(2) < threshold_2) && 
      (analog(3) < threshold_3)) {
    right_90();
  }
  
  ao();
}


void right_90_cross() {
  float init_time;

  servo(1, 2150);
  fd(1);
  bk(2);
  
  init_time = seconds();
  
  
  while (!((analog(2) < threshold_2) && 
	   (analog(3) > threshold_3) && 
	   (analog(4) < threshold_4) &&
	   (seconds() > (.8 + init_time)))) {
    sleep(.01); 
  }
  
  ao();
  sleep(0.5);
  
  if ((analog(3) < threshold_3) && 
      (analog(4) < threshold_4)) {
    left_90();
  }
  
  ao();
}


void left_90() {
  servo(1, 2150);
  bk(1);
  fd(2);
  
  while (!((analog(2) < threshold_2) && 
	   (analog(3) > threshold_3) && 
	   (analog(4) < threshold_4))) {
    sleep(.01);
    
  }

  ao();
  sleep(0.5);

  if ((analog(2) < threshold_2) && 
      (analog(3) < threshold_3)) {
    right_90();
  }
  
  ao();
}

void right_90() {
  servo(1, 2150);
  fd(1);
  bk(2);
  
  while (!((analog(2) < threshold_2) && 
	   (analog(3) > threshold_3) && 
	   (analog(4) < threshold_4))) {
    sleep(.01);
  }

  ao();
  sleep(0.5);

  if ((analog(3) < threshold_3) && 
      (analog(4) < threshold_4)) {
    left_90();
  }
  
  ao();
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



