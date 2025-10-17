<img src="assets/logo.svg" alt="ConsoleToDo Logo" width="400"/>

#
![GitHub release (latest by date)](https://img.shields.io/github/v/release/kletter97/console-todo)
![GitHub license](https://img.shields.io/github/license/kletter97/console-todo)<br>
[Версия на русском](README_RU.md)

## ConsoleToDo: To-do list in your terminal. 
ConsoleToDo is a terminal program for keeping a to-do list. You can create, view, edit, sort and track your tasks — all in friendly interface in your terminal.

## Installation
- Clone the repository: 
`git clone https://github.com/kletter97/console-todo.git`;
- edit Makefile (change the compiler if required)
- type `make` to compile;
- run your binary.

Or simply download the latest release in [Releases](https://github.com/kletter97/console-todo/releases)

## Usage
- When started at the first time, ConsoleToDo creates `tasks/` folder in its path, it'll be used for storing your tasks and projects;
- On the first launch, program will create an empty task folder `General`;
- Use up and down arrows to navigate through folders;
- It's recommended to avoid non-latin letters in tasks' and folders' names;
- Type commands to work with your tasks:

|Command|Arguments|Description|
|-------|---------|-----------|
|help|<command_name/all>|Get information about commands|
|new|<folder/task> <name> <date (if task)>|Create task or folder|
|done/undone|<task_name>|Mark task as done/undone|
|display|<folder/all> <folder_name/target_date>|Set output table settings|
|edit|<name/date> <new_name/DD MM YYYY/+days/-days>|Edit task name and scheduled date|
|move|<task_name> <target_folder_name>|Move given task to given folder|
|delete|<task/folder/done> <task_name/folder_name>|Delete tasks and folders|
|about|—|Show main information about this software, such as name, version and description|
|exit|—|Save your tasks and quit safely|

To get more information, type `help all`.

## Features in development
- Folder with tasks assigned by configurable date;
- Priority-marked tasks;
- SQL integration;
- "today", "tomorrow" and weekdays aliases and more.

## License
This project is licensed under the [MIT License](LICENSE).