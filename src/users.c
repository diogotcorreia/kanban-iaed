/* Diogo Correia - ist199211 */
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "proj1.h"

/**
 * Adds a new user to the Kanban global store.
 * Returns 0 if the user is created successfully.
 * If the store has reached the maximum user count, returns -1.
 * If there is already a user with the given name, returns -1.
 */
int add_user(kanban *global_store, char name[])
{
	/* verificação de argumentos */
	if (get_user_id(global_store, name) >= 0)
	{
		return -2;
	}
	if (global_store->users_count == MAX_USERS)
	{
		return -1;
	}

	strcpy(global_store->users[global_store->users_count++], name);

	return 0;
}

/**
 * Returns the index if the user with the given name.
 * If the user does not exist, returns -1.
 */
int get_user_id(kanban *global_store, char name[])
{
	int i;
	for (i = 0; i < global_store->users_count; ++i)
	{
		if (strcmp(global_store->users[i], name) == 0)
		{
			return i;
		}
	}
	return -1;
}

/**
 * Prints the name of all users, sorted by index.
 */
void list_all_users(kanban *global_store)
{
	int i;
	for (i = 0; i < global_store->users_count; ++i)
	{
		printf(USER_TO_STRING, global_store->users[i]);
	}
}