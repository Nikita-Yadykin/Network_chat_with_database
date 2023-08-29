Сетевой Чат с использованием контейнеров стандартной библиотеки шаблонов

В проекте выделили 3 класса: User (пользователи чата), Message (для обмена сообщениями) и ConsoleChat (реализация основных функций). При входе в программу пользователь выполняет регистрацию, входит в чат и имеет возможность написать сообщение как в общий чат, так и конкретному пользователю. После того как пользователь выйдет из системы, другой пользователь сможет выполнить те же действия.

Описание пользовательских типов и функций в проекте:

Класс User: принимает в конструктор логин, пароль и имя пользователя

getLogin()-Получить логин пользователя  
getPassword()-Получить пароль пользователя  
getName()-получить имя пользователя  

Класс Message: принимает в конструктор отправителя, получателя и текст сообщения

getSender()-Получить отправителя  
getRecipient()-Получить получателя  
getLetter()-Получить текст сообщения  

Класс ConsoleChat: Реализует основной функционал

start()-Переменная _сhatStarted принимает значение true  
сhatStarted()-Возвращает значение переменной _сhatStarted  
startMenu()-Стартовое меню программы, возможность зарегистрироваться, войти в чат, узнать имена всех пользователей и выйти из программы  
signUp()-Регистрация в чате, необходимо задать логин, пароль и имя  
logIn()-Вход в чат, необходимо указать логин и пароль  
getOnlineUser()-Возвращает текущего пользователя  
chatMenu()-Меню чата, позволяет просмотреть чат, написать сообщение и выйти из чата  
sendMessage()-Позволяет написать сообщение, в качестве адресата необходимо указать конкретного получателя/всех пользователей  
getUserByLogin(const string login)-Возвращает указатель на пользователя, если его логин совпадает с указанным  
openChat()-Отображает чат  
showAllUserNames()-Выводит имена всех пользователй  
hashPassword()-Функция для хеширования пароля  
verifyPassword()-Функция для проверки соответствия хэша пароля   

Класс sha1: Реализует криптографическую хэш-функцию

Класс исключения LoginException:public exception, наследуется от класса exception.
метод what() выводит строку с предупреждением, если указанный логин уже занят

Функция main() - Происходит создание класса ConsoleChat, пока чат запущен отображается стартовое меню, если есть онлайн пользователь, то отображается меню чата

Для OC Windows реализована цветовая поддержка консоли


Для связи с БД MySQL установить  mysql-connector-c++-8.0.33-win32.msi
база данных для root установить пароль password
после уставновки выполнить 
mysql -h localhost -u root -p

mysql>create database chatdb;
mysql>use chatdb;

После создания базы данных выполнить скрипты:
CREATE TABLE `table_users` (
  `id` int NOT NULL AUTO_INCREMENT COMMENT 'Primary Key',
  `create_time` datetime DEFAULT CURRENT_TIMESTAMP COMMENT 'Create Time',
  `name` varchar(255) NOT NULL,
  `login` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
)

CREATE TABLE `table_messages` (
  `id` int NOT NULL AUTO_INCREMENT COMMENT 'Primary Key',
  `create_time` datetime DEFAULT CURRENT_TIMESTAMP COMMENT 'Create Time',
  `letter` varchar(255) DEFAULT NULL,
  `sender_id` int NOT NULL,
  `recepient_id` int DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `sender_ind` (`sender_id`),
  KEY `recepient_ind` (`recepient_id`),
  CONSTRAINT `table_messages_ibfk_1` FOREIGN KEY (`sender_id`) REFERENCES `table_users` (`id`) ON DELETE CASCADE,
  CONSTRAINT `table_messages_ibfk_2` FOREIGN KEY (`recepient_id`) REFERENCES `table_users` (`id`) ON DELETE CASCADE
)

Server компилировать в RELEASE X32 (не Debug, под ним в windows c mysqlconnector баг)

Настройка Microsoft Visual Studio:
Project->Properties->C/C++->General->Additional Include Directories: D:\libraries\mysql8.0.33-32\include\jdbc;%(AdditionalIncludeDirectories)
Project->Properties->C/C++->Preprocessor->Preprocessor Definitions: STATIC_CONCPP;WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)
Project->Properties->Linker->General->Additional Library Directories: D:\libraries\mysql8.0.33-32\lib\vs14;%(AdditionalLibraryDirectories)
Project->Properties->Linker->Input->Additional Dependencies: mysqlcppconn-static.lib;%(AdditionalDependencies)

Используется протокол TCP и архитектура клиент-сервер.

класс NetWorkStream: экземпляр класса используется вместо стандартных объектов std::cin и std::cout

