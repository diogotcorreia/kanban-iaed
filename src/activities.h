#ifndef ACTIVIES_H
#define ACTIVIES_H

#include "tasks.h"

#define MAX_ACTIVITY_NAME_LENGTH 50
#define MAX_ACTIVITIES 10

#define ACTIVITY_TODO "TO DO"
#define ACTIVITY_IN_PROGRESS "IN PROGRESS"
#define ACTIVITY_DONE "DONE"

#define ACTIVITY_TO_STRING "%s\n"
#define ACTIVITY_ERR_TOO_MANY "too many activities\n"
#define ACTIVITY_ERR_DUPLICATE "duplicate activity\n"
#define ACTIVITY_ERR_INVALID_DESC "invalid description\n"

typedef struct
{
	char name[MAX_ACTIVITY_NAME_LENGTH];
	int status;
} activity;

void setup_activities();

activity get_activity(int index);

activity add_activity(char name[MAX_ACTIVITY_NAME_LENGTH]);

int is_duplicate_activity_name(char name[]);

int is_invalid_activity_name(char name[]);

void list_all_activities();

#endif