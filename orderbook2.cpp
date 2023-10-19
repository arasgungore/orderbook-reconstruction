#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>

const double EPSILON = 1e-9; // Arbitrary number between 0.000 and 0.001, for double comparisons.

// Update struct definition.
struct Update
{
    std::string side;       // Type of update: "BID" or "ASK".
    double price, quantity; // Price and corresponding quantity.
    int sequence_num;       // Sequence no of the update.
    // Constructor of update.
    Update(const std::string &side, const double &price, const double &quantity, const int &sequence_num) : side(side), price(price), quantity(quantity), sequence_num(sequence_num) {}
};

// Snapshot struct definition.
struct Snapshot
{
    std::vector<std::pair<double, double>> bids, asks; // Keys are prices and values are quantities in these hashes.
    int sequence_num;                                  // Sequence no of the snapshot.
    // Constructor of snapshot.
    Snapshot(const std::vector<std::pair<double, double>> &bids, const std::vector<std::pair<double, double>> &asks, const int &sequence_num) : bids(bids), asks(asks), sequence_num(sequence_num) {}
};

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

    // Clears all asks and bids in the orderbook.
    void clearOrderBook()
    {
        asks.clear();
        bids.clear();
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
    // Reads the number of levels, incremental updates, snapshots, and bids and asks to output respectively.
    int N, M, Z, Y;
    std::cin >> N >> M >> Z >> Y;
    std::vector<Update> updates;     // A vector for containing all updates.
    std::vector<Snapshot> snapshots; // A vector for containing all snapshots.
    updates.reserve(M);              // Set updates capacity to M.
    snapshots.reserve(Z);            // Set snapshots capacity to Z.

    for (int i = 0; i < M + Z; i++)
    {
        std::string command;
        int sequence_num;
        std::cin >> command;
        if (command == "UPDATE")
        { // If it is an UPDATE, add it in updates vector.
            std::string side;
            double price, quantity;
            std::cin >> side >> price >> quantity >> sequence_num;
            updates.emplace_back(side, price, quantity, sequence_num);
        }
        else if (command == "SNAPSHOT")
        { // If it is a SNAPSHOT, add it in snapshots vector.
            std::cin >> sequence_num;
            std::string side;
            std::vector<std::pair<double, double>> bids, asks;
            bids.reserve(N); // Set bids and asks capacity to N.
            asks.reserve(N);
            for (int j = 0; j < 2; j++)
            {
                std::cin >> side;
                if (side == "BID") // Add the bid to bids vector.
                    for (int k = 0; k < N; k++)
                    {
                        double price, quantity;
                        std::cin >> price >> quantity;
                        bids.emplace_back(price, quantity);
                    }
                else if (side == "ASK") // Add the ask to asks vector.
                    for (int k = 0; k < N; k++)
                    {
                        double price, quantity;
                        std::cin >> price >> quantity;
                        asks.emplace_back(price, quantity);
                    }
            } // Add the snapshot to snapshots vector.
            snapshots.emplace_back(bids, asks, sequence_num);
        }
    }

    // If there are no snapshots, print nothing and terminate program.
    if (snapshots.empty())
        return 0;

    // Sort updates and snapshots according to sequence number in ascending order.
    std::sort(updates.begin(), updates.end(), [](const Update &a, const Update &b)
              { return a.sequence_num < b.sequence_num; });
    std::sort(snapshots.begin(), snapshots.end(), [](const Snapshot &a, const Snapshot &b)
              { return a.sequence_num < b.sequence_num; });

    // If the snapshot with least seq no has higher or equal seq no than all updates, print nothing and terminate program.
    if (snapshots.begin()->sequence_num >= updates.rbegin()->sequence_num)
        return 0;

    OrderBook orderbook(Y); // Creates the orderbook.
    auto current_snapshot = snapshots.begin();
    int last_update_seq_num = -1, last_snapshot_seq_num = -1; // Store last update sequence no and last snapshot sequence no received until current iteration.
    bool synced = true;                                       // Boolean value for checking synchronization among updates.
    for (const auto &update : updates)
    {
        if (last_update_seq_num != -1 && update.sequence_num != last_update_seq_num + 1)
        {
            orderbook.clearOrderBook(); // If B != A + 1, reset orderbook.
            std::cout << "SEQUENCE GAP" << std::endl;
            last_update_seq_num = update.sequence_num;
            synced = false; // Synchronization is lost in a sequence gap.
            continue;
        }
        last_update_seq_num = update.sequence_num;

        if (current_snapshot != snapshots.end() && current_snapshot->sequence_num <= update.sequence_num)
        { // If snapshot seq no is smaller than or equal to update seq no, add bids and asks to orderbook.
            // orderbook.clearOrderBook();
            for (const auto &bid : current_snapshot->bids)
                orderbook.addBid(bid.first, bid.second);
            for (const auto &ask : current_snapshot->asks)
                orderbook.addAsk(ask.first, ask.second);
            last_snapshot_seq_num = current_snapshot->sequence_num;
            synced = true; // Re-achieved synchronization.
            current_snapshot++;
            orderbook.printOrderbook(); // Prints orderbook after following snapshot.
        }
        // If snapshot seq no is smaller than update seq no, update bids/asks in orderbook.
        if (last_snapshot_seq_num != -1 && last_snapshot_seq_num < update.sequence_num)
        {
            if (update.side == "BID")
                orderbook.updateBid(update.price, update.quantity); // Updates bid in the orderbook.
            else
                orderbook.updateAsk(update.price, update.quantity); // Updates ask in the orderbook.
            if (orderbook.isValid())
            {
                if (synced)
                    orderbook.printOrderbook(); // Prints the current state of the orderbook after each update.
            }
            else
                std::cout << "INVALID" << std::endl; // Prints invalid if the orderbook is invalid.
        }
    }
    // Print the remaining snapshots after all updates have been processed.
    /*
    for(; current_snapshot != snapshots.end(); current_snapshot++) {
        orderbook.clearOrderBook();
        for(const auto& bid : current_snapshot->bids)
            orderbook.addBid(bid.first, bid.second);
        for(const auto& ask : current_snapshot->asks)
            orderbook.addAsk(ask.first, ask.second);
        orderbook.printOrderbook();     // Prints orderbook after following snapshot.
    }
    */

    return 0;
}