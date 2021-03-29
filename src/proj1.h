#ifndef MAIN_H
#define MAIN_H

#define TIME_ERR_INVALID "invalid time\n"

/* tasks */

#define MAX_TASKS 10000
#define MAX_DESCRIPTION_LENGTH 50

#define TASK_ADD_SUCCESS "task %d\n"
#define TASK_TO_STRING "%d %s #%d %s\n"
#define TASK_ERR_TOO_MANY_TASKS "too many tasks\n"
#define TASK_ERR_DUPLICATE_DESCRIPTION "duplicate description\n"
#define TASK_ERR_NO_SUCH_TASK "%d: no such task\n"

#define TASK_MOVE_ERR_NO_SUCH_TASK "no such task\n"
#define TASK_MOVE_ERR_ALREADY_STARTED "task already started\n"
#define TASK_MOVE_ERR_NO_SUCH_USER "no such user\n"
#define TASK_MOVE_ERR_NO_SUCH_ACTIVITY "no such activity\n"
#define TASK_MOVE_DURATION "duration=%d slack=%d\n"

#define TASK_BY_ACTIVITY_ERR_NO_SUCH_ACTIVITY "no such activity\n"
#define TASK_BY_ACTIVITY_TO_STR "%d %d %s\n"

/* activities */

#define MAX_ACTIVITY_NAME_LENGTH 50
#define MAX_ACTIVITIES 10

#define ACTIVITY_TODO "TO DO"
#define ACTIVITY_IN_PROGRESS "IN PROGRESS"
#define ACTIVITY_DONE "DONE"

#define ACTIVITY_TO_STRING "%s\n"
#define ACTIVITY_ERR_TOO_MANY "too many activities\n"
#define ACTIVITY_ERR_DUPLICATE "duplicate activity\n"
#define ACTIVITY_ERR_INVALID_DESC "invalid description\n"

/* users */

#define MAX_USER_NAME_LENGTH 20
#define MAX_USERS 50

#define USER_TO_STRING "%s\n"
#define USER_ERR_TOO_MANY "too many users\n"
#define USER_ERR_DUPLICATE "user already exists\n"

typedef struct
{
	short id; /* irá ser sempre o índice na array + 1, ou zero caso não exista */
	int duration;
	char description[MAX_DESCRIPTION_LENGTH];
	int activity;
	int start_time;
	int user_id;
} task;

typedef struct
{
	task tasks[MAX_TASKS];
	int tasks_count;
	char activities[MAX_ACTIVITIES][MAX_ACTIVITY_NAME_LENGTH];
	int activities_count;
	char users[MAX_USERS][MAX_USER_NAME_LENGTH];
	int users_count;
	int time;
	task *tasks_sorted_desc[MAX_TASKS];
	task *tasks_sorted_time[MAX_TASKS];
} kanban;

/* Recebe um comando do stdin, e redireciona para a função desejada */
int handle_command(kanban *global_store);

void handle_add_task_command(kanban *global_store);

void handle_list_tasks_command(kanban *global_store);

void handle_time_forward_command(kanban *global_store);

void handle_users_command(kanban *global_store);

void handle_move_command(kanban *global_store);

void handle_list_by_activities_command(kanban *global_store);

void handle_activities_command(kanban *global_store);

/* tasks.c */

task add_task(kanban *global_store, int duration, char description[]);

task get_task(kanban *global_store, int id);

void insert_task_sorted(kanban *global_store, task *task);

int get_tasks_by_activity(kanban *global_store, int id, task tasks[]);

void update_task(kanban *global_store, int id, task task);

int is_duplicate_description(kanban *global_store, char description[]);

void print_all_tasks(kanban *global_store);

void print_task_by_id(kanban *global_store, int id);

/* activities.h */

void setup_activities(kanban *global_store);

char *get_activity(kanban *global_store, int index);

int add_activity(kanban *global_store, char name[MAX_ACTIVITY_NAME_LENGTH]);

int get_activity_id(kanban *global_store, char name[]);

int is_invalid_activity_name(char name[]);

void list_all_activities(kanban *global_store);

/* users.h */

char *get_user(kanban *global_store, int index);

int add_user(kanban *global_store, char name[]);

int get_user_id(kanban *global_store, char name[]);

void list_all_users(kanban *global_store);

#endif