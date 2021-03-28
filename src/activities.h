#ifndef ACTIVIES_H
#define ACTIVIES_H

#include "tasks.h"

#define MAX_ACTIVITY_NAME_LENGTH 50
#define MAX_ACTIVITIES 10

typedef struct
{
  char name[MAX_ACTIVITY_NAME_LENGTH];
  task tasks[MAX_TASKS];
  int task_size;
} activity;

void setup_activities();

#endif