#pragma once

class SelectUser
{
    public:
        SelectUser();
        /** Call this to start the sim */
        void init();
    private: 
        void printOption();
        int getUserOption();
        void processUserOption(int userOption);
};
