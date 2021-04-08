/* Diogo Correia - ist199211 */
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "proj1.h"

/**
 * Adds the default activities (TO DO, IN PROGRESS and DONE)
 * to the Kanban global store.
*/
void setup_activities(kanban *global_store)
{
	add_activity(global_store, ACTIVITY_TODO);
	add_activity(global_store, ACTIVITY_IN_PROGRESS);
	add_activity(global_store, ACTIVITY_DONE);
}

/**
 * Returns the name of the activity by the index in the global store.
*/
char *get_activity(kanban *global_store, int index)
{
	return global_store->activities[index];
}

/**
 * Adds an activity to the global store.
 * Returns 0 if the activity is created successfully.
 * If the store has reached the maximum number of activities, returns -1.
 * If there is already an activity with the same name, returns -2.
 * If the name is invalid (that is, it has lowercase characters), returns -3.
 */
int add_activity(kanban *global_store, char name[MAX_ACTIVITY_NAME_LENGTH])
{
	/* verificação de argumentos */
	if (global_store->activities_count == MAX_ACTIVITIES)
	{
		return -1;
	}
	if (get_activity_id(global_store, name) >= 0)
	{
		return -2;
	}
	if (is_invalid_activity_name(name))
	{
		return -3;
	}

	strcpy(global_store->activities[global_store->activities_count++], name);

	return 0;
}

/**
 * Returns the index of the activity with the given name. Returns -1 if it does not exist.
 */
int get_activity_id(kanban *global_store, char name[])
{
	int i;
	for (i = 0; i < global_store->activities_count; ++i)
	{
		if (strcmp(global_store->activities[i], name) == 0)
		{
			return i;
		}
	}
	return -1;
}

/**
 * Returns 1 if the given activity name is invalid.
 * Returns 0 otherwise.
 */
int is_invalid_activity_name(char name[])
{
	int i;
	char c;
	for (i = 0; i < MAX_ACTIVITY_NAME_LENGTH; ++i)
	{
		c = name[i];
		if (c == '\0')
		{
			break;
		}
		if (islower(c))
		{
			return 1;
		}
	}
	return 0;
}

/**
 * Prints the name of all activities, ordered by index.
 */
void list_all_activities(kanban *global_store)
{
	int i;
	for (i = 0; i < global_store->activities_count; ++i)
	{
		printf(ACTIVITY_TO_STRING, global_store->activities[i]);
	}
}