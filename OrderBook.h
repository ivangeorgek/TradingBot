#pragma once
#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"

class OrderBook
{
    public:
    /** construct, reading a csv data file */
        OrderBook(std::string filename);
    /** return vector of all know products in the dataset*/
        std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                              std::string product, 
                                              std::string timestamp);

        /** returns the earliest time in the orderbook*/
        std::string getEarliestTime();

        /** returns the next timestamp and ends when it reaches the final timestamp */
        std::string getNextTime(std::string timestamp);


        void insertOrder(OrderBookEntry& order);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);


        /** returns the average price of ASKS of a particular product during a given timestamp */
        double averageAsksPrice(std::string product, std::string timestamp);
        /** returns the average "amount" of ASKS of a particular product during a given timestamp */
        double averageAsksAmount(std::string product, std::string timestamp);

        /** returns the average price of BIDS of a particular product during a given timestamp */
        double averageBidsPrice(std::string product, std::string timestamp);
        /** returns the average "amount" of BIDS of a particular product during a given timestamp */
        double averageBidsAmount(std::string product, std::string timestamp);

    private:
        std::vector<OrderBookEntry> orders;

};
