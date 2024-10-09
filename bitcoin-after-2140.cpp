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
    double reputationScore; // Flywheel mechanism to encourage decentralization
    double baseReward; // Base reward to incentivize mining even with low transaction traffic
    double bonusReward; // Bonus reward for maintaining network stability

    Miner(const std::string& id) : minerID(id), earnedFees(0.0), reputationScore(1.0), baseReward(0.1), bonusReward(0.05) {}

    void mineBlock(Block& block) {
        // Reward miner based on fees, reputation score, base reward, and bonus reward
        earnedFees += (block.totalFees * reputationScore) + baseReward + bonusReward;
        reputationScore += 0.01; // Increment reputation to incentivize further mining
        std::cout << "Miner " << minerID << " mined block " << block.blockNumber << " and earned fees: " << earnedFees << "\n";
    }
};

// Decentralized Mining Network class
class MiningNetwork {
private:
    std::vector<Miner> miners;
    std::map<int, Block> blockchain;

public:
    void addMiner(const std::string& minerID) {
        miners.emplace_back(minerID);
    }

    void addBlock(int blockNumber, const std::vector<Transaction>& transactions) {
        Block newBlock(blockNumber);
        newBlock.transactions = transactions;
        for (const auto& tx : transactions) {
            newBlock.totalFees += tx.fee;
        }
        blockchain[blockNumber] = newBlock;
        assignBlockToMiner(newBlock);
    }

    void assignBlockToMiner(Block& block) {
        // Randomly assign block to a miner, favoring those with lower reputation to promote decentralization
        std::shuffle(miners.begin(), miners.end(), std::default_random_engine(static_cast<unsigned int>(time(0))));
        auto& selectedMiner = miners.front();
        selectedMiner.mineBlock(block);
    }

    void displayMinerStatus() {
        for (const auto& miner : miners) {
            std::cout << "Miner ID: " << miner.minerID << ", Earned Fees: " << miner.earnedFees << ", Reputation Score: " << miner.reputationScore << "\n";
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

    // Add a block with low transaction fees to show the effect of base and bonus rewards
    std::vector<Transaction> transactions3 = { {"Ivan", "Judy", 0.5, 0.005} };
    network.addBlock(3, transactions3);

    network.displayMinerStatus();

    return 0;
}
