/* Diogo Correia - ist199211 */
#include <string.h>
#include <stdio.h>

#include "tasks.h"
#include "activities.h"

static task task_store[MAX_TASKS] = {{0}};

/**
 * Adiciona uma tarefa ao sistema.
 * Retorna a estrutura que representa a nova tarefa.
 * Caso o sistema tenha atingido o número máximo de tarefas, retorna
 * uma tarefa cujo ID é -1.
 * Caso já exista uma tarefa com a mesma descrição, retorna uma tarefa
 * cujo ID é -2.
 */
task add_task(int duration, char description[])
{
	static int id = 0; /* guardar o ID da tarefa anterior */
	task new_task;
	if (id == MAX_TASKS)
	{
		task too_many_tasks = {-1, 0, "", 0, 0, -1};
		return too_many_tasks;
	}
	if (is_duplicate_description(description, id))
	{
		task duplicate_task = {-2, 0, "", 0, 0, -1};
		return duplicate_task;
	}
	new_task.id = id + 1;
	new_task.duration = duration;
	new_task.activity = 0;
	new_task.start_time = 0;
	new_task.user_id = -1;
	strcpy(new_task.description, description);

	task_store[id++] = new_task;

	return new_task;
}

task get_task(int id)
{
	task task;
	if (id <= 0)
	{
		task.id = 0;
		return task;
	}
	return task_store[id - 1];
}

int get_tasks_by_activity(int id, task tasks[])
{
	int i;
	int j = 0;
	for (i = 0; i < MAX_TASKS && task_store[i].id > 0; ++i)
	{
		if (task_store[i].activity == id)
		{
			tasks[j++] = task_store[i];
		}
	}
	return j;
}

void update_task(int id, task task)
{
	task_store[id - 1] = task;
}

/* Retorna 1 se já existir uma tarefa com esta descrição. Retorna 0 em caso contrário. */
int is_duplicate_description(char description[], int tasks_size)
{
	int i;
	for (i = 0; i < tasks_size; ++i)
	{
		if (strcmp(task_store[i].description, description) == 0)
		{
			return 1;
		}
	}
	return 0;
}

/* Imprime todas as tarefas, ordenadas pela sua descrição, para o stdout */
void print_all_tasks()
{
	/* TODO this should print the tasks ordered by description */
	int i = 0;
	task task;
	activity activity;
	while ((task = task_store[i++]).id > 0)
	{
		activity = get_activity(task.activity);
		printf(TASK_TO_STRING, task.id, activity.name, task.duration, task.description);
	}
}

void print_task_by_id(int id)
{
	task task = task_store[id - 1];
	if (task.id > 0)
	{
		activity activity = get_activity(task.activity);
		printf(TASK_TO_STRING, task.id, activity.name, task.duration, task.description);
	}
	else
	{
		printf(TASK_ERR_NO_SUCH_TASK, id);
	}
}