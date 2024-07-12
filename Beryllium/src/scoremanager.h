#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

static const std::string HIGH_SCORES_FILE_NAME = "scores.txt";

class Score
{
public:
    Score(const std::vector<std::string> &content);
    Score(const std::string &id, const std::string &lastName, const std::string &firstName, const std::string &score);

    bool operator<(const Score &score) const;

private:
    std::string m_id;
    std::string m_LastName;
    std::string m_FirstName;
    int m_value;
};

class ScoreManager
{
public:
    static ScoreManager &GetInstance()
    {
        static ScoreManager m_instance;
        return m_instance;
    }

    ScoreManager(const ScoreManager &) = delete;
    ScoreManager &operator=(const ScoreManager &) = delete;

    std::vector<Score> GetHighScores();

private:
    ScoreManager();
    ~ScoreManager() = default;
    // tbeaulieu 2: todo place in its own util file
    std::vector<std::string> Split(const std::string &s, char delimiter);
    // tbeaulieu end
};