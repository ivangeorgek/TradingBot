#include "MerkelBot.h"


MerkelBot::MerkelBot() {

}

void MerkelBot::init() {
    std::cout << " Bot trading ongoing .." <<std::endl;

    //Setting timeframes
    previousTime = orderBook.getEarliestTime();
    currentTime = orderBook.getNextTime(previousTime);

    int input;
    wallet.insertCurrency("USDT", 10000);
    wallet.insertCurrency("BTC", 5);

    //Stops running when the final timestamp is reached
    while(currentTime != "") {
        predictionAlgo();
        gotoNextTimeframe();
    }


    while(true) {
        printStatsMenu();
        input = getUserOption();
        processUserOption(input);
    }

}

void MerkelBot::printStatsMenu() {

    std::cout << "1: Print asks placed by bot " << std::endl;

    std::cout << "2: Print bids placed by bot" << std::endl;

    std::cout << "3: Print sales made " << std::endl;

    std::cout << "4: Print the wallet " << std::endl;
}

int MerkelBot::getUserOption() {
    int userOption = 0;
    std::string line;

    std::cout << "Type in 1 - 4" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        // 
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}


void MerkelBot::processUserOption(int userOption) {
    // Bad input
    if (userOption == 0) {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1) {
        printAsks();
    }
    if (userOption == 2) {
        printBids();
    }
    if (userOption == 3) {
        printSales();
    }
    if (userOption == 4) {
        printWallet();
    }
}


void MerkelBot::enterAsk(double price, double amount, std::string product) {
    OrderBookEntry obe {price,
                        amount, 
                        currentTime, 
                        product, 
                        OrderBookType::ask};

    obe.username = "Bot";

    //Inserts obe into orderBook and botAsks vector if wallet can fulfill it
    if (wallet.canFulfillOrder(obe)) {
        orderBook.insertOrder(obe);
        botAsks.push_back(obe);
    }
}


void MerkelBot::enterBid (double price, double amount, std::string product) {
    OrderBookEntry obe {price,
                        amount, 
                        currentTime, 
                        product, 
                        OrderBookType::bid};

    obe.username = "Bot";
    //Inserts obe into orderBook and botBids vector if wallet can fulfill it
    if (wallet.canFulfillOrder(obe)) {
        orderBook.insertOrder(obe);
        botBids.push_back(obe);
    }
}

void MerkelBot::predictionAlgo() {

    for (std::string p : orderBook.getKnownProducts()) {   
        double average_asks_price = orderBook.averageAsksPrice(p, currentTime);
        double average_asks_amount = orderBook.averageAsksAmount(p, currentTime);

        double average_bids_price = orderBook.averageBidsPrice(p, currentTime);
        double average_bids_amount = orderBook.averageBidsAmount(p, currentTime);

        //Compares average ASKS from current time with previous time
        if((average_asks_price - orderBook.averageAsksPrice(p, previousTime)) > 0) {
            enterAsk(average_asks_price, average_bids_amount, p);
        }
        
        //Compares average BIDS from previous time with current time
        if((orderBook.averageBidsPrice(p, previousTime) - average_bids_price) > 0) {
            enterBid(average_bids_price, average_asks_amount, p);
        }
    }
}

void MerkelBot::gotoNextTimeframe() {
    std::cout << "Going to next time frame. " << std::endl;

    for (std::string p : orderBook.getKnownProducts()) {

        //All sales are stored in sales vector
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);

        //Process the sale on wallet and stores it inside botSales vector
        for (OrderBookEntry& sale : sales) {
            if (sale.username == "Bot") {
                wallet.processSale(sale);
                botSales.push_back(sale);
            }
        }        
    }

    //Resetting timeframes
    previousTime = currentTime;
    currentTime = orderBook.getNextTime(currentTime);
}



void MerkelBot::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}

void MerkelBot::printSales() {
    if(botSales.empty()) {
        std::cout << "No sales were made" <<std::endl;

    } else {
        std::cout << "The sales are : " <<std::endl;
        for (OrderBookEntry& sale : botSales) {
            MerkelBot::printOrderBookEntry(sale);
        }
    }
}

void MerkelBot::printAsks() {
    if(botAsks.empty()) {
    std::cout << "No Asks were placed" <<std::endl;

    } else {
        std::cout << "The asks are : " <<std::endl;
        for (OrderBookEntry& ask : botAsks) {
            MerkelBot::printOrderBookEntry(ask);
        }
    }
}

void MerkelBot::printBids() {
    if(botBids.empty()) {
    std::cout << "No Bids were placed" <<std::endl;

    } else {
        std::cout << "The bids are : " <<std::endl;
        for (OrderBookEntry& bid : botBids) {
            MerkelBot::printOrderBookEntry(bid);
        }
    }
}

void MerkelBot::printOrderBookEntry(OrderBookEntry obe) {
    std::string s;
    s += obe.timestamp + ", " + obe.product + ", " + std::to_string(obe.price) + ", " + std::to_string(obe.amount);
    std::cout << s << std::endl;
}