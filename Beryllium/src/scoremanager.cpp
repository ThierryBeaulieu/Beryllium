#include "scoremanager.h"

Score::Score()
{
    Score("---", "---", "---", "0");
}

Score::Score(const std::vector<std::string> &content)
{
    // Files contains oldest to newest score
    //
    // takes the most recent score
    //
    // score format : id,score,LastName,FirstName,jjmmyyyy
    if (content.size() != 5)
    {
        std::cerr << "Error the high score file is unreadable" << std::endl;
    }

    Score(content[0], content[1], content[2], content[3]);
}

Score::Score(const std::string &id, const std::string &lastName, const std::string &firstName, const std::string &score)
    : m_id(id),
      m_LastName(lastName),
      m_FirstName(firstName)
{
    try
    {
        int number = std::stoi(score);
        m_value = number;
        std::cout << "The number is: " << number << std::endl;
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

bool Score::operator<(const Score &score) const
{
    return score.m_value < m_value;
}

ScoreManager::ScoreManager()
{
    // Todo: create a high score file if it doesn't exists.
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

}

std::vector<Score> ScoreManager::GetHighScores()
{
    std::ifstream file;

    file.open(HIGH_SCORES_FILE_NAME);
    std::vector<Score> scores;

    if (scores.size() < 3)
    {
        for (int i = 0; i < 3 - scores.size(); ++i)
        {
            scores.push_back(Score());
        }
    }

    if (!file.is_open())
    {
        std::cerr << "Unable to open high score file" << std::endl;
        return scores;
    }

    std::string line;
    while (std::getline(file, line))
    {
        const std::vector<std::string> &content = Split(line, ',');
        scores.push_back(Score(content));
    }
    file.close();

    std::sort(scores.begin(), scores.end(), [](const Score &a, const Score &b)
              { return a < b; });

    std::vector<Score> highestScores(scores.begin(), scores.begin() + 2);
    return highestScores;
}
