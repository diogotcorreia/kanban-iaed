/* Diogo Correia - ist199211 */
#include <string.h>
#include <stdio.h>

#include "proj1.h"

/**
 * Adiciona uma tarefa ao sistema.
 * Retorna o id que representa a nova tarefa.
 * Caso o sistema tenha atingido o número máximo de tarefas, retorna -1.
 * Caso já exista uma tarefa com a mesma descrição, retorna uma tarefa -2.
 * Caso a duração dada seja negativa, retorna -3.
 */
int add_task(kanban *global_store, int duration, char description[])
{
	task new_task;
	if (global_store->tasks_count == MAX_TASKS)
	{
		return -1;
	}
	if (is_duplicate_description(global_store, description))
	{
		return -2;
	}
	if (duration <= 0)
	{
		return -3;
	}
	new_task.id = global_store->tasks_count + 1;
	new_task.duration = duration;
	new_task.activity = 0;
	new_task.start_time = 0;
	new_task.user_id = -1;
	strcpy(new_task.description, description);

	global_store->tasks[global_store->tasks_count] = new_task;

	insert_task_sorted(global_store, &global_store->tasks[global_store->tasks_count]);
	insert_task_sorted_time(global_store, &global_store->tasks[global_store->tasks_count], 0);

	++global_store->tasks_count;

	return new_task.id;
}

task *get_task(kanban *global_store, int id)
{
	if (id <= 0 || id > global_store->tasks_count)
	{
		return 0;
	}
	return &global_store->tasks[id - 1];
}

int task_description_cmp(const task *a, const task_cmp *b)
{
	return strcmp(a->description, b->description);
}

void insert_task_sorted(kanban *global_store, task *task)
{
	int m, i;
	task_cmp task_cmp;

	task_cmp.description = task->description;

	m = binary_search(&task_cmp, global_store->tasks_sorted_desc, global_store->tasks_count, task_description_cmp);

	if (m < 0)
	{
		m = -m - 1;
	}

	for (i = global_store->tasks_count; i >= m; --i)
		global_store->tasks_sorted_desc[i] = global_store->tasks_sorted_desc[i - 1];

	global_store->tasks_sorted_desc[m] = task;
}

int task_time_cmp(const task *a, const task_cmp *b)
{
	int cmp;
	cmp = a->start_time - *b->time;
	if (cmp == 0)
	{
		cmp = strcmp(a->description, b->description);
	}
	return cmp;
}

void insert_task_sorted_time(kanban *global_store, task *task, int new_time)
{
	task_cmp task_cmp;
	int m, i, moved;

	task_cmp.description = task->description;
	task_cmp.time = &new_time;

	m = binary_search(&task_cmp, global_store->tasks_sorted_time, global_store->tasks_count, task_time_cmp);

	if (m < 0)
	{
		m = -m - 1;
	}

	for (i = 0, moved = 0; i < global_store->tasks_count - 1; ++i)
	{
		if (moved)
		{
			global_store->tasks_sorted_time[i] = global_store->tasks_sorted_time[i + 1];
		}
		else if (task == global_store->tasks_sorted_time[i])
		{
			moved = 1;
			global_store->tasks_sorted_time[i] = global_store->tasks_sorted_time[i + 1];
		}
	}

	for (i = global_store->tasks_count; i >= m; --i)
		global_store->tasks_sorted_time[i] = global_store->tasks_sorted_time[i - 1];

	global_store->tasks_sorted_time[m - moved] = task;
}

void update_task(kanban *global_store, int id, task task)
{
	global_store->tasks[id - 1] = task;
}

/* Retorna 1 se já existir uma tarefa com esta descrição. Retorna 0 em caso contrário. */
int is_duplicate_description(kanban *global_store, char description[])
{
	task_cmp task;
	int result;

	task.description = description;
	result = binary_search(&task, global_store->tasks_sorted_desc, global_store->tasks_count, task_description_cmp);

	return result >= 0;
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
	if (id > 0 && id - 1 < global_store->tasks_count)
	{
		printf(TASK_TO_STRING, task.id, get_activity(global_store, task.activity), task.duration, task.description);
	}
	else
	{
		printf(TASK_ERR_NO_SUCH_TASK, id);
	}
}