#include "simJoin.h"
#include <cmath>
#include <tuple>
using namespace std;

int simJoin::getDataNum() const
{
  return data.size();
}

bool simJoin::getString(int id, string &out) const
{
  if (id < 0 || id >= data.size())
    return false;
  out = data[id];
  return true;
}

bool simJoin::readData(const string &filename)
{
  string str;
  ifstream datafile(filename, ios::in);
  while (getline(datafile, str))
    data.emplace_back(str);

  return true;
}

 /*
 * It should do a similarity join operation betweent the set of strings from a data file
 * such that the edit distance between two string is not larger than the given threshold. The 
 * format of result is a triple of numbers which respectively stand for the two IDs of the pair of strings
 * from the data file and the edit distance between the two strings. All results are stored in a vector, sorted based on the IDs of the string from
 * the first file and then the IDs of the string from the second file in an ascending order. Return 
 * an error if the similarity join operation is failed.
 */


bool simJoin::SimilarityJoin(unsigned threshold, vector< triple<unsigned, unsigned, unsigned> > &results) 
{
  
  lengthFilter(threshold);
  
  cout << "Filtered Data Length after L Filter " << filteredData.size() << endl;

  for( auto i: filteredData)
  {
    
    int indr = get<0>(i), inds = get<1>(i);
    string r = data[indr], s = data[inds];

    tuple<vector<string>, vector<unsigned>> rPartsRes = rpartition(r,threshold);
    vector<string> rParts = get<0>(rPartsRes);
    vector<unsigned> rIndices = get<1>(rPartsRes);
    unsigned delta = abs((int)s.length() - (int)r.length());
    for (int i=0; i<rParts.size(); i++)
    {
      bool isPair = spartition(s, rParts[i], threshold, rIndices[i], i+1, delta);
      if(isPair)
      {
        partFilteredData.push_back(make_tuple(indr, inds));
        break;
      }
    }

  }
  
  cout << "Filtered Data Length after P Filter " << partFilteredData.size() << endl;

  for( auto i: partFilteredData)
  {
    
    int indr = get<0>(i), inds = get<1>(i);
    string r = data[indr], s = data[inds];
    
    int editDist = minDistance(r, s);

    if(editDist <= threshold)
    {
      triple<unsigned, unsigned, unsigned> triples = {(unsigned)indr, (unsigned)inds, (unsigned)editDist};
      results.push_back(triples);
    }
     
  }

	return true;
}

bool simJoin::lengthFilter(unsigned threshold)
{
  int size = data.size();
  cout << size;
  for (int i=0; i<size; i++){
    for(int j=i+1; j<size && data[j].size() - data[i].size() <= threshold ; j++){
      if(data[j].size() - data[i].size() <= threshold){
        filteredData.push_back(make_tuple(i,j));
      }
    }
  }


  return true;
}

tuple<vector<string>, vector<unsigned>> simJoin::rpartition(string r, unsigned threshold)
{
  double numPart = threshold+1, size = r.length();
  unsigned min = size/numPart;
  double parts = size/numPart;
  unsigned fSize = min, sSize = min, fParts = numPart, sParts = 0;

  if((double)min != parts)
  {
    sSize = min+1;
    unsigned mod = (unsigned)(size)%(min*(unsigned)numPart);
    fParts = numPart - mod, sParts = mod;
  }

  vector<string> res;
  vector<unsigned> indices;
  int i;
  for ( i=0; i<fParts; i++){
    unsigned startInd = i*fSize;    
    res.emplace_back(r.substr(startInd, fSize));
    indices.emplace_back(startInd);
  }

  unsigned totSize = i*fSize;

  for (i=0; i<sParts; i++){
    unsigned startInd = totSize + i*sSize;    
    res.emplace_back(r.substr(startInd, sSize));
    indices.emplace_back(startInd);
  }
  
  return make_tuple(res, indices);
}
/**
 * @brief 
 * 
 * @param s -- String "s" of the candidate pair
 * @param sub --> substring from string r to compare
 * @param t --> threshold
 * @param start --> start position Pi of sub in r
 * @param i --> Position of the substring sub in vector<> rString
 * @param delta --> ||s| - |r||
 * @return true --> if there is a match
 * @return false --> if there is no match
 */
bool simJoin::spartition(string s, string sub, int t, int start, int i, int delta)
{
    int nstart = max(0,max(start - (i - 1),start + delta - (t + 1 - i)));
    int nend = min((int)s.length() - 1,min(start + (i - 1),start + delta + (t + 1 - i)));
    string s1 = s.substr(nstart, nend-nstart+1+sub.length());
    int pos = s1.find(sub);
    
    return nstart+pos >= nstart && nstart+pos <=nend;
}

int simJoin::minDistance(std::string word1, std::string word2)
{
  //cout << "IN EDIT DIST" << endl;
  
  int m = word1.size(), n = word2.size();
  vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
  for (int i = 1; i <= m; i++) {
      dp[i][0] = i;
  }
  for (int j = 1; j <= n; j++) {
      dp[0][j] = j;
  }
  for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
          if (word1[i - 1] == word2[j - 1]) {
              dp[i][j] = dp[i - 1][j - 1];
          } else {
              dp[i][j] = min(dp[i - 1][j - 1], min(dp[i][j - 1], dp[i - 1][j])) + 1;
          }
      }
  }
  //cout << "OUT EDIT DIST" << endl;
  return dp[m][n];
    
}
