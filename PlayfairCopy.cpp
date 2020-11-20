
// Standard library includes
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

// Out project headers
#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher(const std::string &key)
    : key_{""}
{
  setKey(key);
}

void PlayfairCipher::setKey(const std::string &key)
{
  // store the original key
  key_ = key;
  // Append the alphabet
  key_ += alphabet;
  // Make sure the key is upper case
  std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);
  // Remove non-alpha characters
  key_.erase(std::remove_if(key_.begin(), key_.end(),
                            [](char character) { return !isalpha(character); }),
             key_.end());
  // Change J -> I
  std::transform(key_.begin(), key_.end(), key_.begin(),
                 [](char val) {if (val=='J') return 'I'; else return val; });
  // Remove duplicated letters
  std::string lettersFound{""};
  auto encounteredChars = [&](char c) {
    if (lettersFound.find(c) == std::string::npos)
    {
      lettersFound += c;
      return false;
    }
    else
    {
      return true;
    }
  };
  key_.erase(std::remove_if(key_.begin(), key_.end(), encounteredChars), key_.end());
  // Store the coords of each letter
  using Letter2CoordMap = std::map<std::string, std::pair<int, int>>;
  Letter2CoordMap coordMap;
  int n{0};
  for (std::string::size_type i = 0; i < key_.size(); ++i, ++n)
  {
    int xCoord{0};
    if (n <= 4)
      xCoord = 0;
    else if (n > 4 && n <= 9)
      xCoord = 1;
    else if (n > 9 && n <= 14)
      xCoord = 2;
    else if (n > 14 && n <= 19)
      xCoord = 3;
    else if (n > 19 && n <= 24)
      xCoord = 4;
    int yCoord{n % 5};
    auto coord = std::make_pair(xCoord, yCoord);
    //std::cout << "xCoord = "<< xCoord << " yCoord = " << yCoord << " coord = " << coord.first << "," << coord.second << std::endl;
    std::string character{key_[i]};
    coordMap[character] = coord;
  };
  for (auto p : coordMap)
  {
    std::cout << p.first << ": " << p.second.first << "," << p.second.second << std::endl;
  }
  // Store the playfair cihper key map
  using Coord2LetterMap = std::map<std::pair<int, int>, std::string>;
  Coord2LetterMap letterMap;
  int m{0};
  for (std::string::size_type i = 0; i < key_.size(); ++i, ++m)
  {
    int xCoord{0};
    if (m <= 4)
      xCoord = 0;
    else if (m > 4 && m <= 9)
      xCoord = 1;
    else if (m > 9 && m <= 14)
      xCoord = 2;
    else if (m > 14 && m <= 19)
      xCoord = 3;
    else if (m > 19 && m <= 24)
      xCoord = 4;
    int yCoord{m % 5};
    auto coord = std::make_pair(xCoord, yCoord);
    std::string character{key_[i]};
    letterMap[coord] = character;
  };
  for (auto q : letterMap)
  {
    std::cout << q.first.first << "," << q.first.second << ": " << q.second << std::endl;
  }
  // Print the square
  std::string row{""};
  for (auto q : letterMap)
  {
    row.append(q.second + " ");
    if (row.size() == 10)
    {
      std::cout << row << std::endl;
      row = "";
    }
  }
}

std::string PlayfairCipher::applyCipher(const std::string &inputText, const CipherMode /*cipherMode*/) const
{
  std::string input{""};
  input += inputText;
  // ChangeJ→I
  std::transform(input.begin(), input.end(), input.begin(),
                 [](char val) {if (val=='J') return 'I'; else return val; });
  // If repeated chars in a digraph add an X or Q if XX
  std::string thoughts{"Here loop through the string using something like the lambda to change j->i algorithm used above\
  to make the necessary change. might need a loop to work with the letter pairs\
  What i tried doesnt take into account the digraphs and also doesnt work.\
  Maybe have a loop that takes each character !%2 and the next character and compares them."};
  /*for (std::string::size_type i = 0; i < input.size(); ++i)
  {
    char val2 {input.at(i-1)};
    std::transform(input.begin(), input.end(), input.begin(),[](char val, char val2) {if (val==val2) return 'X'; else return val; });
  }
  std::transform(input.begin(), input.end(), input.begin(),
                 [](char val) {char val2 {val -1}; 
                 if (val==val2) return 'X'; else return val; });
  std::transform(input.begin(), input.end(), input.begin(),
                 [](char val) {char val2 {'X'}; if (val==val2) return 'Q'; else return val; });*/

  // if the size of input is odd, add a trailing Z
  if (input.size() % 2 != 0)
  {
    input += 'Z';
  }
  // Loop over the input in Digraphs
  std::string thoughts3{"Have a loop for the next few tasks. Have it loop through the input and append every two letters\
  to a vector\
  Currently this jsut makes a vector of digraphs, perhpas needs to change to loop over each i and i+1 as a pair"};
  std::vector<std::pair<std::string, std::string>> digraphs {};
  int index{0};
  for (std::string::size_type i = 0; i < input.size(); ++i, ++index)
  {
    std::string diFirst {""};
    std::string diSecond {""};
    if (index == 0 || (index % 2 == 0 && i < (input.size() -1)))
    {
      diFirst += input.at(index);
      diSecond += input.at(index + 1);
      auto Pair = std::make_pair(diFirst, diSecond);
      digraphs.push_back(Pair);
    }
  }
  for (std::string::size_type i = 0; i < digraphs.size(); ++i)
    {
      std::cout << digraphs.at(i).first << digraphs.at(i).second << std::endl;
    }

  // - Find the coords in the grid for each digraph
  std::string thoughts4{"find the coords in coordsMap for each letter pair in the above vector and fill another \
  empty vector with smaller vectors containing two coord pairs in each"};

  // - Apply the rules to these coords to get 'new' coords
  std::string thoughts5{"Have a switch which checks for the conditions. One conditions tests for if they are on the same row\
  and then assigns the letter to the one to the left. One tests for if the same column and shifts one row down. The final one \
  Assumes both of the above is wrong and then shifts the letters to the required positions. Example switch commented out below\
  The loop only ads one to the coords for the first two cases or the required difference in the final case. These new\
  Coords are then put into a new vector.\
  Also i think end the loop here."};
  //Have a loop which loops through each vector of letters and thus each coord vector pair, this coord pair is then put into the switch
  /*switch(coord){
      case(coord[0].first == coord[1].first):
        //Assign to letter to the right
        break;
      case(coord[0].second == coord[1].second):
        //Assign to letter below
        break;
      default:
        //Find difference in .first. Check the rules for which one moves which way 
        break;
    }*/

  // - Find the letter associated with the new coords
  std::string thoughts6{"I think here take the vector with the new coords and loop through the coordMap to find the associated letters\
  I think make a new set of vectors with the new letter pairs, as above but with the new letters for each coord.\
  So have a loop through the new coord vector which find the associated letter in coordMap and appends this to the new vector"};

  // return the text
  return key_ + "__" + input;
}
