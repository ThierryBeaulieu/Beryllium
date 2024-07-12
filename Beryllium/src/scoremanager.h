#pragma once

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

class Score
{
public:
    Score(const std::vector<std::string> &content);
    Score(const std::string &id, const std::string &lastName, const std::string &firstName, const std::string &score);

private:
    std::string m_id;
    std::string m_LastName;
    std::string m_FirstName;
    int m_score;
};

static const std::string HIGH_SCORES_FILE_NAME = "scores.txt";

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

    std::list<Score> GetHighScores();

private:
    ScoreManager();
    ~ScoreManager() = default;
    // tbeaulieu 2: todo place in its own util file
    std::vector<std::string> Split(const std::string &s, char delimiter);
    // tbeaulieu end
    std::list<Score> FilterHighestScores(const std::vector<Score> &scores);
};