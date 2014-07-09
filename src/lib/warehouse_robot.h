/**
  * avg_light
  *
  * @return int the average between the minimum light level (black, line to follow) and the maximum light level (white, empty space).
  *
  * The light sensor must be connected to port 3.
  * The function has its own "POWER" (50).
  *
  */
int avg_light(void);
/**
  * turn_time_90
  *
  * @parameter int the average of light to find the line.
  *
  * @return long time in milliseconds to turn 90 degree.
  *
  * The light sensor must be connected to port 3.
  * The function has its own "POWER" (50).
  *
  */
long turn_time_90(int i_avg_light);



int avg_light(void) {
	long l_end_time;
	int i_avg_light;
	int i_min = 999;
	int i_max = -1;
	int i_current_light = 0;

	// Turning left
	l_end_time = CurrentTick() + 600; // 600 ms ~=> 90 degrees
	OnFwd(OUT_B, 50); // Motor B => right wheel
	OnRev(OUT_A, 50); // Motor A => left wheel
	while(CurrentTick() < l_end_time) {
		i_current_light = Sensor(IN_3); // 3 => light sensor

		if (i_current_light > i_max) {
			i_max = i_current_light;
		} else if (i_current_light < i_min) {
			i_min = i_current_light;
		}
	}

	// Turning right
	l_end_time = CurrentTick() + 1200; // 1200 ms ~=> 180 degrees
	OnFwd(OUT_A, 50); // Motor A => left wheel
	OnRev(OUT_B, 50); // Motor B => right wheel
	while (CurrentTick() < l_end_time) {
		i_current_light = Sensor(IN_3); // 3 => light sensor

		if (i_current_light > i_max) {
			i_max = i_current_light;
		} else if (i_current_light < i_min) {
			i_min = i_current_light;
		}
	}

	// Back to start position (turning left)
	l_end_time = CurrentTick() + 600; // 600 ms ~=> 90 degrees
	OnFwd(OUT_B, 50); // Motor B => right wheel
	OnRev(OUT_A, 50); // Motor A => left wheel
	while(CurrentTick() < l_end_time) {
		i_current_light = Sensor(IN_3); // 3 => light sensor

		if (i_current_light > i_max) {
			i_max = i_current_light;
		} else if (i_current_light < i_min) {
			i_min = i_current_light;
		}
	}
	Off(OUT_AB); // Stop motors

	// Calculate average
	i_avg_light = (i_min + i_max) / 2;

	return i_avg_light;
}

long turn_time_90(int i_avg_light) {
	int i_current_light = 0;
	int i_current_time = 0;
	int i_turn_time_180 = 0;

	i_current_time = CurrentTick(); // save current time

	// turn left out of the line
	OnFwd(OUT_B, 50);
	OnRev(OUT_A, 50);
	Wait(300);

	// turn left until the line is found (~180 degrees).
	do {
		OnFwd(OUT_B, 50);
		OnRev(OUT_A, 50);
		i_current_light = Sensor(IN_3);
	} while(i_current_light >= i_avg_light);

	// save the amount of past time.
	i_turn_time_180 = CurrentTick() - i_current_time;

	// Back to start position.
	// turn right out of the line
	OnFwd(OUT_A, 50);
	OnRev(OUT_B, 50);
	Wait(i_turn_time_180);

	Off(OUT_AB); // stop motors

	return (i_turn_time_180 / 2);
}
