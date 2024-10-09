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

// Representation of a participant (Miner/Stakeholder)
class Participant {
public:
    std::string participantID;
    double earnedFees;
    double stakeAmount; // Stake owned by the participant

    Participant(const std::string& id, double stake) : participantID(id), earnedFees(0.0), stakeAmount(stake) {}

    void mineBlock(Block& block) {
        earnedFees += block.totalFees;
        std::cout << "Participant " << participantID << " mined block " << block.blockNumber << " and earned fees: " << earnedFees << "\n";
    }

    double getStakeWeight() const {
        return stakeAmount;
    }
};

// Hybrid Proof of Work and Proof of Stake Network class
class HybridNetwork {
private:
    std::vector<Participant> participants;
    std::map<int, Block> blockchain;
    double baseFeeRate;

public:
    HybridNetwork() : baseFeeRate(0.01) {} // Initial base fee rate

    void addParticipant(const std::string& participantID, double stakeAmount) {
        participants.emplace_back(participantID, stakeAmount);
    }

    void addBlock(int blockNumber, std::vector<Transaction>& transactions) {
        adjustFees(transactions);
        Block newBlock(blockNumber);
        newBlock.transactions = transactions;
        for (const auto& tx : transactions) {
            newBlock.totalFees += tx.fee;
        }
        blockchain[blockNumber] = newBlock;
        assignBlockToParticipant(newBlock);
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

    void assignBlockToParticipant(Block& block) {
        // Assign block to a participant based on a combination of random selection and stake weight
        double totalStake = 0.0;
        for (const auto& participant : participants) {
            totalStake += participant.getStakeWeight();
        }

        // Generate a random number between 0 and totalStake
        double randomPoint = static_cast<double>(rand()) / RAND_MAX * totalStake;

        double cumulativeStake = 0.0;
        for (auto& participant : participants) {
            cumulativeStake += participant.getStakeWeight();
            if (cumulativeStake >= randomPoint) {
                participant.mineBlock(block);
                break;
            }
        }
    }

    void displayParticipantStatus() {
        for (const auto& participant : participants) {
            std::cout << "Participant ID: " << participant.participantID << ", Earned Fees: " << participant.earnedFees << ", Stake Amount: " << participant.stakeAmount << "\n";
        }
    }
};

int main() {
    HybridNetwork network;
    network.addParticipant("ParticipantA", 50.0);
    network.addParticipant("ParticipantB", 30.0);
    network.addParticipant("ParticipantC", 20.0);

    // Add some blocks with transactions
    std::vector<Transaction> transactions1 = { {"Alice", "Bob", 1.0, 0.01}, {"Charlie", "Dave", 2.0, 0.015} };
    network.addBlock(1, transactions1);

    std::vector<Transaction> transactions2 = { {"Eve", "Frank", 1.5, 0.02}, {"Grace", "Heidi", 3.0, 0.03} };
    network.addBlock(2, transactions2);

    // Add a block with low transaction fees to show the effect of dynamic fee adjustment and PoS integration
    std::vector<Transaction> transactions3 = { {"Ivan", "Judy", 0.5, 0.005} };
    network.addBlock(3, transactions3);

    network.displayParticipantStatus();

    return 0;
}
