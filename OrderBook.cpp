#include <iostream>
#include <map>
#include <algorithm>

#include "OrderBook.h"
#include "CSVReader.h"


/** Reads the filename provided and stores it in orders */
OrderBook::OrderBook(std::string filename) {
    orders = CSVReader::readCSV(filename);
}

/** Returns a vector of all products in dataset using std::map */
std::vector<std::string> OrderBook::getKnownProducts() {

    std::vector<std::string> products;

    //Maps product to a boolean as a key-value
    std::map<std::string,bool> prodMap;

    //Sets each product to true
    for (OrderBookEntry& e : orders) {
        prodMap[e.product] = true;
    }
    
    // now deconstruct the map to return a vector of products
    for (auto const& e : prodMap) {
        products.push_back(e.first);
    }

    return products;
}
/** returns vector of Orders according to the set filters - type, product, timestamp*/
std::vector<OrderBookEntry> OrderBook::getOrders(   OrderBookType type, 
                                                    std::string product, 
                                                    std::string timestamp)
{
    std::vector<OrderBookEntry> filtered_orders;

    for (OrderBookEntry& e : orders) {
        if (e.orderType == type && 
            e.product == product && 
            e.timestamp == timestamp) {
                filtered_orders.push_back(e);
            }
    }
    return filtered_orders;
}


double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max)max = e.price;
    }
    return max;
}


double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min)min = e.price;
    }
    return min;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp) 
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    
    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry& order) {
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}


double OrderBook::averageAsksPrice(std::string product, std::string timestamp) {   
    double _total = 0;
    double _count = 0;
    for (OrderBookEntry& e : orders) {
        if (e.product == product && e.timestamp ==timestamp && e.orderType== OrderBookType::ask) {
           _total += e.price;
            _count += 1;
        }
    }
    return (_total/_count);
}


double OrderBook::averageAsksAmount(std::string product, std::string timestamp) {   
    double _total = 0;
    double _count = 0;
    for (OrderBookEntry& e : orders) {
        if (e.product == product && e.timestamp ==timestamp && e.orderType== OrderBookType::ask) {
           _total += e.amount;
            _count += 1;
        }
    }
    return (_total/_count);
}



double OrderBook::averageBidsPrice(std::string product, std::string timestamp) {   
    double _total = 0;
    double _count = 0;
    for (OrderBookEntry& e : orders) {
        if (e.product == product && e.timestamp ==timestamp && e.orderType== OrderBookType::bid) {
           _total += e.price;
            _count += 1;
        }
    }
    return (_total/_count);
}


double OrderBook::averageBidsAmount(std::string product, std::string timestamp) {   
    double _total = 0;
    double _count = 0;
    for (OrderBookEntry& e : orders) {
        if (e.product == product && e.timestamp ==timestamp && e.orderType== OrderBookType::bid) {
           _total += e.amount;
            _count += 1;
        }
    }
    return (_total/_count);
}



std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
// asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, 
                                                 product, 
                                                 timestamp);
// bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, 
                                                 product, 
                                                timestamp);

    // sales = []
    std::vector<OrderBookEntry> sales; 

    // I put in a little check to ensure we have bids and asks
    // to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << "OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // for ask in asks:

    
    for (OrderBookEntry& ask : asks)
    {
    //     for bid in bids:
        for (OrderBookEntry& bid : bids)
        {
    //         if bid.price >= ask.price # we have a match
            if (bid.price >= ask.price)
            {
    //             sale = new order()
    //             sale.price = ask.price
            OrderBookEntry sale{ask.price, 0, 
                                timestamp, 
                                product, 
                                OrderBookType::asksale};

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType =  OrderBookType::asksale;
                }


                if (bid.username == "Bot")
                {
                    sale.username = "Bot";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "Bot")
                {
                    sale.username = "Bot";
                    sale.orderType =  OrderBookType::asksale;
                }

            
    //             # now work out how much was sold and 
    //             # create new bids and asks covering 
    //             # anything that was not sold
    //             if bid.amount == ask.amount: # bid completely clears ask
                if (bid.amount == ask.amount)
                {
    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
    //                 sales.append(sale)
                    sales.push_back(sale);
    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
    //                 # can do no more with this ask
    //                 # go onto the next ask
    //                 break
                    break;
                }
    //           if bid.amount > ask.amount:  # ask is completely gone slice the bid
                if (bid.amount > ask.amount)
                {
    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
    //                 sales.append(sale)
                    sales.push_back(sale);
    //                 # we adjust the bid in place
    //                 # so it can be used to process the next ask
    //                 bid.amount = bid.amount - ask.amount
                    bid.amount =  bid.amount - ask.amount;
    //                 # ask is completely gone, so go to next ask                
    //                 break
                    break;
                }


    //             if bid.amount < ask.amount # bid is completely gone, slice the ask
                if (bid.amount < ask.amount && 
                   bid.amount > 0)
                {
    //                 sale.amount = bid.amount
                    sale.amount = bid.amount;
    //                 sales.append(sale)
                    sales.push_back(sale);
    //                 # update the ask
    //                 # and allow further bids to process the remaining amount
    //                 ask.amount = ask.amount - bid.amount
                    ask.amount = ask.amount - bid.amount;
    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
    //                 # some ask remains so go to the next bid
    //                 continue
                    continue;
                }
            }
        }
    }
    return sales;             
}
