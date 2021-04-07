/* Diogo Correia - ist199211 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "proj1.h"

int main()
{
	kanban global_store = {0};
	setup_activities(&global_store);
	while (handle_command(&global_store))
		;
	return 0;
}

int handle_command(kanban *global_store)
{
	char c = getchar();
	switch (c)
	{
	case 't':
		handle_add_task_command(global_store);
		return 1;
	case 'l':
		handle_list_tasks_command(global_store);
		return 1;
	case 'n':
		handle_time_forward_command(global_store);
		return 1;
	case 'u':
		handle_users_command(global_store);
		return 1;
	case 'm':
		handle_move_command(global_store);
		return 1;
	case 'd':
		handle_list_by_activities_command(global_store);
		return 1;
	case 'a':
		handle_activities_command(global_store);
		return 1;
	case 'q':
		/* Sair do programa */
		return 0;
	default:
		/* Ignorar comandos desconhecidos */
		return 1;
	}
}

void handle_add_task_command(kanban *global_store)
{
	int duration, result_task;
	char description[MAX_DESCRIPTION_LENGTH];

	scanf("%d", &duration);
	getchar(); /* consume space before description */
	fgets(description, MAX_DESCRIPTION_LENGTH, stdin);
	description[strcspn(description, "\n")] = 0; /* remover \n no final da string */

	result_task = add_task(global_store, duration, description);

	if (result_task == -1)
	{
		printf(TASK_ERR_TOO_MANY_TASKS);
	}
	else if (result_task == -2)
	{
		printf(TASK_ERR_DUPLICATE_DESCRIPTION);
	}
	else if (result_task == -3)
	{
		printf(TASK_ERR_INVALID_DURATION);
	}
	else
	{
		printf(TASK_ADD_SUCCESS, result_task);
	}
}

void handle_list_tasks_command(kanban *global_store)
{
	int id = 0, all = 1, neg = 0, read = 0;
	char c;

	while ((c = getchar()) != EOF && c != '\n')
	{
		if (c == '-')
		{
			neg = 1;
		}
		else if (c < '0' || c > '9')
		{
			if (read != 0)
			{
				print_task_by_id(global_store, neg ? -id : id);
			}
			id = 0;
			neg = 0;
			read = 0;
		}
		else
		{
			id = id * 10 + c - '0';
			all = 0;
			read = 1;
		}
	}
	if (read != 0)
	{
		print_task_by_id(global_store, neg ? -id : id);
	}
	if (all)
	{
		print_all_tasks(global_store);
	}
}

void handle_time_forward_command(kanban *global_store)
{
	int increment;
	scanf("%d", &increment);

	if (increment < 0)
	{
		printf(TIME_ERR_INVALID);
		return;
	}

	global_store->time += increment;
	printf("%d\n", global_store->time);
}

void handle_users_command(kanban *global_store)
{
	char c;
	int i = 0, result;
	char name[MAX_USER_NAME_LENGTH];

	while ((c = getchar()) != EOF && c != '\n' && i < MAX_USER_NAME_LENGTH - 1)
	{
		if (!isspace(c))
		{
			name[i++] = c;
		}
	}
	name[i] = '\0';

	if (i)
	{
		result = add_user(global_store, name);
		switch (result)
		{
		case -1:
			printf(USER_ERR_TOO_MANY);
			break;
		case -2:
			printf(USER_ERR_DUPLICATE);
			break;
		}
	}
	else
	{
		list_all_users(global_store);
	}
}

void handle_move_command(kanban *global_store)
{
	task *task;
	int task_id, user_id, activity_id;
	char user[MAX_USER_NAME_LENGTH];
	char activity[MAX_ACTIVITY_NAME_LENGTH];

	scanf("%d", &task_id);
	scanf("%s", user);

	getchar(); /* read space after user */
	fgets(activity, MAX_ACTIVITY_NAME_LENGTH, stdin);
	activity[strcspn(activity, "\n")] = 0; /* remover \n no final da string */

	task = get_task(global_store, task_id);
	user_id = get_user_id(global_store, user);
	activity_id = get_activity_id(global_store, activity);

	/* verificar condições e retornar erros */
	if (task == 0 || task->id == 0)
	{
		printf(TASK_MOVE_ERR_NO_SUCH_TASK);
		return;
	}
	else if (strcmp(activity, ACTIVITY_TODO) == 0)
	{
		printf(TASK_MOVE_ERR_ALREADY_STARTED);
		return;
	}
	else if (user_id < 0)
	{
		printf(TASK_MOVE_ERR_NO_SUCH_USER);
		return;
	}
	else if (activity_id < 0)
	{
		printf(TASK_MOVE_ERR_NO_SUCH_ACTIVITY);
		return;
	}

	if (task->activity == ACTIVITY_TODO_ID)
	{
		insert_task_sorted_time(global_store, task, global_store->time);
		task->start_time = global_store->time;
	}

	if (activity_id == ACTIVITY_DONE_ID && task->activity != ACTIVITY_DONE_ID)
	{
		printf(TASK_MOVE_DURATION, global_store->time - task->start_time, global_store->time - task->start_time - task->duration);
	}

	task->activity = activity_id;
	task->user_id = user_id;
}

void handle_list_by_activities_command(kanban *global_store)
{
	char activity[MAX_ACTIVITY_NAME_LENGTH];
	int activity_id, i;
	task *task;

	getchar(); /* read space after command */
	fgets(activity, MAX_ACTIVITY_NAME_LENGTH, stdin);
	activity[strcspn(activity, "\n")] = 0; /* remover \n no final da string */

	activity_id = get_activity_id(global_store, activity);

	if (activity_id < 0)
	{
		printf(TASK_BY_ACTIVITY_ERR_NO_SUCH_ACTIVITY);
		return;
	}

	for (i = 0; i < global_store->tasks_count; ++i)
	{
		task = global_store->tasks_sorted_time[i];
		if (task->activity == activity_id)
		{
			printf(TASK_BY_ACTIVITY_TO_STR, task->id, task->start_time, task->description);
		}
	}
}

void handle_activities_command(kanban *global_store)
{
	char c;
	int i = 0, result;
	char name[MAX_ACTIVITY_NAME_LENGTH];

	while ((c = getchar()) != EOF && c != '\n' && i < MAX_ACTIVITY_NAME_LENGTH - 1)
	{
		if (i || !isspace(c))
		{
			name[i++] = c;
		}
	}
	name[i] = '\0';

	if (i)
	{
		result = add_activity(global_store, name);
		switch (result)
		{
		case -1:
			printf(ACTIVITY_ERR_TOO_MANY);
			break;
		case -2:
			printf(ACTIVITY_ERR_DUPLICATE);
			break;
		case -3:
			printf(ACTIVITY_ERR_INVALID_DESC);
			break;
		}
	}
	else
	{
		list_all_activities(global_store);
	}
}