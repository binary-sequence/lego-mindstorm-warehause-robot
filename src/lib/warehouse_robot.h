#define POWER 25

/**
  * avg_light
  *
  * @return int The average between the minimum light level (black, line to follow) and the maximum light level (white, empty space).
  *
  * The light sensor must be connected to port 3.
  *
  */
int avg_light(void);
/**
  * find_left_path
  *
  */
void find_left_path(long l_turn_time_90);
/**
  * find_middle_path
  *
  */
void find_middle_path(long l_turn_time_90);
/**
  * find_right_path
  *
  */
void find_right_path(long l_turn_time_90);
/**
  * follow_line
  *
  * @parameter int The average of light to find the line.
  * @parameter long Time in milliseconds to turn 90 degree.
  *
  * The light sensor must be connected to port 3.
  *
  */
void follow_line(int i_avg_light, long l_turn_time_90);
/**
  * get_object_color
  *
  */
int get_object_color(void);
/**
  * turn_time_90
  *
  * @parameter int The average of light to find the line.
  *
  * @return long Time in milliseconds to turn 90 degree.
  *
  * The light sensor must be connected to port 3.
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
	OnFwd(OUT_B, POWER); // Motor B => right wheel
	OnRev(OUT_A, POWER); // Motor A => left wheel
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
	OnFwd(OUT_A, POWER); // Motor A => left wheel
	OnRev(OUT_B, POWER); // Motor B => right wheel
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
	OnFwd(OUT_B, POWER); // Motor B => right wheel
	OnRev(OUT_A, POWER); // Motor A => left wheel
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

void find_left_path(long l_turn_time_90) {
	// move ahead
	OnFwd(OUT_AB, POWER);
	Wait(500);

	// Turn left 90 degrees.
	OnFwd(OUT_B, POWER);
	OnRev(OUT_A, POWER);
	Wait(l_turn_time_90);

	Off(OUT_AB); // stop motors
}

void find_middle_path(long l_turn_time_90) {
	// move ahead
	OnFwd(OUT_AB, POWER);
	Wait(500);

	Off(OUT_AB); // stop motors
}

void find_right_path(long l_turn_time_90) {
	// move ahead
	OnFwd(OUT_AB, POWER);
	Wait(500);

	// Turn right 45 degrees.
	OnFwd(OUT_A, POWER);
	OnRev(OUT_B, POWER);
	Wait(l_turn_time_90 / 2);

	Off(OUT_AB); // stop motors
}

void follow_line(int i_avg_light, long l_turn_time_90) {
	int i_current_light = 0;
	bool b_end_of_line = 0;
	long l_current_time = CurrentTick();
	long l_time_passed = 0;

	while(b_end_of_line == false) {
		i_current_light = Sensor(IN_3);

		if(i_current_light <= 50) {
			l_current_time = CurrentTick();

			OnFwd(OUT_AB, POWER); // go ahead

			// somebody/something in front of the robot
			while(SensorUS(IN_2) < 20) {
				Off(OUT_AB); // Stop motors
				TextOut(0, LCD_LINE3, "Warning!!");
			}
			ClearScreen();
		} else {
			// save the time passed since it turns
			l_time_passed = CurrentTick() - l_current_time;

			if(l_time_passed <= l_turn_time_90) {
				// Turn right to look for the line (maximum 90 degrees)
				OnFwd(OUT_A, POWER);
				OnRev(OUT_B, POWER);
			} else {
				if(l_time_passed <= l_turn_time_90 * 3) {
					// Turn left to look for the line (maximum 270 degrees)
					OnFwd(OUT_B, POWER);
					OnRev(OUT_A, POWER);
				} else {
					// Turn right 90 degrees (back to the start position)
					OnFwd(OUT_A, POWER);
					OnRev(OUT_B, POWER);
					Wait(l_turn_time_90);
					Off(OUT_AB); // Stop motors

					b_end_of_line = true;
				}
			}
		}
	}
}

int get_object_color(void) {
	int i_current_color = 0;

	do {
		while(Sensor(IN_4) == 0) {
			i_current_color = Sensor(IN_1);
			TextOut(0, LCD_LINE1, "Reading color: ");
			NumOut(0, LCD_LINE2, i_current_color);
		}

		if (i_current_color != 5 && i_current_color != 6) {
			TextOut(0, LCD_LINE4, "Only accepts");
			TextOut(0, LCD_LINE5, "red and white.");
		}
	} while(i_current_color != 5 && i_current_color != 6);
	ClearScreen();

	return i_current_color;
}

long turn_time_90(int i_avg_light) {
	int i_current_light = 0;
	int i_current_time = 0;
	int i_turn_time_180 = 0;

	i_current_time = CurrentTick(); // save current time

	// turn left out of the line
	OnFwd(OUT_B, POWER);
	OnRev(OUT_A, POWER);
	Wait(300);

	// turn left until the line is found (~180 degrees).
	do {
		OnFwd(OUT_B, POWER);
		OnRev(OUT_A, POWER);
		i_current_light = Sensor(IN_3);
	} while(i_current_light >= i_avg_light);

	// save the amount of past time.
	i_turn_time_180 = CurrentTick() - i_current_time;

	// Back to start position.
	// turn right out of the line
	OnFwd(OUT_A, POWER);
	OnRev(OUT_B, POWER);
	Wait(i_turn_time_180);

	Off(OUT_AB); // stop motors

	return (i_turn_time_180 / 2);
}
