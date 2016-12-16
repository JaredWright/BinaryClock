
__attribute__((packed)) struct ds3231_time {
	uint8_t year;	// 0 - 99
	uint8_t month;	// 1 - 12
	uint8_t date;	// 1 - 31
	uint8_t day;	// Weekday (1-7)
	uint8_t hour;	// 1 - 12 + AM/PM (12hr mode) || 0 - 23 (24hr mode)
	uint8_t minute;	// 0 - 59
	uint8_t second;	// 0 - 59
};

int ds3231_set_mode(uint8_t mode);
void ds3231_set_time(struct ds3231_time * t);
int ds3231_get_time(struct ds3231_time * t);

