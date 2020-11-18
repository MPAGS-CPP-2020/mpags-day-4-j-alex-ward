
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
  int n2{0};
  for (std::string::size_type i = 0; i < key_.size(); ++i, ++n2)
  {
    int xCoord{0};
    if (n2 <= 4)
      xCoord = 0;
    else if (n2 > 4 && n2 <= 9)
      xCoord = 1;
    else if (n2 > 9 && n2 <= 14)
      xCoord = 2;
    else if (n2 > 14 && n2 <= 19)
      xCoord = 3;
    else if (n2 > 19 && n2 <= 24)
      xCoord = 4;
    int yCoord{n2 % 5};
    auto coord = std::make_pair(xCoord, yCoord);
    std::string character{key_[i]};
    letterMap[coord] = character;
  };
  for (auto q : letterMap)
  {
    std::cout << q.first << ": " << q.second.first << "," << q.second.second << std::endl;
  }
}

std::string PlayfairCipher::applyCipher(const std::string &inputText, const CipherMode /*cipherMode*/) const
{
  // ChangeJ→I
  // If repeated chars in a digraph add an X or Q if XX
  // if the size of input is odd, add a trailing Z
  // Loop over the input in Digraphs
  // - Find the coords in the grid for each digraph
  // - Apply the rules to these coords to get 'new' coords
  // - Find the letter associated with the new coords
  // return the text
  //  return inputText;
  return key_ + "____" + inputText;
}
