#include "scoremanager.h"

Score::Score(const std::vector<std::string> &content)
{
    if (content.size() != 4)
    {
        std::cerr << "Error the high score file is unreadable" << std::endl;
    }

    *this = Score(content[0], content[1], content[2], content[3]);
}

Score::Score(const std::string &id, const std::string &lastName, const std::string &firstName, const std::string &score)
    : m_Id(id),
      m_LastName(lastName),
      m_FirstName(firstName)
{
    try
    {
        int number = std::stoi(score);
        m_Value = number;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Out of range: " << e.what() << std::endl;
    }
}

const std::string &Score::GetID() const
{
    return m_Id;
}

const std::string &Score::GetLastName() const
{
    return m_LastName;
}

const std::string &Score::GetFirstName() const
{
    return m_FirstName;
}

int Score::GetValue() const
{
    return m_Value;
}

bool Score::operator<(const Score &score) const
{
    return score.m_Value < m_Value;
}

ScoreManager::ScoreManager()
{
    std::ifstream file(HIGH_SCORES_FILE_NAME);

    if (file.good())
    {
        file.close();
    }
    else
    {
        file.close();
        std::cout << "File '" << HIGH_SCORES_FILE_NAME << "' does not exist. Creating it..." << std::endl;
        std::ofstream newFile(HIGH_SCORES_FILE_NAME);
        if (newFile.is_open())
        {
            newFile.close();
        }
        else
        {
            std::cerr << "Failed to create file '" << HIGH_SCORES_FILE_NAME << "'." << std::endl;
        }
    }
}

std::vector<std::string> ScoreManager::Split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

void ScoreManager::AddScore(const Score &score)
{
    std::ofstream file(HIGH_SCORES_FILE_NAME, std::ios::app);

    if (!file.is_open())
    {
        std::cerr << "Unable to open high score file for appending" << std::endl;
        return;
    }

    file << score.GetID() << "," << score.GetValue() << "," << score.GetLastName() << "," << score.GetFirstName() << std::endl;

    file.close();
}

std::vector<Score> ScoreManager::GetHighScores()
{
    std::ifstream file;

    file.open(HIGH_SCORES_FILE_NAME);

    if (!file.is_open())
    {
        std::cerr << "Unable to open high score file" << std::endl;
        std::vector<Score> emptyScores;
        return emptyScores;
    }

    std::vector<Score> scores;
    std::string line;
    while (std::getline(file, line))
    {
        const std::vector<std::string> &content = Split(line, ',');
        scores.push_back(Score(content));
    }
    file.close();

    std::sort(scores.begin(), scores.end(), [](const Score &a, const Score &b)
              { return a < b; });

    return scores;
}
