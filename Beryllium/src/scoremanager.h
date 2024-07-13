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
    Score(const std::string &lastName, const std::string &firstName, int score);
    Score(const std::string &lastName, const std::string &firstName, const std::string &score);

    bool operator<(const Score &score) const;

    const std::string &GetLastName() const;
    const std::string &GetFirstName() const;
    int GetValue() const;

private:
    std::string m_LastName;
    std::string m_FirstName;
    int m_Value;
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
    void AddScore(const Score &score);

private:
    ScoreManager();
    ~ScoreManager() = default;
    // tbeaulieu 2: todo place in its own util file
    std::vector<std::string> Split(const std::string &s, char delimiter);
    // tbeaulieu end
};