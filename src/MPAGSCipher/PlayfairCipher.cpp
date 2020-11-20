
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
    // Store the playfair cihper key map
    letterMap[coord] = character;
  };
  for (auto p : coordMap)
  {
    std::cout << p.first << ": " << p.second.first << "," << p.second.second << std::endl;
  }
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
  std::string digraph{""};
  std::string cipher{""};
  std::string playfairCipher{""};
  input += inputText;
  // ChangeJ→I
  std::transform(input.begin(), input.end(), input.begin(),
                 [](char val) {if (val=='J') return 'I'; else return val; });
  // If repeated chars in a digraph add an X or Q if XX
  for (std::string::size_type i = 0, j = 1; j < input.size(); ++i, ++j, ++i, ++j)
  {
    if (input.at(i) == 'X' && input.at(i) == input.at(j))
    {
      input.replace(j, 1, "Q");
    }
    if (input.at(i) == input.at(j))
    {
      input.replace(j, 1, "X");
    }
  }
  // if the size of input is odd, add a trailing Z
  if (input.size() % 2 != 0)
  {
    input += 'Z';
  }
  // Loop over the input in Digraphs
  for (std::string::size_type i = 0; i < input.size(); ++i, ++i)
  {
    std::cout << "Digraph = " << input.at(i) << input.at(i + 1) << std::endl;
    // - Find the coords in the grid for each digraph
    /// --- Not sure im using .find() correctly here, doesnt seem like its needed in the below code.
    std::string character{input.at(i)};
    std::string character2{input.at(i + 1)};
    std::string newChar1 {""};
    std::string newChar2 {""};
    if (coordMap.find(character) != coordMap.end())
    {
      std::cout << "Coords (" << character << ") = " << coordMap.at(character).first << ":" << coordMap.at(character).second << std::endl;
      std::cout << "Coords (" << character2 << ") = " << coordMap.at(character2).first << ":" << coordMap.at(character2).second << std::endl;
      auto coordinates = std::make_pair(coordMap.at(character).first, coordMap.at(character).second);
      auto coordinates2 = std::make_pair(coordMap.at(character2).first, coordMap.at(character2).second);

      // - Apply the rules to these coords to get 'new' coords
      if (coordinates.first == coordinates2.first)
      {
        coordinates = std::make_pair(coordMap.at(character).first, coordMap.at(character).second +1);
        coordinates2 = std::make_pair(coordMap.at(character2).first, coordMap.at(character2).second +1);
      }
      else if (coordinates.second == coordinates2.second)
      {
        coordinates = std::make_pair(coordMap.at(character).first+1, coordMap.at(character).second);
        coordinates2 = std::make_pair(coordMap.at(character2).first+1, coordMap.at(character2).second);
      }
      else
      {
        int yDiff {coordinates.second - coordinates2.second};
        coordinates = std::make_pair(coordMap.at(character).first, coordMap.at(character).second-yDiff);
        coordinates2 = std::make_pair(coordMap.at(character2).first, coordMap.at(character2).second+yDiff);
      }
      std::cout << "New Coords (" << character << ") = " << coordinates.first << ":" << coordinates.second << std::endl;
      std::cout << "New Coords (" << character2 << ") = " << coordinates2.first << ":" << coordinates2.second << std::endl;
    
      // - Find the letter associated with the new coords
      newChar1 = letterMap.at(coordinates);
      newChar2 = letterMap.at(coordinates2);
      digraph += character + character2 + " ";
      cipher += newChar1 + newChar2 + " ";
      playfairCipher += newChar1 + newChar2;
    }
  } //end of loop
  // return the text
  std::cout << "Input = " << inputText << "  " << "Transformed Input = " << input << std::endl;
  std::cout << "Key = " << key_ << std::endl;
  std::cout << "Digraph = " << digraph << "  " << "Transformed Digraph = " << cipher << std::endl;
  return "PlayfairCipher = " + playfairCipher;
}
