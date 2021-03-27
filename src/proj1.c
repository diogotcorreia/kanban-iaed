/* Diogo Correia - ist199211 */
#include <stdio.h>

#include "activities.h"

/* Recebe um comando do stdin, e redireciona para a função desejada */
int handle_command();

void handle_add_task_command();

int main()
{
	setup_activities();
	while (handle_command())
		;
	print_all_tasks(); /* TODO temporarily show all tasks on quit */
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
	fgets(description, MAX_DESCRIPTION_LENGTH, stdin);

	result_task = add_task(duration, description);

	if (result_task.id == -1)
	{
		printf("too many tasks\n");
	}
	else if (result_task.id == -2)
	{
		printf("duplicate description\n");
	}
	else
	{
		printf("task %d\n", result_task.id);
	}
}