#ifndef USERS_H
#define USERS_H

#define MAX_USER_NAME_LENGTH 20
#define MAX_USERS 50

#define USER_TO_STRING "%s\n"
#define USER_ERR_TOO_MANY "too many users\n"
#define USER_ERR_DUPLICATE "user already exists\n"

typedef struct
{
	char name[MAX_USER_NAME_LENGTH];
	int status;
} user;

user get_user(int index);

user add_user(char name[]);

int get_user_id(char name[]);

void list_all_users();

#endif