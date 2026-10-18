#include <iostream>
#include <cassert>
#include <filesystem>
#include "save/ScoreStore.h"

int main() {
    std::cout << "[TEST] Running ScoreStore_test..." << std::endl;
    std::string testPath = "test_scores_temp.txt";

    // Clean up any old leftover file
    std::filesystem::remove(testPath);

    {
        sl::ScoreStore store(testPath);
        assert(!store.load()); // Non-existent file should return false cleanly without crashing
        assert(store.all().empty());

        // Test 1: Record matches
        store.recordMatch("Arthur", sl::GameId::OddEven, true, 2, 1);
        store.recordMatch("Gabriel", sl::GameId::OddEven, false, 1, 2);

        store.recordMatch("Arthur", sl::GameId::Rps, true, 2, 0);
        store.recordMatch("arthur", sl::GameId::Rps, true, 2, 1); // Case-insensitive match!

        // Test 2: Profile count should be 2, not 3 (Arthur merged)
        assert(store.all().size() == 2);
        const auto* art = store.find("ARTHUR");
        assert(art != nullptr);
        assert(art->name == "Arthur"); // Preserved original casing
        assert(art->oddEvenWins == 1);
        assert(art->rpsWins == 2);
        assert(art->totalMatchesWon() == 3);

        // Test 3: Flappy records
        bool rec1 = store.recordFlappy("Arthur", 12);
        assert(rec1 == true); // New record
        bool rec2 = store.recordFlappy("Arthur", 8);
        assert(rec2 == false); // Not a new record
        bool rec3 = store.recordFlappy("Gabriel", 25);
        assert(rec3 == true);

        // Test 4: Overall score calculation
        // Arthur: (1 OE + 2 RPS) * 10 + 12 = 42
        // Gabriel: (0 OE + 0 RPS) * 10 + 25 = 25
        assert(art->totalPoints() == 42);
        const auto* gab = store.find("gabriel");
        assert(gab != nullptr);
        assert(gab->totalPoints() == 25);

        // Overall ranking: Arthur #1, Gabriel #2
        auto overall = store.overallRanking();
        assert(overall.size() == 2);
        assert(overall[0].name == "Arthur");
        assert(overall[1].name == "Gabriel");

        // Flappy ranking: Gabriel #1 (25), Arthur #2 (12)
        auto flappyRank = store.ranking(sl::GameId::Flappy);
        assert(flappyRank.size() == 2);
        assert(flappyRank[0].name == "Gabriel");
        assert(flappyRank[1].name == "Arthur");

        // Save to file
        assert(store.save() == true);
        assert(std::filesystem::exists(testPath));
    }

    // Test 5: Reload from file
    {
        sl::ScoreStore store2(testPath);
        assert(store2.load() == true);
        assert(store2.all().size() == 2);

        const auto* art = store2.find("Arthur");
        assert(art != nullptr);
        assert(art->oddEvenWins == 1);
        assert(art->rpsWins == 2);
        assert(art->flappyHighScore == 12);
        assert(art->totalPoints() == 42);

        const auto* gab = store2.find("Gabriel");
        assert(gab != nullptr);
        assert(gab->flappyHighScore == 25);
    }

    // Test 6: Tolerant loading (corrupt lines test)
    {
        std::ofstream appendFile(testPath, std::ios::app);
        appendFile << "CorruptLine|1|2|Invalid\n";
        appendFile << "\n";
        appendFile << "# Comment line\n";
        appendFile << "ValidUser|1|0|2|0|0|0|0|0|5|1\n";
        appendFile.close();

        sl::ScoreStore store3(testPath);
        assert(store3.load() == true);
        assert(store3.all().size() == 3); // 2 original + 1 valid line, corrupt ignored
        assert(store3.find("ValidUser") != nullptr);
    }

    // Clean up
    std::filesystem::remove(testPath);
    std::filesystem::remove(testPath + ".tmp");

    std::cout << ">> ScoreStore_test PASSED successfully!\n" << std::endl;
    return 0;
}
