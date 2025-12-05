#ifndef TIMER_H_
#define TIMER_H_

#define TICKSPS 1 // The number of timer ticks per second.

typedef struct {
    unsigned char month; // Jan = 1, Dec = 12
    unsigned char day; // First day of month = 1
    unsigned char hour; // 12AM = 0, 24 hour clock
    unsigned char minute; // xx:00 = 0
    unsigned char second; // xx:xx:00 = 0
} timedate_t;

void setupTimer(void);
unsigned long getTime(void);
float getSecondsF(void);
unsigned long getSeconds(void);
timedate_t getTimeDate(void);
void setTimer(unsigned long ticks);
void setSeconds(unsigned long seconds);
void setTime(timedate_t time);

timedate_t secondsToTimeDate(unsigned long inSeconds);
unsigned long timeDateToSeconds(timedate_t td);


#endif /* TIMER_H_ */
