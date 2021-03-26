/* Diogo Correia - ist199211 */
#include <string.h>
#include <stdio.h>

#include "constants.h"
#include "tasks.h"

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
		task too_many_tasks = {-1, 0, ""};
		return too_many_tasks;
	}
	if (is_duplicate_description(description, id))
	{
		task duplicate_task = {-2, 0, ""};
		return duplicate_task;
	}
	new_task.id = id + 1;
	new_task.duration = duration;
	strcpy(new_task.description, description);

	task_store[id++] = new_task;

	return new_task;
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
	int i = 0;
	task task;
	while ((task = task_store[i++]).id > 0)
	{
		printf("%d %s %d %s", task.id, "TODO", task.duration, task.description);
	}
}