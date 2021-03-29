#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "users.h"

static user users[MAX_USERS];
static int user_count = 0;

user get_user(int index)
{
	return users[index];
}

/**
 * Adiciona um utilizador ao sistema.
 * Retorna a estrutura que representa o novo utilizador.
 * Caso o sistema tenha atingido o número máximo de utilizadores, retorna
 * um utilizador cujo status é -1.
 * Caso já exista um utilizador com o mesmo nome, retorna um utilizador
 * cujo status é -2.
 */
user add_user(char name[])
{
	user new_user;

	/* verificação de argumentos */
	if (user_count == MAX_USERS)
	{
		new_user.status = -1;
		return new_user;
	}
	if (is_duplicate_user(name))
	{
		new_user.status = -2;
		return new_user;
	}

	strcpy(new_user.name, name);
	new_user.status = 0;
	users[user_count++] = new_user;

	return new_user;
}

/* Retorna 1 se já existir um utilizador com este nome. Retorna 0 em caso contrário. */
int is_duplicate_user(char name[])
{
	int i;
	for (i = 0; i < user_count; ++i)
	{
		if (strcmp(users[i].name, name) == 0)
		{
			return 1;
		}
	}
	return 0;
}

void list_all_users()
{
	int i;
	for (i = 0; i < user_count; ++i)
	{
		printf(USER_TO_STRING, users[i].name);
	}
}