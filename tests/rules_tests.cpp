#include <iostream>
#include <cassert>
#include <memory>
#include "games/OddEvenRules.hpp"
#include "games/RpsRules.hpp"
#include "games/HumanPlayer.hpp"
#include "games/CpuPlayer.hpp"
#include "games/Match.hpp"

using namespace Starlight;

void testOddEvenRules() {
    std::cout << "[TEST] Testing OddEvenRules..." << std::endl;
    OddEvenRules rulesEven(true); // Player A is Even

    for (int a = 0; a <= 5; ++a) {
        for (int b = 0; b <= 5; ++b) {
            Outcome res = rulesEven.resolve(a, b);
            int sum = a + b;
            if (sum % 2 == 0) {
                assert(res == Outcome::WinA);
            } else {
                assert(res == Outcome::WinB);
            }
        }
    }

    OddEvenRules rulesOdd(false); // Player A is Odd
    for (int a = 0; a <= 5; ++a) {
        for (int b = 0; b <= 5; ++b) {
            Outcome res = rulesOdd.resolve(a, b);
            int sum = a + b;
            if (sum % 2 != 0) {
                assert(res == Outcome::WinA);
            } else {
                assert(res == Outcome::WinB);
            }
        }
    }
    std::cout << "  -> OddEvenRules passed all 72 combinations!" << std::endl;
}

void testRpsRules() {
    std::cout << "[TEST] Testing RpsRules..." << std::endl;
    RpsRules rps;

    // Draws
    assert(rps.resolve(Rock, Rock) == Outcome::Draw);
    assert(rps.resolve(Paper, Paper) == Outcome::Draw);
    assert(rps.resolve(Scissors, Scissors) == Outcome::Draw);

    // Player A wins
    assert(rps.resolve(Rock, Scissors) == Outcome::WinA);
    assert(rps.resolve(Scissors, Paper) == Outcome::WinA);
    assert(rps.resolve(Paper, Rock) == Outcome::WinA);

    // Player B wins
    assert(rps.resolve(Scissors, Rock) == Outcome::WinB);
    assert(rps.resolve(Paper, Scissors) == Outcome::WinB);
    assert(rps.resolve(Rock, Paper) == Outcome::WinB);

    // Pose mappings
    assert(RpsRules::toPoseIndex(Rock) == 0);
    assert(RpsRules::toPoseIndex(Paper) == 5);
    assert(RpsRules::toPoseIndex(Scissors) == 2);

    std::cout << "  -> RpsRules passed all combinations and pose mappings!" << std::endl;
}

void testMatchOddEven() {
    std::cout << "[TEST] Testing Match with Par ou Ímpar (best of 3)..." << std::endl;
    auto p1 = std::make_shared<HumanPlayer>("Alice", 0);
    auto p2 = std::make_shared<CpuPlayer>("Bob", 1);
    auto rules = std::make_shared<OddEvenRules>(true); // Alice is Even

    Match match(p1, p2, rules, 2);
    assert(!match.isOver());
    assert(match.scoreA() == 0 && match.scoreB() == 0);
    assert(match.roundNumber() == 1);

    // Round 1: Alice 2, Bob 2 (sum 4 = even -> Alice wins)
    Outcome r1 = match.playRound(2, 2);
    assert(r1 == Outcome::WinA);
    assert(match.scoreA() == 1 && match.scoreB() == 0);
    assert(match.roundNumber() == 2);
    assert(!match.isOver());

    // Round 2: Alice 1, Bob 2 (sum 3 = odd -> Bob wins)
    Outcome r2 = match.playRound(1, 2);
    assert(r2 == Outcome::WinB);
    assert(match.scoreA() == 1 && match.scoreB() == 1);
    assert(match.roundNumber() == 3);
    assert(!match.isOver());

    // Round 3: Alice 4, Bob 4 (sum 8 = even -> Alice wins match!)
    Outcome r3 = match.playRound(4, 4);
    assert(r3 == Outcome::WinA);
    assert(match.scoreA() == 2 && match.scoreB() == 1);
    assert(match.isOver());
    assert(match.winner() == 0);

    std::cout << "  -> Match Par ou Ímpar passed!" << std::endl;
}

void testMatchRpsDrawRepeat() {
    std::cout << "[TEST] Testing Match with Jokenpô draw repeat rule..." << std::endl;
    auto p1 = std::make_shared<HumanPlayer>("Alice", 0);
    auto p2 = std::make_shared<HumanPlayer>("Bob", 1);
    auto rules = std::make_shared<RpsRules>();

    Match match(p1, p2, rules, 2);

    // Round 1: Both Rock -> Draw. Must NOT count on score and must NOT increment round number!
    Outcome r1 = match.playRound(Rock, Rock);
    assert(r1 == Outcome::Draw);
    assert(match.scoreA() == 0 && match.scoreB() == 0);
    assert(match.roundNumber() == 1);
    assert(!match.isOver());

    // Round 1 retry: Rock vs Scissors -> Alice wins
    Outcome r1b = match.playRound(Rock, Scissors);
    assert(r1b == Outcome::WinA);
    assert(match.scoreA() == 1 && match.scoreB() == 0);
    assert(match.roundNumber() == 2);
    assert(!match.isOver());

    // Round 2: Rock vs Paper -> Bob wins
    Outcome r2 = match.playRound(Rock, Paper);
    assert(r2 == Outcome::WinB);
    assert(match.scoreA() == 1 && match.scoreB() == 1);
    assert(match.roundNumber() == 3);

    // Another draw on round 3
    Outcome r3 = match.playRound(Paper, Paper);
    assert(r3 == Outcome::Draw);
    assert(match.scoreA() == 1 && match.scoreB() == 1);
    assert(match.roundNumber() == 3);
    assert(!match.isOver());

    // Round 3 resolution: Scissors vs Paper -> Alice wins match 2-1!
    Outcome r3b = match.playRound(Scissors, Paper);
    assert(r3b == Outcome::WinA);
    assert(match.scoreA() == 2 && match.scoreB() == 1);
    assert(match.isOver());
    assert(match.winner() == 0);

    std::cout << "  -> Match Jokenpô draw-repeat passed!" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Running Starlight Games Rule Tests..." << std::endl;
    std::cout << "========================================" << std::endl;

    testOddEvenRules();
    testRpsRules();
    testMatchOddEven();
    testMatchRpsDrawRepeat();

    std::cout << "========================================" << std::endl;
    std::cout << "ALL RULE TESTS PASSED SUCCESSFULLY!" << std::endl;
    std::cout << "========================================" << std::endl;
    return 0;
}
