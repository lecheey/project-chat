#pragma once

#include "sha1.h"
#include "string.h"
#include <iostream>

#define LOGINLENGHT 4 // длина логина
#define FACTOR 8 // для увеличения массива; размер в зависимости от предполагаемого количества пользователей

// класс чат
class Chat{
public:
	Chat();
	~Chat();

	void Reg(char _login[LOGINLENGHT], char _pass[], char pass_length);
	bool LogIn(char _login[LOGINLENGHT], char _pass[], char pass_length);
	void Del(char _login[LOGINLENGHT]);
	void Show();
	void Resize();
	void addUser(char _login[LOGINLENGHT], uint* pass);
	void Renew();
	void getInfo();

private:
	enum enPairStatus{
		free,
		engaged,
		deleted
	};

	// класс пользователь
	struct User{
		User() : login(""), pass_sha1_hash(0), status(enPairStatus::free){}
		User(char _login[LOGINLENGHT], uint* sh1) : status(enPairStatus::engaged){
			strcpy(login, _login);
			pass_sha1_hash = sh1;
		}
		~User(){}

		char login[LOGINLENGHT];
		uint* pass_sha1_hash;

		enPairStatus status;
	};
	int hash_func(char _login[LOGINLENGHT], int offset);

	User* user_list;
	int list_size;
	int count;
};
