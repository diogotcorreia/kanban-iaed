/* Diogo Correia - ist199211 */
#include "proj1.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main() {
	kanban global_store = {0};
	setup_activities(&global_store); /* add TO DO, IN PROGRESS and DONE */

	/* execute program until the user sends the 'q' command */
	while (handle_command(&global_store)) {
	}

	return 0;
}

/**
 * Handles command input.
 * Returns 1 if the program should continue after running the command.
 * Otherwise, returns 0.
 */
int handle_command(kanban *global_store) {
	char c = getchar();
	switch (c) {
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
		case 'g':
			handle_groups_command(global_store);
			return 1;
		case 'q':
			/* stop the program */
			return 0;
		default:
			/* ignore unknown commands */
			return 1;
	}
}

/**
 * Handles the 't' command.
 * Adds a new task with the specified duration and description.
 */
void handle_add_task_command(kanban *global_store) {
	int duration, result_task;
	char description[MAX_DESCRIPTION_LENGTH];

	scanf("%d", &duration);
	populate_string(description, MAX_DESCRIPTION_LENGTH);

	result_task = add_task(global_store, duration, description);

	if (result_task == -1) {
		printf(TASK_ERR_TOO_MANY_TASKS);
	} else if (result_task == -2) {
		printf(TASK_ERR_DUPLICATE_DESCRIPTION);
	} else if (result_task == -3) {
		printf(TASK_ERR_INVALID_DURATION);
	} else {
		/* result_task is the ID of the task */
		printf(TASK_ADD_SUCCESS, result_task);
	}
}

/**
 * Handles the 'l' command.
 * If no arguments prints all tasks in alphabetical order.
 * Otherwise, prints the tasks with the specified IDs.
 */
void handle_list_tasks_command(kanban *global_store) {
	int id = 0, all = 1, neg = 0, read = 0;
	char c;

	/* read numbers one at a time, printing the output after reading a number */
	while ((c = getchar()) != EOF && c != '\n') {
		if (c == '-') {
			neg = 1; /* handle negative numbers correctly */
		} else if (c < '0' || c > '9') {
			/* print the task with the id, if we had a number. reset values */
			if (read != 0) {
				print_task_by_id(global_store, neg ? -id : id);
			}
			id = 0, neg = 0, read = 0;
		} else {
			/* add read digit to our number */
			id = id * 10 + c - '0';
			all = 0, read = 1;
		}
	}
	if (read != 0) {
		/* print leftover number from while loop */
		print_task_by_id(global_store, neg ? -id : id);
	}
	if (all) {
		/* if there was no number inserted, print all tasks */
		print_all_tasks(global_store);
	}
}

/**
 * Handles the 'n' command.
 * Forwards time by the amount specified.
 */
void handle_time_forward_command(kanban *global_store) {
	int increment;
	scanf("%d", &increment);

	if (increment < 0) {
		printf(TIME_ERR_INVALID);
		return;
	}

	global_store->time += increment;
	printf(TIME_MSG, global_store->time);
}

/**
 * Handles the 'u' command.
 * Used for adding a new user if it has arguments,
 * or prints all users if no arguments are specified.
 */
void handle_users_command(kanban *global_store) {
	int i;
	char name[MAX_USER_NAME_LENGTH];

	i = populate_string(name, MAX_USER_NAME_LENGTH);

	if (i) /* command has an argument */
	{
		int result = add_user(global_store, name);
		switch (result) {
			case -1:
				printf(USER_ERR_TOO_MANY);
				break;
			case -2:
				printf(USER_ERR_DUPLICATE);
				break;
		}
	} else {
		list_all_users(global_store);
	}
}

/**
 * Handles the 'm' command.
 * Moves a task with the specified ID to another activity.
 * If the task leaves TO DO, the leave time is saved to the task.
 * If the task enters DONE, a message is printed with the task duration and
 * slack.
 */
void handle_move_command(kanban *global_store) {
	task *task;
	int task_id, user_id, activity_id;
	char user[MAX_USER_NAME_LENGTH];
	char activity[MAX_ACTIVITY_NAME_LENGTH];

	scanf("%d", &task_id);
	scanf("%s", user);
	populate_string(activity, MAX_ACTIVITY_NAME_LENGTH);

	/* try to get the given task, user and activity by their ID */
	task = get_task(global_store, task_id);
	user_id = get_user_id(global_store, user);
	activity_id = get_activity_id(global_store, activity);

	if (!move_command_has_errors(task, user_id, activity_id)) {
		/* if all the arguments are correct, handle the move */
		if (task->activity == ACTIVITY_TODO_ID) {
			insert_task_sorted_time(global_store, task, global_store->time);
			task->start_time = global_store->time;
		}

		if (activity_id == ACTIVITY_DONE_ID) {
			int duration = global_store->time - task->start_time;
			printf(TASK_MOVE_DURATION, duration, duration - task->duration);
		}

		task->activity = activity_id;
		task->user_id = user_id;
	}
}

/**
 * Checks if the input for the 'm' command has errors.
 * If so, returns 1, otherwise returns 0.
 */
