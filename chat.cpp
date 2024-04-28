#include "chat.h"
#include <iostream>
#include "string.h"

Chat::Chat(){
	count = 0;
	list_size = 8;
	user_list = new User[list_size];
}

Chat::~Chat(){
	delete[] user_list;
	std::cout << "deleting arr\n";
}

int Chat::hash_func(char _login[LOGINLENGHT], int offset){
	int sum = 0, i = 0;
	for(; i < strlen(_login); i++){
		sum += _login[i];
	}
	const float a = 0.7;
	float midVal = sum / i;

	int r = list_size * ( a * midVal - int(a * midVal)); // Хеш-функцию для хеш-таблицы методом умножения
	return (r + offset*offset) % list_size; // метод квадратичного пробирования
}

// регистрация
void Chat::Reg(char _login[LOGINLENGHT], char _pass[], char pass_length){
	int index = -1, i = 0;
	for(; i < list_size; i++){
		index = hash_func(_login, i);
		if(user_list[index].status != enPairStatus::engaged){
			break;
		}
	}
	if(i >= list_size){
		Resize();
		Reg(_login, _pass, pass_length);
	}
	else{
		uint* h = sha1(_pass, pass_length);
		user_list[index] = User(_login, h);
		count++;
	}
}

// авторизация
bool Chat::LogIn(char _login[LOGINLENGHT], char _pass[], char pass_length){
	int i = 0;
	while(strcmp(_login, user_list[i].login) && i < list_size){
		i++;
	}
	if(i >= list_size){
		std::cout << "Incorrect login\n";
		return false;
	}
	if(user_list[i].status == enPairStatus::deleted){
		std::cout << "User has been deleted\n";
		return false;
	}

	uint* h = sha1(_pass, pass_length);
	bool result = !memcmp(user_list[i].pass_sha1_hash, h, SHA1HASHLENGTHBYTES);
	
	if(!result){
		std::cout << "Incorrect password\n";
	}

	delete[] h;
	return result;
}

// удаление пользователя
void Chat::Del(char _login[LOGINLENGHT]){
	int index = -1, i = 0;
	for(; i < list_size; i++){
		index = hash_func(_login, i);
		if((!strcmp(_login, user_list[index].login)) &&
		   user_list[index].status == enPairStatus::engaged){
			user_list[index].status = enPairStatus::deleted;
			std::cout << "deleting user\n" ;
			count--;
			return;
		}
		else if(user_list[index].status == enPairStatus::free){ return; }
	}
}

// вывод списка пользователей
void Chat::Show(){
	for(int i = 0; i < list_size; i++){
		if(user_list[i].status == enPairStatus::engaged){
			std::cout << user_list[i].login << '\n';
		}
	}
}

// метод для добавления пользователей при увеличиении массива
void Chat::addUser(char _login[LOGINLENGHT], uint* pass){
	int index = -1, i = 0;
	for(; i < list_size; i++){
		index = hash_func(_login, i);
		if(user_list[index].status != enPairStatus::engaged){
			break;
		}
	}
	user_list[index] = User(_login, pass);
	count++;
}

// увеличение массива
void Chat::Resize(){
	User* old_user_list = user_list;
	int old_list_size= list_size;

	list_size += FACTOR;
	user_list = new User[list_size];
	count = 0;
	
	for(int i = 0; i < old_list_size; i++){
		User& old_user = old_user_list[i];
		if(old_user.status == enPairStatus::engaged){
			addUser(old_user.login, old_user.pass_sha1_hash);
		}
	}

	delete[] old_user_list;
}

// функция обновления списка пользоватлей для исключения удаленных ранее
void Chat::Renew(){
	User* old_user_list = user_list;

	user_list = new User[list_size];
	count = 0;
	
	for(int i = 0; i < list_size; i++){
		User& old_user = old_user_list[i];
		if(old_user.status == enPairStatus::engaged){
			addUser(old_user.login, old_user.pass_sha1_hash);
		}
	}

	delete[] old_user_list;
}

void Chat::getInfo(){
	std::cout << "array size: " << list_size << "\nusers in system: " << count << '\n';
}
