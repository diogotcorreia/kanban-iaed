/* Diogo Correia - ist199211 */
#include <string.h>
#include <stdio.h>

#include "proj1.h"

/**
 * Adiciona uma tarefa ao sistema.
 * Retorna a estrutura que representa a nova tarefa.
 * Caso o sistema tenha atingido o número máximo de tarefas, retorna
 * uma tarefa cujo ID é -1.
 * Caso já exista uma tarefa com a mesma descrição, retorna uma tarefa
 * cujo ID é -2.
 */
task add_task(kanban *global_store, int duration, char description[])
{
	task new_task;
	if (global_store->tasks_count == MAX_TASKS)
	{
		task too_many_tasks = {-1, 0, "", 0, 0, -1};
		return too_many_tasks;
	}
	if (is_duplicate_description(global_store, description))
	{
		task duplicate_task = {-2, 0, "", 0, 0, -1};
		return duplicate_task;
	}
	new_task.id = global_store->tasks_count + 1;
	new_task.duration = duration;
	new_task.activity = 0;
	new_task.start_time = 0;
	new_task.user_id = -1;
	strcpy(new_task.description, description);

	global_store->tasks[global_store->tasks_count++] = new_task;

	insert_task_sorted(global_store, &global_store->tasks[global_store->tasks_count - 1]);

	return new_task;
}

task get_task(kanban *global_store, int id)
{
	task task;
	if (id <= 0)
	{
		task.id = 0;
		return task;
	}
	return global_store->tasks[id - 1];
}

void insert_task_sorted(kanban *global_store, task *task)
{
	int l = 0;
	int h = global_store->tasks_count - 1;
	int i, m = 0, cmp = -1;

	while (l < h && cmp != 0)
	{
		m = (l + h) / 2;
		cmp = strcmp(task->description, global_store->tasks[m].description);
		if (cmp < 0)
		{
			h = m;
		}
		else if (cmp > 0)
		{
			l = m + 1;
		}
	}

	if (cmp > 0)
	{
		++m;
	}

	for (i = global_store->tasks_count - 1; i >= m; --i)
		global_store->tasks_sorted_desc[i] = global_store->tasks_sorted_desc[i - 1];

	global_store->tasks_sorted_desc[m] = task;
}

/* TODO remove this */
int get_tasks_by_activity(kanban *global_store, int id, task tasks[])
{
	int i;
	int j = 0;
	for (i = 0; i < MAX_TASKS && global_store->tasks[i].id > 0; ++i)
	{
		if (global_store->tasks[i].activity == id)
		{
			tasks[j++] = global_store->tasks[i];
		}
	}
	return j;
}

void update_task(kanban *global_store, int id, task task)
{
	global_store->tasks[id - 1] = task;
}

/* Retorna 1 se já existir uma tarefa com esta descrição. Retorna 0 em caso contrário. */
int is_duplicate_description(kanban *global_store, char description[])
{
	int i;
	for (i = 0; i < global_store->tasks_count; ++i)
	{
		if (strcmp(global_store->tasks[i].description, description) == 0)
		{
			return 1;
		}
	}
	return 0;
}

/* Imprime todas as tarefas, ordenadas pela sua descrição, para o stdout */
void print_all_tasks(kanban *global_store)
{
	int i;
	task *task;
	for (i = 0; i < global_store->tasks_count; ++i)
	{
		task = global_store->tasks_sorted_desc[i];
		printf(TASK_TO_STRING, task->id, get_activity(global_store, task->activity), task->duration, task->description);
	}
}

void print_task_by_id(kanban *global_store, int id)
{
	task task = global_store->tasks[id - 1];
	if (task.id > 0)
	{
		printf(TASK_TO_STRING, task.id, get_activity(global_store, task.activity), task.duration, task.description);
	}
	else
	{
		printf(TASK_ERR_NO_SUCH_TASK, id);
	}
}