#include "scoremanager.h"

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
        m_score = number;
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

std::list<Score> ScoreManager::GetHighScores()
{
    std::ifstream file;

    file.open(HIGH_SCORES_FILE_NAME);

    if (!file.is_open())
    {
        std::cerr << "Unable to open high score file" << std::endl;
        const std::list<Score> noScoreFound;
        return noScoreFound;
    }

    std::list<Score> highScores;
    std::string line;

    std::vector<Score> scores;
    while (std::getline(file, line))
    {
        const std::vector<std::string> &content = Split(line, ',');
        scores.push_back(Score(content));
    }

    file.close();

    return highScores;
}
