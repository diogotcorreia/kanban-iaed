/* Diogo Correia - ist199211 */
#include <stdio.h>
#include <string.h>

#include "proj1.h"

/**
 * Adds a new task to the Kanban global store.
 * Returns the ID that represents the new task.
 * If the store has reached the maximum number of tasks, returns -1.
 * If there is already a task with the same description, returns -2.
 * If the duration is negative, returns -3.
 */
int add_task(kanban *global_store, int duration, char description[]) {
	task new_task = {0};
	if (global_store->tasks_count == MAX_TASKS) {
		return -1;
	}
	if (is_duplicate_description(global_store, description)) {
		return -2;
	}
	if (duration <= 0) {
		return -3;
	}

	/* save task properties */
	new_task.id = global_store->tasks_count + 1;
	new_task.duration = duration;
	new_task.user_id = -1;
	strcpy(new_task.description, description);

	global_store->tasks[global_store->tasks_count] = new_task;

	/* save the task, sorted by description (and time),
	  on pointer arrays for easy access by the 'l' and 'd' commands */
	insert_task_sorted(global_store,
	                   &global_store->tasks[global_store->tasks_count]);
	insert_task_sorted_time(global_store,
	                        &global_store->tasks[global_store->tasks_count], 0);

	++global_store->tasks_count;

	return new_task.id;
}

/**
 * Returns a pointer to the task with the given ID.
 * If the task does not exist, returns 0.
 */
task *get_task(kanban *global_store, int id) {
	if (id <= 0 || id > global_store->tasks_count) {
		return 0; /* task id outside array bounds */
	}
	return &global_store->tasks[id - 1];
}

/**
 * Comparison function used in binary search.
 * Compares the description between two tasks.
 */
int task_description_cmp(const task *a, const task_cmp *b) {
	return strcmp(a->description, b->description);
}

/**
 * Finds the insertion point in the sorted task pointer array, then adds
 * the new task and shifts the other elements of the array.
 */
void insert_task_sorted(kanban *global_store, task *task) {
	int m, i;
	task_cmp task_cmp;

	/* add description to the task comparison struct */
	task_cmp.description = task->description;

	m = binary_search(&task_cmp, global_store->tasks_sorted_desc,
	                  global_store->tasks_count, task_description_cmp);

	if (m < 0) {
		/* this is going to always happen,
		  but leave it here in case behaviour changes */
		m = -m - 1;
	}

	for (i = global_store->tasks_count; i >= m; --i)
		global_store->tasks_sorted_desc[i] = global_store->tasks_sorted_desc[i - 1];

	global_store->tasks_sorted_desc[m] = task;
}

/**
 * Comparison function used in binary search.
 * Compares the start time between two tasks.
 * If they are the same, compares the descriptions instead.
 */
int task_time_cmp(const task *a, const task_cmp *b) {
	int cmp;
	cmp = a->start_time - *b->time;
	if (cmp == 0) {
		/* if time is the same, compare descriptions instead */
		cmp = strcmp(a->description, b->description);
	}
	return cmp;
}

/**
 * Finds the insertion point in the sorted task pointer array, then adds
 * the new task and shifts the other elements of the array.
 * If the task already exists, it also makes sure to remove it from the previous
 * position.
 */
void insert_task_sorted_time(kanban *global_store, task *task, int new_time) {
	task_cmp task_cmp;
	int m, i, moved;

	task_cmp.description = task->description;
	task_cmp.time = &new_time;

	m = binary_search(&task_cmp, global_store->tasks_sorted_time,
	                  global_store->tasks_count, task_time_cmp);

	if (m >= 0) {
		return; /* task did not change position */
	}
	m = -m - 1;

	/* move elements backwards to fill space left by moved task */
	for (i = 0, moved = 0; i < global_store->tasks_count - 1; ++i) {
		if (moved || task == global_store->tasks_sorted_time[i]) {
			moved = 1;
			global_store->tasks_sorted_time[i] =
					global_store->tasks_sorted_time[i + 1];
		}
	}

	/* move elements after insertion index forward to open space for moved task */
	for (i = global_store->tasks_count; i >= m; --i)
		global_store->tasks_sorted_time[i] = global_store->tasks_sorted_time[i - 1];

	global_store->tasks_sorted_time[m - moved] = task;
}

/**
 * Performs a binary search by the task description.
 * Returns 1 if there is already a task with this description.
 * Returns 0 otherwise.
 */
int is_duplicate_description(kanban *global_store, char description[]) {
	task_cmp task;
	int result;

	task.description = description;
	result = binary_search(&task, global_store->tasks_sorted_desc,
	                       global_store->tasks_count, task_description_cmp);

	/* if binary search returns a value >= 0, then the item exists */
	return result >= 0;
}

/**
 * Prints all tasks, sorted alphabetically by description.
 */
void print_all_tasks(kanban *global_store) {
	int i;
	task *task;
	char *activity;
	for (i = 0; i < global_store->tasks_count; ++i) {
		task = global_store->tasks_sorted_desc[i];
		activity = get_activity(global_store, task->activity);
		printf(TASK_TO_STRING, task->id, activity, task->duration,
		       task->description);
	}
}

/**
 * Prints a single task by ID.
 * If the task does not exist, prints an error.
 */
void print_task_by_id(kanban *global_store, int id) {
	task *task = get_task(global_store, id);
	if (task) /* if task exists */
	{
		char *activity = get_activity(global_store, task->activity);
		printf(TASK_TO_STRING, task->id, activity, task->duration,
		       task->description);
		return;
	}
	printf(TASK_ERR_NO_SUCH_TASK, id);
}