#ifndef CTDINFO_H
#define CTDINFO_H

#include <string>

class CTDInfo
{
    public:
        static constexpr const char* name = "ConsoleToDo";
        static constexpr const char* version = "0.2";
        static constexpr const char* publicationDate = "17.10.2025";
        static constexpr const char* description = "-----\nConsoleToDo is a terminal program for tracking tasks. You can create, view, edit, sort and track your tasks.\n-----\nAuthor:\t\tgithub.com/kletter97\nSource code:\tgithub.com/kletter97/console-todo\n-----\nType 'help all' to get more information.";
        static constexpr const char* logo = \
R"(┏━━━┓               ┏━┓ ┏━━━━━┓ ┏━━━┓ 
┃ ╻ ┣━━━┳━━━┳━━━┳━━━┫ ┣━┻━┓ ┏━┻━┫ ╻ ┣━━━┓
┃ ┣━┫ ╻ ┃ ╻ ┃ ╺━┫ ╻ ┃ ┃ ╺ ┃ ┃ ╻ ┃ ┃ ┃ ╻ ┃
┃ ╹ ┃ ╹ ┃ ┃ ┣━╸ ┃ ╹ ┃ ┃ ╺━┫ ┃ ╹ ┃ ╹ ┃ ╹ ┃
┗━━━┻━━━┻━┻━┻━━━┻━━━┻━┻━━━┻━┻━━━┻━━━┻━━━┛ v0.2)";
        CTDInfo() {}
        static const std::string getInfoText()
        {
            std::string ret = std::string(name) + ", version " + version + "\n" + description;

            return ret;
        }
        static const char* getHelp(std::string& command)
        {
            if(command=="display")
            {
                return "Description: \"display\" shows a table with your tasks according to arguments.\n\
                Syntax: display <arguments>\n\
                Arguments:\n\
                \tall: displays all tasks;\n\
                \tfolder <folder_name>: displays all tasks in selected folder.\n\
                NOTE: It's easier to use arrow-up and arrow-down keys to navigate (added in v0.2).\n\
                NOTE: argument \"date\" is removed in v0.2.";
            }
            else if(command=="edit")
            {
                return "Description: \"edit\" allows you to edit your tasks parameters: name and planned day.\n\
                Syntax: edit <taskname> <arguments>\n\
                Arguments:\n\
                \tname <new_name>: changes name of the task to given;\n\
                \tdate <DD MM YYYY>: sets planned date of selected task to given;\n\
                \tdate <+/-day>: moves planned date for given days, to the future if \"+\", to the past if \"-\". WARNING: in this version this command works correctly only for -31<given_day<31.\n";
            }
            else if(command=="about")
            {
                return "Description: \"about\" shows main information about this software, such as name, version and description.\n\
                Syntax: about\n\
                Arguments: there\'s no arguments for this command.";
            }
            else if(command=="exit")
            {
                return "Description: \"exit\" saves your tasks and exiting safely.\n\
                Syntax: exit\n\
                Arguments: there\'s no arguments for this command.";
            }
            else if(command=="done" || command=="undone")
            {
                return "Description: \"done\"/\"undone\" is a command to mark selected task done or undone.\n\
                Syntax: done/undone <task_name>\n\
                Arguments: there\'s no arguments for this command";
            }
            else if(command=="all")
            {
                return "Here\'s a list of commands supported by this version:\n\
                \t- display;\n\
                \t- edit;\n\
                \t- help;\n\
                \t- about;\n\
                \t- exit;\n\
                \t- new;\n\
                \t- delete;\n\
                \t- move.\n\
                type \"help <command_name> to get more information about specific command";
            }
            else if(command=="new")
            {
                return "Description: \"new\" creates new folder or task.\n\
                Syntax: new <arguments>\n\
                Arguments:\n\
                \ttask <folder_name> <task_name> <DD MM YYYY>: creates a new task in selected folder with given name and planned date;\n\
                \tfolder <folder_name>: creates a new folder with given name.\n";
            }
            else if(command=="delete")
            {
                return "Description: \"delete\" allows you to delete tasks and folders, which are no longer needed.\n\
                Syntax: delete <arguments>\n\
                Arguments:\n\
                \ttask <task_name>: deletes a task with given name;\n\
                \tdone <folder_name/\'all\'>: deletes all done tasks in specified folder/all folders, if given \'all\';\n\
                \tfolder <folder_name>: deletes a folder with given name.\n";
            }
            else if(command=="move")
            {
                return "Description: \"move\" moves given task to given folder.\n\
                Syntax: move <task_name> <target_folder_name>\n\
                Arguments: there\'s no arguments for this command\n";
            }
            else return "Command is not found or there\'s no description for this command.";
        }
        ~CTDInfo() {}
};

#endif
