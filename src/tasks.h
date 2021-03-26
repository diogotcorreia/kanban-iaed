#include "constants.h"

typedef struct
{
	short id;
	int duration;
	char description[MAX_DESCRIPTION_LENGTH];
} task;

task add_task(int duration, char description[]);

int is_duplicate_description(char description[], int tasks_size);

void print_all_tasks();
