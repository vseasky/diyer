#ifndef _START_TASK_H
#define _START_TASK_H



void create_start_task(void);
void start_task(void *pvParameters);
void led0_task(void *pvParameters);
void led1_task(void *pvParameters);
#endif