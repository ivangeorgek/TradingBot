#pragma once

#include <vector>
#include <iostream>

#include "OrderBook.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Wallet.h"


class MerkelBot
{
    public:
        MerkelBot();
        /** Call this to start the sim */
        void init();
    private: 
        /** Prints guide for user to access stats */
        void printStatsMenu();

        int getUserOption();

        void processUserOption(int input);

        /** enters an ask to orderbook */
        void enterAsk(double price, double amount, std::string product);
        /** enters a bid to orderbook */
        void enterBid(double price, double amount, std::string product);

        /** prints the sales done by bot */
        void printSales();

        /** prints the asks done by bot */
        void printAsks();

        /** prints the bids done by bot */
        void printBids();

        /** converts the order book entry into a string and cout the string */
        void printOrderBookEntry(OrderBookEntry obe);

        /** Stores the asks made by the bot */
        std::vector <OrderBookEntry>botAsks;
        /** Stores the bids made by the bot */
        std::vector <OrderBookEntry>botBids;

        /** Stores the sales by bot */
        std::vector <OrderBookEntry>botSales;

        void printWallet();

        /** Goes to next time frame and process the asks and bids in current timeframe */
        void gotoNextTimeframe();

        /** Algorithm to predict the market */
        void predictionAlgo();

        std::string currentTime;
        /** shows the previous timestamp relative to the current timestamp */
        std::string previousTime;

        OrderBook orderBook{"20200601.csv"};

        Wallet wallet;
};