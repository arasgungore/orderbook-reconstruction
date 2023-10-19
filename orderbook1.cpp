#include <iostream>
#include <map>
#include <iomanip>

const double EPSILON = 1e-9; // Arbitrary number between 0.000 and 0.001, for double comparisons.

// Orderbook class definition.
class OrderBook
{
private:
    // Keys are prices and values are quantities in these hashes.
    std::map<double, double, std::greater<double>> bids; // Bid side is ordered in descending order.
    std::map<double, double> asks;                       // Ask side is ordered in ascending order.
    const int Y;                                         // Number of bids and asks to output.

public:
    OrderBook(const int &Y) : Y(Y) {} // Constructor of orderbook.

    // Adds bids to initial orderbook.
    void addBid(const double &price, const double &quantity)
    {
        bids[price] = quantity;
    }

    // Adds asks to initial orderbook.
    void addAsk(const double &price, const double &quantity)
    {
        asks[price] = quantity;
    }

    // Updates bids in the orderbook, deletes prices with zero quantities.
    void updateBid(const double &price, const double &quantity)
    {
        if (quantity > EPSILON)
            bids[price] = quantity;
        else
            bids.erase(price);
    }

    // Updates asks in the orderbook, deletes prices with zero quantities.
    void updateAsk(const double &price, const double &quantity)
    {
        if (quantity > EPSILON)
            asks[price] = quantity;
        else
            asks.erase(price);
    }

    // Checks if the orderbook is valid, that is no ask is less than or equal to any bid.
    bool isValid() const
    {
        if (bids.empty() || asks.empty())
            return true;
        return asks.begin()->first > bids.begin()->first;
    }

    // Prints the current state of the orderbook.
    void printOrderbook() const
    {
        // Prints the best Y bids.
        int printedBids = 0;
        for (const auto &level : bids)
        {
            if (printedBids == Y)
                break;
            std::cout << std::fixed << std::setprecision(1) << level.first << " "
                      << std::fixed << std::setprecision(3) << level.second << " ";
            printedBids++;
        }
        while (printedBids < Y)
        {
            std::cout << "0.0 0.000 "; // Prints "0.0 0.000" for each missing level in bids.
            printedBids++;
        }
        std::cout << std::endl;

        // Prints the best Y asks.
        int printedAsks = 0;
        for (const auto &level : asks)
        {
            if (printedAsks == Y)
                break;
            std::cout << std::fixed << std::setprecision(1) << level.first << " "
                      << std::fixed << std::setprecision(3) << level.second << " ";
            printedAsks++;
        }
        while (printedAsks < Y)
        {
            std::cout << "0.0 0.000 "; // Prints "0.0 0.000" for each missing level in asks.
            printedAsks++;
        }
        std::cout << std::endl;
    }
};

int main()
{
    // Reads the number of levels, incremental updates and bids and asks to output.
    int N, M, Y;
    std::cin >> N >> M >> Y;
    OrderBook orderbook(Y); // Creates the orderbook.

    // Adds bids to initial orderbook.
    for (int i = 0; i < N; i++)
    {
        double price, quantity;
        std::cin >> price >> quantity;
        orderbook.addBid(price, quantity);
    }

    // Adds asks to initial orderbook.
    for (int i = 0; i < N; i++)
    {
        double price, quantity;
        std::cin >> price >> quantity;
        orderbook.addAsk(price, quantity);
    }

    orderbook.printOrderbook(); // Prints orderbook after following reception.

    for (int i = 0; i < M; i++)
    {
        std::string side;
        double price, quantity;
        std::cin >> side >> price >> quantity;
        if (side == "BID")
            orderbook.updateBid(price, quantity); // Updates bid in the orderbook.
        else if (side == "ASK")
            orderbook.updateAsk(price, quantity); // Updates ask in the orderbook.

        if (orderbook.isValid())
            orderbook.printOrderbook(); // Prints the current state of the orderbook after each update.
        else
            std::cout << "INVALID" << std::endl; // Prints invalid if the orderbook is invalid.
    }

    return 0;
}