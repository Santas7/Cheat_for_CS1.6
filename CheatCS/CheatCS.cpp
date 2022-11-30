#include <iostream>
#include <algorithm>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <unordered_map>
#include <string>
#include <map>
#include <typeinfo>
#include <stack>
#include "memoryapi.h"
#include <conio.h>


#define ADDRESS_MEMORY 0x254C54D4
#define CS_MODULE_NAME "hl.exe"
#define CLIENT_DLL_MODULE_NAME "client.dll"


int get_key() {
    int key = _getch();
    if (key == 27 || key == 49 || key == 50 || key == 51 || key == 52 || key == 53 || key == 54 || key == 55 || key == 56 || key == 57 || key == 8)
        return key;
    return 0;
}


std::uint32_t find_cs_proc_id()
{
    HANDLE process_of_counter_strike = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (process_of_counter_strike == NULL)
    {
        throw std::out_of_range("Process CS1.6 if failed!");
    }

    PROCESSENTRY32 pe32{ sizeof pe32 }; // Describes an entry from a list of the processes residing in the system address space when a snapshot was taken.
    // pe32.dwSize = sizeof(pe32); // The size of the structure, in bytes. 

    Process32First(process_of_counter_strike, &pe32); // Retrieves information about the first process encountered in a system snapshot.

    do
    {
        if (std::string(pe32.szExeFile) == CS_MODULE_NAME) // szExeFile - The name of the executable file for the process. 
            return pe32.th32ProcessID; // return the process identifier.

    } while (Process32Next(process_of_counter_strike, &pe32)); // Retrieves information about the next process recorded in a system snapshot.
    return 0;
}


std::map<std::uint32_t, int> get_address_answer_and_set_value_of_money(const int& money = 16000)
{
   
    if (money <= 0 || money > 16000)
        throw std::out_of_range("The value of the variable <money> is out of bounds!");


    std::map<std::uint32_t, int> value{
            {ADDRESS_MEMORY, money},
    };

    return value;
}


void menu_program()
{
    std::cout << "[1] Set Money and writing to RAM" << std::endl << "[BackSpace] Exit.." << std::endl << "--> ";
    while (true) 
    {
        int command_key = get_key();

        if (command_key == 8) 
        {
            system("cls");
            std::cout << "Bye... waiting for you again...)";
            break;
        }

        switch (command_key) 
        {
            case '1':
            {
                system("cls");
                try
                {
                    // money set
                    int money_set;
                    std::cout << "Set value money--> ";
                    std::cin >> money_set;

                    auto value = get_address_answer_and_set_value_of_money(money_set);
                    DWORD id_process = find_cs_proc_id();
                    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, id_process);
                    if (!process)
                    {
                        throw std::out_of_range("error");
                    }
                    else
                    {
                        if (WriteProcessMemory(process, (LPVOID)ADDRESS_MEMORY, &value[ADDRESS_MEMORY], (DWORD)sizeof(value[ADDRESS_MEMORY]), NULL) <= 0)
                        {
                            throw std::out_of_range("Error when writing to RAM!");
                        }
                        std::cout << "The data has been successfully written to the computer's RAM!" << std::endl;
                    }
                    CloseHandle(process);
                }
                catch (const std::out_of_range& err)
                {
                    std::cerr << err.what() << std::endl;
                }
                break;
            }
            case 0:
            {
                system("cls");
                std::cout << "[1] Set Money and writing to RAM" << std::endl << "[BackSpace] Exit.." << std::endl << "--> ";
                break;
            }
            default:
            {
                system("cls");
                std::cout << "Error!" << std::endl;
                break;
            }
        }
    }
}


int main()
{
    menu_program();
    return 0;
}

