#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sys/time.h>

using namespace std;

template<typename T1, typename T2, typename T3>
struct triple
{
  T1 id1;
  T2 id2;
  T3 ed;
};

class simJoin 
{
public:
	simJoin(const string &filename) 
  {
    readData(filename);
  };

	~simJoin  () {};

  bool SimilarityJoin(unsigned ed_threshold, vector< triple<unsigned, unsigned, unsigned> > &results);
  bool getString(int id, string &out) const;
  int getDataNum() const;
  
private:
  vector<string> data;
  vector< tuple<unsigned, unsigned> > filteredData, partFilteredData;
  bool readData(const string &filename);
  bool lengthFilter(unsigned ed_threshold);
  tuple<vector<string>, vector<unsigned>> rpartition(string s, unsigned tau);
  bool spartition(std::string s, std::string sub, int t, int start, int i, int delta);
  int minDistance(std::string str1, std::string str2);
};
