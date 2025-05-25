#include "iostream"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>

using namespace std;

int evaluate_order(const vector<int>& order, const vector<vector<int>>& m) {
  int N = order.size();
  int score = 0;
  // For every pair i < j, add m[order[i]][order[j]]
  for (int i = 0; i < N; ++i) {
      for (int j = i + 1; j < N; ++j) {
          score += m[order[i]][order[j]];
      }
  }
  return score;
}

void local_search_order(vector<int>& order, const vector<vector<int>>& m) {
  int N = order.size();
  bool improved = true;

  while (improved) {
      improved = false;
      // Try swapping adjacent pairs
      for (int i = 0; i < N - 1; ++i) {
          swap(order[i], order[i + 1]);
          int new_score = evaluate_order(order, m);
          swap(order[i], order[i + 1]); // revert

          int current_score = evaluate_order(order, m);

          if (new_score > current_score) {
              // Accept swap
              swap(order[i], order[i + 1]);
              improved = true;
          }
      }
  }
  int counter=0;
  vector<vector<int>> y(N, vector<int>(N, 0));
  for (int i = 0; i < N; ++i) {
      for (int j = i + 1; j < N; ++j) {
          int u = order[i];
          int v = order[j];
          y[u][v] = 1;  
      }
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (y[i][j] == 1)
        counter += m[i][j];
      cout << y[i][j] << " ";
    }
    cout << endl;
  }

  cout << "Points with greedy solution and local search enhancement: "<< counter << endl;
}

void buildAcyclicPriority(const vector<vector<int>> &m, int N) {
  
  vector<pair<int,int>> node_score; 
  for (int i = 0; i < N; ++i) {
      int sum_row = 0;
      for (int j = 0; j < N; ++j) {
          sum_row += m[i][j];
      }
      node_score.emplace_back(sum_row, i);
  }

  
  sort(node_score.rbegin(), node_score.rend());

  
  vector<int> order(N);
  for (int i = 0; i < N; ++i) {
      order[i] = node_score[i].second;
  }

  vector<vector<int>> x(N, vector<int>(N, 0));
  for (int i = 0; i < N; ++i) {
      for (int j = i + 1; j < N; ++j) {
          int u = order[i];
          int v = order[j];
          x[u][v] = 1;  
      }
  }
  
  int counter = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (x[i][j] == 1)
        counter += m[i][j];
      cout << x[i][j] << " ";
    }
    cout << endl;
  }

  cout << "Points with only greedy: " << counter << endl;
  counter = 0;
  cout << "The greedy + local search Solution:" << endl;
  local_search_order(order, m);
}

void grasp(const vector<vector<int>>& m, float alpha) {
  int N = m.size();
  vector<int> order;
  vector<bool> chosen(N, false);

  
  vector<int> scores(N, 0);
  for (int i = 0; i < N; ++i) {
      int s = 0;
      for (int j = 0; j < N; ++j) {
          s += m[i][j];
      }
      scores[i] = s;
  }

  srand(time(nullptr));

  while ((int)order.size() < N) {
      
      vector<pair<int, int>> candidates;
      for (int i = 0; i < N; ++i) {
          if (!chosen[i]) {
              candidates.push_back({scores[i], i});
          }
      }

      
      sort(candidates.rbegin(), candidates.rend());

      
      int rcl_size = max(1, int(alpha * candidates.size()));

      
      int pick = rand() % rcl_size;

      int selected = candidates[pick].second;
      order.push_back(selected);
      chosen[selected] = true;
  }

  vector<vector<int>> y(N, vector<int>(N, 0));
  for (int i = 0; i < N; ++i) {
      for (int j = i + 1; j < N; ++j) {
          int u = order[i];
          int v = order[j];
          y[u][v] = 1;  
      }
  }

  int counter = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (y[i][j] == 1)
        counter += m[i][j];
      cout << y[i][j] << " ";
    }
    cout << endl;
  }
  cout << "Points with GRASP solution: " << counter << endl;
}

int main() {
  int N;
  vector<vector<int>> m;

  ifstream infile("input.txt");
  string line;

  // Read N
  while (getline(infile, line)) {
    if (line.find("N") != string::npos) {
      size_t eq = line.find('=');
      if (eq != string::npos) {
        N = stoi(line.substr(eq + 1));
      }
      break;
    }
  }

  // Skip to matrix
  while (getline(infile, line)) {
    if (line.find('[') != string::npos)
      break;
  }

  // Read matrix rows
  for (int i = 0; i < N; ++i) {
    getline(infile, line);
    vector<int> row;
    stringstream ss(line);
    char ch;
    int val;
    while (ss >> ch) {
      if (isdigit(ch) || ch == '-') {
        ss.putback(ch);
        ss >> val;
        row.push_back(val);
      }
    }
    m.push_back(row);
  }

  cout << "Matrix m:" << endl;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cout << m[i][j] << " ";
    }
    cout << endl;
  }

  cout << "The only greedy Solution:" << endl;
  buildAcyclicPriority(m, N);
  
  grasp(m, 0.1);
  return 0;
}