#ifndef TIMER_H_
#define TIMER_H_

#define TICKSPS 128 // The number of timer ticks per second.

void setupTimer(void);
unsigned long getTime(void);
float getSecondsF(void);
unsigned int getSeconds(void);


#endif /* TIMER_H_ */
