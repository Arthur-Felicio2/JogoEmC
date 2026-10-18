#pragma once
// ScoreStore.h — Persistência e ranking para os 3 jogos da Starlight Company
// Apenas biblioteca padrão C++17

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <cctype>

namespace sl {

enum class GameId {
    OddEven,
    Rps,
    Flappy
};

struct PlayerProfile {
    std::string name;

    // Par ou Ímpar
    int oddEvenWins = 0;
    int oddEvenLosses = 0;
    int oddEvenRoundsWon = 0;
    int oddEvenRoundsLost = 0;

    // Jokenpô
    int rpsWins = 0;
    int rpsLosses = 0;
    int rpsRoundsWon = 0;
    int rpsRoundsLost = 0;

    // Flappy
    int flappyHighScore = 0;
    int flappyAttempts = 0;

    static constexpr int POINTS_PER_WIN = 10;

    int totalPoints() const {
        return (oddEvenWins + rpsWins) * POINTS_PER_WIN + flappyHighScore;
    }

    int totalMatchesWon() const {
        return oddEvenWins + rpsWins;
    }
};

class ScoreStore {
public:
    explicit ScoreStore(std::string filePath = "scores.txt")
        : m_filePath(std::move(filePath)) {}

    const std::string& filePath() const { return m_filePath; }
    void setFilePath(const std::string& path) { m_filePath = path; }

