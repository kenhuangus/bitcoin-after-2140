#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

// A simple representation of a transaction
struct Transaction {
    std::string sender;
    std::string receiver;
    double amount;
    double fee;
};

// Representation of a block in the blockchain
struct Block {
    int blockNumber;
    std::vector<Transaction> transactions;
    double totalFees;

    Block(int num) : blockNumber(num), totalFees(0.0) {}
};

// Representation of a miner
class Miner {
public:
    std::string minerID;
    double earnedFees;

    Miner(const std::string& id) : minerID(id), earnedFees(0.0) {}

    void mineBlock(Block& block) {
        earnedFees += block.totalFees;
        std::cout << "Miner " << minerID << " mined block " << block.blockNumber << " and earned fees: " << earnedFees << "\n";
    }
};

// Decentralized Mining Network class with Dynamic Fee Structure
class MiningNetwork {
private:
    std::vector<Miner> miners;
    std::map<int, Block> blockchain;
    double baseFeeRate;

public:
    MiningNetwork() : baseFeeRate(0.01) {} // Initial base fee rate

    void addMiner(const std::string& minerID) {
        miners.emplace_back(minerID);
    }

    void addBlock(int blockNumber, std::vector<Transaction>& transactions) {
        adjustFees(transactions);
        Block newBlock(blockNumber);
        newBlock.transactions = transactions;
        for (const auto& tx : transactions) {
            newBlock.totalFees += tx.fee;
        }
        blockchain[blockNumber] = newBlock;
        assignBlockToMiner(newBlock);
    }

    void adjustFees(std::vector<Transaction>& transactions) {
        double networkLoad = calculateNetworkLoad();
        double adjustedFeeRate = baseFeeRate * (1.0 + networkLoad);
        for (auto& tx : transactions) {
            tx.fee = tx.amount * adjustedFeeRate;
        }
    }

    double calculateNetworkLoad() {
        // Calculate network load based on the number of pending transactions in the blockchain
        double loadFactor = 0.0;
        int totalTransactions = 0;
        for (const auto& block : blockchain) {
            totalTransactions += block.second.transactions.size();
        }
        loadFactor = static_cast<double>(totalTransactions) / 10.0; // Assuming 10 transactions is average load
        return loadFactor;
    }

    void assignBlockToMiner(Block& block) {
        // Randomly assign block to a miner
        std::shuffle(miners.begin(), miners.end(), std::default_random_engine(static_cast<unsigned int>(time(0))));
        auto& selectedMiner = miners.front();
        selectedMiner.mineBlock(block);
    }

    void displayMinerStatus() {
        for (const auto& miner : miners) {
            std::cout << "Miner ID: " << miner.minerID << ", Earned Fees: " << miner.earnedFees << "\n";
        }
    }
};

int main() {
    MiningNetwork network;
    network.addMiner("MinerA");
    network.addMiner("MinerB");
    network.addMiner("MinerC");

    // Add some blocks with transactions
    std::vector<Transaction> transactions1 = { {"Alice", "Bob", 1.0, 0.01}, {"Charlie", "Dave", 2.0, 0.015} };
    network.addBlock(1, transactions1);

    std::vector<Transaction> transactions2 = { {"Eve", "Frank", 1.5, 0.02}, {"Grace", "Heidi", 3.0, 0.03} };
    network.addBlock(2, transactions2);

    // Add a block with low transaction fees to show the effect of dynamic fee adjustment
    std::vector<Transaction> transactions3 = { {"Ivan", "Judy", 0.5, 0.005} };
    network.addBlock(3, transactions3);

    network.displayMinerStatus();

    return 0;
}
