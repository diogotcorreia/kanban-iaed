/* Diogo Correia - ist199211 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "activities.h"
#include "tasks.h"
#include "users.h"

/* Recebe um comando do stdin, e redireciona para a função desejada */
int handle_command();

void handle_add_task_command();

void handle_list_tasks_command();

void handle_time_forward_command();

void handle_users_command();

void handle_move_command();

void handle_activities_command();

static int time = 0;

int main()
{
	setup_activities();
	while (handle_command())
		;
	return 0;
}

int handle_command()
{
	char c = getchar();
	switch (c)
	{
	case 't':
		handle_add_task_command();
		return 1;
	case 'l':
		handle_list_tasks_command();
		return 1;
	case 'n':
		handle_time_forward_command();
		return 1;
	case 'u':
		handle_users_command();
		return 1;
	case 'm':
		handle_move_command();
		return 1;
	case 'a':
		handle_activities_command();
		return 1;
	case 'q':
		/* Sair do programa */
		return 0;
	default:
		/* Ignorar comandos desconhecidos */
		return 1;
	}
}

void handle_add_task_command()
{
	int duration;
	char description[MAX_DESCRIPTION_LENGTH];
	task result_task;

	scanf("%d", &duration);
	getchar(); /* consume space before description */
	fgets(description, MAX_DESCRIPTION_LENGTH, stdin);

	result_task = add_task(duration, description);

	if (result_task.id == -1)
	{
		printf(TASK_ERR_TOO_MANY_TASKS);
	}
	else if (result_task.id == -2)
	{
		printf(TASK_ERR_DUPLICATE_DESCRIPTION);
	}
	else
	{
		printf(TASK_ADD_SUCCESS, result_task.id);
	}
}

void handle_list_tasks_command()
{
	int id = 0, all = 1;
	char c;

	while ((c = getchar()) != EOF && c != '\n')
	{
		if (c < '0' || c > '9')
		{
			if (id != 0)
			{
				print_task_by_id(id);
			}
			id = 0;
		}
		else
		{
			id = id * 10 + c - '0';
			all = 0;
		}
	}
	if (id != 0)
	{
		print_task_by_id(id);
	}
	if (all)
	{
		print_all_tasks();
	}
}

void handle_time_forward_command()
{
	int increment;
	scanf("%d", &increment);
	time += increment;
	printf("%d\n", time);
}

void handle_users_command()
{
	char c;
	int i = 0;
	char name[MAX_USER_NAME_LENGTH];
	user new_user;

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
		new_user = add_user(name);
		switch (new_user.status)
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
		list_all_users();
	}
}

void handle_move_command()
{
	task task;
	int task_id, user_id, activity_id;
	char user[MAX_USER_NAME_LENGTH];
	char activity[MAX_ACTIVITY_NAME_LENGTH];

	scanf("%d", &task_id);
	scanf("%s", user);

	getchar(); /* read space after user */
	fgets(activity, MAX_ACTIVITY_NAME_LENGTH, stdin);
	activity[strcspn(activity, "\n")] = 0; /* remover \n no final da string */

	task = get_task(task_id);
	user_id = get_user_id(user);
	activity_id = get_activity_id(activity);

	/* verificar condições e retornar erros */
	if (task.id <= 0)
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

	task.activity = activity_id;
	update_task(task_id, task);
}

void handle_activities_command()
{
	char c;
	int i = 0;
	char name[MAX_ACTIVITY_NAME_LENGTH];
	activity new_activity;

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
		new_activity = add_activity(name);
		switch (new_activity.status)
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
		list_all_activities();
	}
}