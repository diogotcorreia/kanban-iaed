#ifndef TASKS_H
#define TASKS_H

#define MAX_TASKS 10000
#define MAX_DESCRIPTION_LENGTH 50

#define TASK_ADD_SUCCESS "task %d\n"
#define TASK_TO_STRING "%d %s #%d %s"
#define TASK_ERR_TOO_MANY_TASKS "too many tasks\n"
#define TASK_ERR_DUPLICATE_DESCRIPTION "duplicate description\n"
#define TASK_ERR_NO_SUCH_TASK "%d: no such task\n"

#define TASK_MOVE_ERR_NO_SUCH_TASK "no such task\n"
#define TASK_MOVE_ERR_ALREADY_STARTED "task already started\n"
#define TASK_MOVE_ERR_NO_SUCH_USER "no such user\n"
#define TASK_MOVE_ERR_NO_SUCH_ACTIVITY "no such activity\n"

typedef struct
{
	short id; /* irá ser sempre o índice na array + 1, ou zero caso não exista */
	int duration;
	char description[MAX_DESCRIPTION_LENGTH];
	int activity;
} task;

task add_task(int duration, char description[]);

task get_task(int id);

void update_task(int id, task task);

int is_duplicate_description(char description[], int tasks_size);

void print_all_tasks();

void print_task_by_id(int id);

#endif