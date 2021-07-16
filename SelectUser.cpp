#include <vector>
#include <iostream>
#include <string>

#include "SelectUser.h"
#include "MerkelMain.h"
#include "MerkelBot.h"


SelectUser::SelectUser()
{

}

void SelectUser::init()
{
    int input;

    printOption();
    input = getUserOption();
    processUserOption(input);

}


void SelectUser::printOption()
{

    std::cout << "Welcome to Merkel App : Choose one of the option by entering the corresponding number" << std::endl;

    std::cout << "1: Activates Merkel Bot that automatically trades to make a profit" << std::endl;

    std::cout << "2: For User Trading by making bids and asks " << std::endl;

}

int SelectUser::getUserOption()
{
    int userOption = 0;     //Default user option 
    std::string line;

    std::cout << "Type in 1 or 2" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);   //Converts user input to integer
    }catch(const std::exception& e)
    {
        
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}


void SelectUser::processUserOption(int userOption) {
    if (userOption == 0) {
        std::cout << "Invalid choice. Choose 1 or 2" << std::endl;
    }

    if (userOption == 1) {
        std::cout << "You choose the Merkel Bot" <<std::endl;
        MerkelBot BotApp{};
        BotApp.init();
    }

    if (userOption == 2) {
        std::cout << "You choose the user trading option" <<std::endl;
        MerkelMain app{};
        app.init();
    }
}
