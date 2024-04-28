#include <iostream>
#include <string>

#include "sha1.h"
#include "chat.h"

int main(){
	Chat chat;
	// примеры пользователей
	chat.Reg((char*)"john", (char*)"1111",4);
	chat.Reg((char*)"dave", (char*)"2222",4);
	chat.Reg((char*)"mary", (char*)"3333",4);
	chat.Reg((char*)"katy", (char*)"4444",4);
	
	// удаление пользовтеля
	chat.Del((char*)"mary");

	// авторизация
	while(true){
		chat.getInfo(); // размер массива и количество пользоватлей
		chat.Show(); // список пользователей
		
		// логин
		std::string login;
		std::cout << "login: ";
		std::getline(std::cin, login);
		char* l = login.data();
		
		// пароль
		std::string pass;
		std::cout << "password: ";
		std::getline(std::cin, pass);
		char* p = pass.data();
		
		// проверка
		bool a = chat.LogIn(l, p, size(pass));
		if(a){
			std::cout << "user's logged in\n";
		}
		
		chat.Resize(); // принудительное увеличение массива для проверки

		std::cout << "continue? y/n\n";
		char c;
		std::cin >> c;

		// выход из цикла
		if(c == 'n'){
			break;
		}
		cin.ignore(256, '\n');
	}
	chat.Renew(); // массив пользователей будет обновляться перед выходом

	return 0;
}
