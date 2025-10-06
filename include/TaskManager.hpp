#ifndef TM_H
#define TM_H

#include "Folder.hpp"
#include "CTDInfo.hpp"
#include <vector>
#include <array>

class TaskManager
{
    private:
        std::vector<Folder*> Folders;   //< vector of all folders
        CTDInfo ctdinfo; //< object CTDInfo, contains text assets, such as logo, help notes, etc
        bool displayMode; //< specifies type of output table:
                                    //true: all tasks arranged by folders (printAllTasks())
                                    //false: list of tasks related to folder or date (printTasks())
        std::vector<Task*> tasksForDisplay; //<vector of tasks to display (set by command 'display <args>')
        std::string currentError;  //<for displaying errors for incorrect input
        /**
         * @brief prints delimeter line for output table
         * @param nameLength - length of the longest task name, used to calculate table width
         * @param mode - specifies type of delimeter:
         *              //0 - upper delimeter (with corners ┏ ┓)
         *              //1 - middle delimeter (with corners ┣ ┫)
         *              //2 - bottom delimeter (with corners ┗ ┛)
         */
        void printDelimeter(const int& nameLength, const int& mode) const;

        /**
         * @brief reads tasks from "tasks/" folder on computer, creates Folders and Tasks according to data in files
         * @throw std::invalid_argument if files are corrupted
         * @note if no tasks found, creates an empty folder "General"
         * reads list of files from "tasks/list.ctdfs", then reads each file from the list
         * file format example:
         * folder=Work
         * next
         * name=Finish report
         * status=0
         * sdate=24092025
         * edate=30092025
         * next
         * name=Email client
         * status=1
         * sdate=20092025
         * edate=21092025
         */
        void readTasks();

        /**
         * @brief saves tasks to "tasks/" folder on computer, creates files if they don't exist
         * @throw std::invalid_argument if files are corrupted
         */
        void saveTasks();

        /**
        * @brief finds task by its name
        * @param taskname - name of the task to find
        * @return pointer to the found task
        * @throw std::invalid_argument if task with given name not found
        * @note if multiple tasks with same name exist, returns the first one found
        */
        Task* getTaskByName(const std::string& taskname) const;

        /**
        * @brief finds folder by its name
        * @param foldername - name of the folder to find
        * @return pointer to the found folder
        * @throw std::invalid_argument if folder with given name not found
        */
        Folder* getFolderByName(const std::string& foldername) const;

        /**
        * @brief finds folder containing the given task
        * @param targetTask - pointer to the task to find the folder for
        * @return pointer to the folder containing the task
        * @throw std::invalid_argument if task not found in any folder
        */
        Folder* getFolderByTask(Task*& targetTask) const;

        /**
        * @brief retrieves all tasks across all folders
        * @return vector of pointers to all tasks
        */
        std::vector<Task*> getAllTasks() const;

        /**
        * @brief retrieves tasks scheduled for a specific date
        * @param day - day of the month
        * @param month - month of the year (0-11)
        * @param year - year
        * @return vector of pointers to tasks scheduled for the specified date
        */
        std::vector<Task*> getTasksByDate(const int& day, const int& month, const int& year) const;

        /**
        * @brief retrieves tasks from a specific folder by its name
        * @param folderName - name of the folder to retrieve tasks from
        * @return vector of pointers to tasks in the specified folder
        * @throw std::invalid_argument if folder with given name not found
        */
        std::vector<Task*> getTasksFromFolder(const std::string& folderName) const;

        /**
        * @brief retrieves tasks from a specific folder by its pointer
        * @param targetFolder - pointer to the folder to retrieve tasks from
        * @return vector of pointers to tasks in the specified folder
        * @throw std::invalid_argument if folder not found
        */
        std::vector<Task*> getTasksFromFolder(Folder* targetFolder) const;

        /**
        * @brief moves a task to a different folder
        * @param targetTask - pointer to the task to be moved
        * @param newFolder - pointer to the folder to move the task to
        * @throw std::invalid_argument if task or folder not found
        */
        void moveTaskToFolder(Task* targetTask, Folder* newFolder);

        /**
         * @brief prints a table of all tasks arranged by folders (called by 'display all' command)
         */
        void printAllTasks() const;

        /**
         * @brief prints a table of specified tasks (called by 'display date <...>' and 'display folder <...>' commands)
         * @param tasks - vector of pointers to tasks to be printed (formed by getTasksByDate() or getTasksFromFolder() methods)
         */
        void printTasks(const std::vector<Task*>& tasks) const;

        //Utility functions for interface
        void printLogo() const;
        void clearScreen() const;
        std::array<int, 2> getTerminalSize() const;
        std::array<int, 2> getIndents(int xLen, int yLen = 0) const;
    public:
        /**
         * @brief default constructor, initializes TaskManager in interactive mode and reads tasks from files
         */
        TaskManager();

        /**
         * @brief constructor for testing purposes, initializes TaskManager in non-interactive mode without reading tasks from files
         * @param mode - mean nothing, just to differentiate from default constructor
         * @note this constructor creates a test task and two test folders containing the same task
         */
        TaskManager(const bool mode);

        /**
         * @brief parses and executes user input commands
         * @param input - user input string
         * @throw std::invalid_argument if command is unknown or arguments are invalid
         * @note supported commands:
         *      - display all
         *      - display date <DD MM YYYY>
         *      - display folder <folder_name>
         *      - edit <task_name> name <new_name>
         *      - edit <task_name> date <DD MM YYYY> or edit <task_name> date +/-<days>
         *      - new task <folder_name> <task_name> <DD MM YYYY>
         *      - new folder <folder_name>
         *      - delete task <task_name>
         *      - delete folder <folder_name>
         *      - delete done <folder_name/all>
         *      - move <task_name> <target_folder_name>
         *      - done <task_name>
         *      - undone <task_name>
         *      - about
         *      - help <command_name/all>
         *      - exit
         */
        void parseInput(const std::string& input);

        /**
         * @brief function to display the interactive interface, called in main loop
         * @note clears screen, prints logo, prints tasks according to displayMode, prompts for user input and processes it
         */
        void printInterface();
};

#endif