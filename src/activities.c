#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "activities.h"

static activity activities[MAX_ACTIVITIES];
static int activity_count = 0;

void setup_activities()
{
	add_activity(ACTIVITY_TODO);
	add_activity(ACTIVITY_IN_PROGRESS);
	add_activity(ACTIVITY_DONE);
}

activity get_activity(int index)
{
	return activities[index];
}

/**
 * Adiciona uma atividade ao sistema.
 * Retorna a estrutura que representa a nova atividade.
 * Caso o sistema tenha atingido o número máximo de atividades, retorna
 * uma atividade cujo status é -1.
 * Caso já exista uma atividade com o mesmo nome, retorna uma atividade
 * cujo status é -2.
 * Caso o nome seja inválido (isto é, não seja apenas maiúsculas e whitespace),
 * retorna uma atividade cujo status é -3.
 */
activity add_activity(char name[MAX_ACTIVITY_NAME_LENGTH])
{
	activity new_activity;

	/* verificação de argumentos */
	if (activity_count == MAX_ACTIVITIES)
	{
		new_activity.status = -1;
		return new_activity;
	}
	if (is_duplicate_activity_name(name))
	{
		new_activity.status = -2;
		return new_activity;
	}
	if (is_invalid_activity_name(name))
	{
		new_activity.status = -3;
		return new_activity;
	}

	strcpy(new_activity.name, name);
	new_activity.status = 0;
	activities[activity_count++] = new_activity;

	return new_activity;
}

/* Retorna 1 se já existir uma atividade com este nome. Retorna 0 em caso contrário. */
int is_duplicate_activity_name(char name[])
{
	int i;
	for (i = 0; i < activity_count; ++i)
	{
		if (strcmp(activities[i].name, name) == 0)
		{
			return 1;
		}
	}
	return 0;
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

void list_all_activities()
{
	int i;
	for (i = 0; i < activity_count; ++i)
	{
		printf(ACTIVITY_TO_STRING, activities[i].name);
	}
}