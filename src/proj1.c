/* Diogo Correia - ist199211 */
#include <stdio.h>

#include "activities.h"
#include "tasks.h"

/* Recebe um comando do stdin, e redireciona para a função desejada */
int handle_command();

void handle_add_task_command();

void handle_list_tasks_command();

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
	case 'q':
	default:
		/* Sair do programa */
		return 0;
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