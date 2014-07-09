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
