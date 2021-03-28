#include <string.h>

#include "activities.h"

static activity activities[MAX_ACTIVITIES];

void setup_activities()
{
  char default_activities[][MAX_ACTIVITY_NAME_LENGTH] = {"TO DO", "IN PROGRESS", "DONE"};
  int i;
  activity current_activity;
  current_activity.task_size = 0;
  for (i = 0; i < 3; ++i)
  {
    strcpy(current_activity.name, default_activities[i]);
    activities[i] = current_activity;
  }
}

activity get_activity(int index)
{
  return activities[index];
}