int move_command_has_errors(task *task, int user_id, int activity_id) {
	if (task == 0 || task->id == 0) {
		printf(TASK_MOVE_ERR_NO_SUCH_TASK); /* check if task exists */
		return 1;
	} else if (task->activity == activity_id) {
		return 1; /* ignore moving to the same activity */
	} else if (activity_id == ACTIVITY_TODO_ID) {
		printf(TASK_MOVE_ERR_ALREADY_STARTED); /* can't move task back to TO DO */
		return 1;
	} else if (user_id < 0) {
		printf(TASK_MOVE_ERR_NO_SUCH_USER); /* user does not exist */
		return 1;
	} else if (activity_id < 0) {
		printf(TASK_MOVE_ERR_NO_SUCH_ACTIVITY); /* activity does not exist */
		return 1;
	}
	return 0;
}

/**
 * Handles the 'd' command.
 * Prints the tasks in the specified activity sorted by start time.
 * If two or more tasks have the same start time, they're storted
 * alphabetically.
 */
void handle_list_by_activities_command(kanban *global_store) {
	char activity[MAX_ACTIVITY_NAME_LENGTH];
	int activity_id, i;
	task *task;

	populate_string(activity, MAX_ACTIVITY_NAME_LENGTH);

	activity_id = get_activity_id(global_store, activity);

	if (activity_id < 0) {
		printf(TASK_BY_ACTIVITY_ERR_NO_SUCH_ACTIVITY);
		return;
	}

	for (i = 0; i < global_store->tasks_count; ++i) {
		/* the tasks_sorted_time array is already sorted by time and description, so
		 * we just need to print, the tasks that belong to the activity we want */
		task = global_store->tasks_sorted_time[i];
		if (task->activity == activity_id) {
			printf(TASK_BY_ACTIVITY_TO_STR, task->id, task->start_time,
			       task->description);
		}
	}
}

/**
 * Handles the 'a' command.
 * Adds a new activity to the Kanban.
 * If no arguments are specified, prints the name of all activities.
 */
void handle_activities_command(kanban *global_store) {
	int i;
	char name[MAX_ACTIVITY_NAME_LENGTH];

	i = populate_string(name, MAX_ACTIVITY_NAME_LENGTH);

	if (i) /* command has an argument */
	{
		int result = add_activity(global_store, name);
		switch (result) {
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
	} else {
		list_all_activities(global_store);
	}
}

/**
 * Handles the 'g' command.
 * Adds a new group to the Kanban.
 */
void handle_groups_command(kanban *global_store) {
	char group_name[MAX_USER_NAME_LENGTH];
	char input_str[(MAX_USER_NAME_LENGTH + 1) * MAX_GROUP_USERS];
	char group_members[MAX_USER_NAME_LENGTH][MAX_GROUP_USERS];
	char *member;
	int member_count = 0, error_no_such_user = 0, error_repeated = 0;

	scanf("%s", group_name);
	fgets(input_str, (MAX_USER_NAME_LENGTH + 1) * MAX_GROUP_USERS, stdin);

	for (member = strtok(input_str, WHITESPACE); member != NULL;
	     member = strtok(NULL, WHITESPACE), ++member_count) {
		/* save the errors for later to show them in order */
		if (in_char_array(group_members, member_count, member)) error_repeated = 1;
		if (get_user_id(global_store, member) < 0) error_no_such_user = 1;

		strcpy(group_members[member_count], member);
	}

	if (get_user_id(global_store, group_name) >= 0) {
		printf(USER_ERR_DUPLICATE);
	} else if (global_store->users_count == MAX_USERS) {
		printf(USER_ERR_TOO_MANY);
	} else if (error_no_such_user) {
		printf(GROUP_ERR_NO_SUCH_USER);
	} else if (error_repeated) {
		printf(GROUP_ERR_REPEATED_USER);
	} else {
		add_user(global_store, group_name);
	}
}

/**
 * Checks if a string is in a group member array with size 'size'.
 * Returns 1 if so, otherwise returns 0.
 */
int in_char_array(char array[MAX_USER_NAME_LENGTH][MAX_GROUP_USERS], int size,
                  char *el) {
	int i;
	for (i = 0; i < size; ++i) {
		if (strcmp(array[i], el) == 0) {
			return 1;
		}
	}
	return 0;
}

/**
 * Auxiliary function that performs a binary search in a list of tasks,
 * using the 'compare' function passed as a parameter.
 * If the task is not present in a list, it returns a negative number
 * that can be used to find the insertion index by doing -i-1.
 */
int binary_search(const task_cmp *key, task **list, int nitems,
                  int (*compare)(const task *, const task_cmp *)) {
	int l = 0;
	int h = nitems - 1;
	int m = 0, cmp;

	while (l <= h) {
		m = (l + h) / 2;
		cmp = compare(list[m], key);
		if (cmp == 0)
			return m; /* found it */
		else if (cmp > 0)
			h = m - 1;
		else if (cmp < 0)
			l = ++m;
	}

	/* if the item hasn't been found, return an expression that can be used to
	 * find the insertion index */
	return -(l + 1);
}

/**
 * Populates the given string with the input from stdin, ignoring leading
 * spaces. If length is reached before reaching a line break or EOF,
 * this function will keep reading and discarding the output.
 * Returns the length of the string.
 */
int populate_string(char *s, int length) {
	int i = 0;
	char c;

	while ((c = getchar()) != EOF && c != '\n') {
		/* ignore leading whitespace and also input after string is full */
		if ((i != 0 && i < length - 1) || (i == 0 && !isspace(c))) {
			s[i++] = c;
		}
	}

	if (length > 0) {
		s[i] = '\0';
	}

	return i;
}