    static bool iequals(const std::string& a, const std::string& b) {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); ++i) {
            if (std::tolower(static_cast<unsigned char>(a[i])) !=
                std::tolower(static_cast<unsigned char>(b[i]))) {
                return false;
            }
        }
        return true;
    }

    const PlayerProfile* find(const std::string& name) const {
        for (const auto& p : m_profiles) {
            if (iequals(p.name, name)) {
                return &p;
            }
        }
        return nullptr;
    }

    PlayerProfile* findOrCreate(const std::string& name) {
        for (auto& p : m_profiles) {
            if (iequals(p.name, name)) {
                return &p;
            }
        }
        // Novo perfil: guarda a grafia da primeira vez
        m_profiles.push_back(PlayerProfile{name});
        return &m_profiles.back();
    }

    void recordMatch(const std::string& name, GameId game, bool won, int roundsWon, int roundsLost) {
        if (name.empty()) return;
        PlayerProfile* prof = findOrCreate(name);

        if (game == GameId::OddEven) {
            if (won) prof->oddEvenWins++;
            else prof->oddEvenLosses++;
            prof->oddEvenRoundsWon += roundsWon;
            prof->oddEvenRoundsLost += roundsLost;
        } else if (game == GameId::Rps) {
            if (won) prof->rpsWins++;
            else prof->rpsLosses++;
            prof->rpsRoundsWon += roundsWon;
            prof->rpsRoundsLost += roundsLost;
        }
    }

    bool recordFlappy(const std::string& name, int score) {
        if (name.empty()) return false;
        PlayerProfile* prof = findOrCreate(name);
        prof->flappyAttempts++;

        if (score > prof->flappyHighScore) {
            prof->flappyHighScore = score;
            return true; // Novo recorde pessoal!
        }
        return false;
    }

    std::vector<PlayerProfile> ranking(GameId game) const {
        std::vector<PlayerProfile> list;
        for (const auto& p : m_profiles) {
            if (game == GameId::OddEven) {
                if ((p.oddEvenWins + p.oddEvenLosses) > 0) list.push_back(p);
            } else if (game == GameId::Rps) {
                if ((p.rpsWins + p.rpsLosses) > 0) list.push_back(p);
            } else if (game == GameId::Flappy) {
                if (p.flappyHighScore > 0 || p.flappyAttempts > 0) list.push_back(p);
            }
        }

        std::sort(list.begin(), list.end(), [game](const PlayerProfile& a, const PlayerProfile& b) {
            if (game == GameId::OddEven) {
                if (a.oddEvenWins != b.oddEvenWins) return a.oddEvenWins > b.oddEvenWins;
                int diffA = a.oddEvenRoundsWon - a.oddEvenRoundsLost;
                int diffB = b.oddEvenRoundsWon - b.oddEvenRoundsLost;
                if (diffA != diffB) return diffA > diffB;
                return (a.oddEvenWins + a.oddEvenLosses) > (b.oddEvenWins + b.oddEvenLosses);
            } else if (game == GameId::Rps) {
                if (a.rpsWins != b.rpsWins) return a.rpsWins > b.rpsWins;
                int diffA = a.rpsRoundsWon - a.rpsRoundsLost;
                int diffB = b.rpsRoundsWon - b.rpsRoundsLost;
                if (diffA != diffB) return diffA > diffB;
                return (a.rpsWins + a.rpsLosses) > (b.rpsWins + b.rpsLosses);
            } else {
                if (a.flappyHighScore != b.flappyHighScore) return a.flappyHighScore > b.flappyHighScore;
                return a.flappyAttempts > b.flappyAttempts;
            }
        });

        return list;
    }

    std::vector<PlayerProfile> overallRanking() const {
        std::vector<PlayerProfile> list = m_profiles;
        std::sort(list.begin(), list.end(), [](const PlayerProfile& a, const PlayerProfile& b) {
            int ptsA = a.totalPoints();
            int ptsB = b.totalPoints();
            if (ptsA != ptsB) return ptsA > ptsB;
            if (a.totalMatchesWon() != b.totalMatchesWon()) return a.totalMatchesWon() > b.totalMatchesWon();
            if (a.flappyHighScore != b.flappyHighScore) return a.flappyHighScore > b.flappyHighScore;
            return a.name < b.name;
        });
        return list;
    }

    const std::vector<PlayerProfile>& all() const { return m_profiles; }
    std::vector<PlayerProfile>& all() { return m_profiles; }

    bool load() {
        m_profiles.clear();
        std::ifstream file(m_filePath);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            std::stringstream ss(line);
            std::string item;
            std::vector<std::string> tokens;
            while (std::getline(ss, item, '|')) {
                tokens.push_back(item);
            }

            if (tokens.size() >= 11) {
                try {
                    PlayerProfile p;
                    p.name = tokens[0];
                    p.oddEvenWins = std::stoi(tokens[1]);
                    p.oddEvenLosses = std::stoi(tokens[2]);
                    p.oddEvenRoundsWon = std::stoi(tokens[3]);
                    p.oddEvenRoundsLost = std::stoi(tokens[4]);

                    p.rpsWins = std::stoi(tokens[5]);
                    p.rpsLosses = std::stoi(tokens[6]);
                    p.rpsRoundsWon = std::stoi(tokens[7]);
                    p.rpsRoundsLost = std::stoi(tokens[8]);

                    p.flappyHighScore = std::stoi(tokens[9]);
                    p.flappyAttempts = std::stoi(tokens[10]);

                    if (!p.name.empty()) {
                        m_profiles.push_back(p);
                    }
                } catch (...) {
                    // Linha corrompida ou inválida: ignora e prossegue sem travar
                }
            }
        }
        return true;
    }

    bool save() const {
        std::string tempPath = m_filePath + ".tmp";
        {
            std::ofstream file(tempPath);
            if (!file.is_open()) {
                return false;
            }

            file << "# Starlight Company - Scores\n";
            file << "# Name|OE_W|OE_L|OE_RW|OE_RL|RPS_W|RPS_L|RPS_RW|RPS_RL|FLAPPY_HIGH|FLAPPY_ATT\n";

            for (const auto& p : m_profiles) {
                file << p.name << "|"
                     << p.oddEvenWins << "|"
                     << p.oddEvenLosses << "|"
                     << p.oddEvenRoundsWon << "|"
                     << p.oddEvenRoundsLost << "|"
                     << p.rpsWins << "|"
                     << p.rpsLosses << "|"
                     << p.rpsRoundsWon << "|"
                     << p.rpsRoundsLost << "|"
                     << p.flappyHighScore << "|"
                     << p.flappyAttempts << "\n";
            }
            file.flush();
        }

        std::error_code ec;
        std::filesystem::remove(m_filePath, ec);
        std::filesystem::rename(tempPath, m_filePath, ec);
        if (ec) {
            // Fallback caso rename falhe entre volumes ou travamentos
            std::filesystem::copy_file(tempPath, m_filePath, std::filesystem::copy_options::overwrite_existing, ec);
            std::filesystem::remove(tempPath, ec);
        }

        return true;
    }

    void clear() {
        m_profiles.clear();
    }

private:
    std::string m_filePath;
    std::vector<PlayerProfile> m_profiles;
};

} // namespace sl
