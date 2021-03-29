#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "proj1.h"

void setup_activities(kanban *global_store)
{
	add_activity(global_store, ACTIVITY_TODO);
	add_activity(global_store, ACTIVITY_IN_PROGRESS);
	add_activity(global_store, ACTIVITY_DONE);
}

char *get_activity(kanban *global_store, int index)
{
	return global_store->activities[index];
}

/**
 * Adiciona uma atividade ao sistema.
 * Retorna 0 caso a atividade seja criada com sucesso.
 * Caso o sistema tenha atingido o número máximo de atividades, retorna -1.
 * Caso já exista uma atividade com o mesmo nome, retorna -2.
 * Caso o nome seja inválido (isto é, não seja apenas maiúsculas e whitespace), retorna -3.
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

/* Retorna o índice se já existir uma atividade com este nome. Retorna -1 caso não exista. */
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

void list_all_activities(kanban *global_store)
{
	int i;
	for (i = 0; i < global_store->activities_count; ++i)
	{
		printf(ACTIVITY_TO_STRING, global_store->activities[i]);
	}
}