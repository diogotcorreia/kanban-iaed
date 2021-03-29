/* Diogo Correia - ist199211 */
#include <stdio.h>
#include <ctype.h>

#include "activities.h"
#include "tasks.h"

/* Recebe um comando do stdin, e redireciona para a função desejada */
int handle_command();

void handle_add_task_command();

void handle_list_tasks_command();

void handle_time_forward_command();

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