#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "proj1.h"

char *get_user(kanban *global_store, int index)
{
	return global_store->users[index];
}

/**
 * Adiciona um utilizador ao sistema.
 * Retorna 0, caso o utilizador seja criado com sucesso.
 * Caso o sistema tenha atingido o número máximo de utilizadores, retorna -1.
 * Caso já exista um utilizador com o mesmo nome, retorna um utilizador -2.
 */
int add_user(kanban *global_store, char name[])
{
	/* verificação de argumentos */
	if (global_store->users_count == MAX_USERS)
	{
		return -1;
	}
	if (get_user_id(global_store, name) >= 0)
	{
		return -2;
	}

	strcpy(global_store->users[global_store->users_count++], name);

	return 0;
}

/* Retorna o índice se já existir um utilizador com este nome. Retorna -1 caso não exista. */
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

void list_all_users(kanban *global_store)
{
	int i;
	for (i = 0; i < global_store->users_count; ++i)
	{
		printf(USER_TO_STRING, global_store->users[i]);
	}
}