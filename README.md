# orderbook-reconstruction

A C++ project which implements an orderbook and outputs the best bids and asks.



## Description

### Orderbook Reconstruction I

In this task, you need to process a series of orderbook updates in order to represent the current orderbook state and output the best Y bids and asks.

An orderbook consists of two sides, a BID side and an ASK side. The bid side is ordered in descending order, with the best bid being the one with the highest price. The ask side is ordered in ascending order, with the best ask being the one with the lowest price. Each price will have an associated quantity. Prices have a precision of .1 and quantities have a precision of .001.

You will be provided with a snapshot representing the initial orderbook state. This will be represented by two lines, the first line being for the bid side and the second line being for the ask side. These lines will consist of a sequence of doubles representing price and quantity in an alternating manner ([price][quantity][price][quantity] ...), with quantity at position i being the quantity for price at position i-1. This snapshot will represent the best N levels, excluding price levels with a quantity of 0.

Following this you will receive incremental updates to the orderbook in the format [side][price][quantity], with side being either BID or ASK. Note that these updates may result in the creation of levels outside of the initial snapshot depth, and that the best ask and bid may change.

Input:
The first line will contain three integers, N, M and Y, with N the number of levels in the snapshot, M the number of incremental updates, and Y the number of bids and asks to output.
The second line will be the bid snapshot, consisting of N * 2 doubles, with doubles at positions 2i representing prices and doubles at position 2i+1 representing quantity for the price at position 2i.
The third line will be the ask snapshot, consisting of N * 2 doubles, with doubles at positions 2i representing prices and doubles at position 2i+1 representing quantity for the price at position 2i. Following this will be M lines in [side][price][quantity] format representing the incremental updates.

Output:
Following reception of the snapshot, and after each update, you must print the current state of the orderbook if it is valid. A valid orderbook state is one for which no ask is less than or equal to any bid. The orderbook state may be invalid at times due to updates having come through for one side before the other (see updates 3 and 4 of the first example). To output the orderbook state, first print one line representing the best Y bids in format [price][quantity][price][quantity], followed by one line representing the best Y asks in format [price][quantity][price][quantity]. This must be excluding price levels for which the quantity is 0. In the case that there are not Y price levels with a non-zero quantity, output those with a non-zero quantity followed by "0.0 0.000" as many times as there are missing levels (so if we have L valid levels, print "0.0 0.000" Y - L times).
In the case that the orderbook is invalid, simply output one line consisting of the word "INVALID".


### Orderbook Reconstruction II

As in the previous task, you need to process a series of orderbook updates in order to represent the current orderbook state and output the best Y bids and asks. However, the orderbook snapshot is now coming over a different channel than the incremental updates, meaning it might not arrive in sequence with the updates. You will have to ensure that it is synchronised and updates are applied in order.

An orderbook consists of two sides, a BID side and an ASK side. The bid side is ordered in descending order, with the best bid being the one with the highest price. The ask side is ordered in ascending order, with the best ask being the one with the lowest price. Each price will have an associated quantity. Prices have a precision of .1 and quantities have a precision of .001.

You will be provided with a snapshot representing the initial orderbook state. Snapshots will be declared first with a line in the format SNAPSHOT [sequence number]. This will be followed by two lines in the format [side][price][quantity][price][quantity] ... , with quantity at position i being the quantity for price at position i-1 and side being either BID or ASK. This snapshot will represent the best N levels, excluding price levels with a quantity of 0. The bid and ask lines of a snapshot both share the same sequence number.

Following this you will receive incremental updates to the orderbook in the format UPDATE [side][price][quantity][sequence number], with side being either BID or ASK. Note that these updates may result in the creation of levels outside of the initial snapshot depth, and that the best bid and ask may change. At the end of a line there will be an integer sequence number.

You will need to synchronise the incremental updates with the snapshot by ensuring that you do not apply any updates to the orderbook until the snapshot is received. For a snapshot with sequence number K, you will want to apply only updates starting from sequence number K+1.

There are two possible issues with synchronising the snapshot - it is up to you to determine how to resolve these:
1. The snapshot sequence number is smaller than the preceding update.
2. The snapshot sequence number is larger than the following update.

The incremental updates are being received via UDP. Unfortunately, UDP may experience packet loss. As a result, there may be sequence gaps in the incremental updates such that, following synchronisation, there is a gap between two updates with sequence numbers A and B where B != A + 1. If this occurs on an update which sequentially comes after the snapshot, the orderbook state will no longer be valid, and should be reset - another snapshot may follow which you would be able to sync with an update which succeeds the sequence gap. If a sequence gap occurs, you should output "SEQUENCE GAP". Do not print output in-between the sequence gap and having re-achieved synchronization. Note that these gaps will only happen at times when you should have a valid orderbook following snapshot synchronization.

Evaluation criteria:
- This is not a competitive programming exercise - take your time, and feel free to conduct research.
- The emphasis of the assessment is on code cleanliness, design and organisation, as well as time complexity and performance, alongside correctness.
- You are expected to design an orderbook data structure with an API clearly separated from input parsing.
- While there is a straightforward solution, this is an open problem for which creative solutions are possible - these will be highly valued.

Input:
The first line will contain three integers, N, M, Z and Y, with N the number of levels in the snapshot, M the number of incremental updates, Z the number of snapshots and Y the number of bids and asks to output.
Following this will be M + Z * 3 lines.
The M part of these lines will be incremental updates in UPDATE [side][price][quantity][sequence number] format.
The Z part of these lines will be one or multiple snapshots, as snapshots will be re-sent if there is a sequence gap in the updates.
The first snapshot line will be the snapshot declaration in SNAPSHOT [sequence number] format.
The second and third snapshot lines will be the bid and ask snapshots in [side][price][quantity][price][quantity] ... [sequence number] format, consisting of N * 2 doubles, with doubles at positions 2i representing prices and doubles at position 2i+1 representing quantity for the price at position 2i, followed by an integer sequence number. There will always be one line for the BID side and one line for the ASK side.

Output:
You must print the orderbook state for the snapshot and all following updates as ordered by the sequence number if it is valid. For example, if receiving updates with sequence numbers 3, 4, then 5, followed by the snapshot with sequence number 2, you should print output as if receiving the snapshot followed by the updates (so 2->3->4->5). Updates that are not necessarily invalid but are not syncable with snapshot (ie: sequence number is lower than the snapshot's) should just be ignored - do not print any output at times when snapshot sync has not occured.
A valid orderbook state is one for which no ask is less than or equal to any bid. The orderbook state may be invalid at times due to updates having come through for one side before the other (see updates 10 and 11 of the first example).
To output the orderbook state, first print one line representing the best Y bids in format [price][quantity][price][quantity], followed by one line representing the best Y asks in format [price][quantity][price][quantity]. This must be excluding price levels for which the quantity is 0. In the case that there are not Y price levels with a non-zero quantity, output those with a non-zero quantity followed by "0.0 0.000" as many times as there are missing levels (so if we have L valid levels, print "0.0 0.000" Y - L times).
In the case that the orderbook is invalid, simply output one line consisting of the word "INVALID".
In the case that there is a sequence gap, ie the sequence number of the current update is not equal to the sequence number of the previous update + 1, output "SEQUENCE GAP" for that update.



## Run on Terminal

### Orderbook 1
```sh
g++ orderbook1.cpp -o o1
o1
```

### Orderbook 2
```sh
g++ orderbook2.cpp -o o2
o2
```



## Author

👤 **Aras Güngöre**

* LinkedIn: [@arasgungore](https://www.linkedin.com/in/arasgungore)
* GitHub: [@arasgungore](https://github.com/arasgungore)
