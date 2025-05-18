#include "iostream"
#include <cstdio>
#include <ostream>
#include <vector>

using namespace std;

vector<vector<int>> corrector(vector<vector<int>> x, const vector<vector<int>>& m, int N) {
  vector<float> averages(N, 0);

  for (int i = 0; i < N; ++i) {
    float sum = 0;
    for (int j = 0; j < N; ++j) {
      sum += m[i][j];
    }
    averages[i] = sum / N;
  }

  int max_i = 0;
  for (int i = 1; i < N; ++i) {
    if (averages[i] > averages[max_i]) {
      max_i = i;
    }
  }

  for (int i = 0; i < N; ++i) {
    if (x[i][max_i] != 0) {
      x[i][max_i] = 0;
      int max_loc = 0;
      for (int j = 0; j < N; ++j) {
        if (x[max_i][j] == 0 && m[max_i][j] > m[max_i][max_loc] && j != max_i) {
          max_loc = j;
        }
      }
      x[max_i][max_loc] = 1;
    }
  }

  return x;
}


vector<vector<int>> only_greedy(vector<vector<int>> m, int N) {
  vector<vector<int>> x(N, vector<int>(N, 0));

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i != j && x[i][j] == 0 && x[j][i] == 0) {
        if (m[i][j] > m[j][i]) {
          x[i][j] = 1;
        } else {
          x[j][i] = 1;
        }
      }
    }
  }

  

  return corrector(x, m, N);
}

int evaluate(const vector<vector<int>> &x, const vector<vector<int>> &m,
             int N) {
  int score = 0;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (x[i][j] != 0) {
        score += m[i][j];
      }
    }
  }

  return score;
}

vector<vector<int>> local_search(vector<vector<int>> x, vector<vector<int>> m,
                                 int N) {
  int best_score = evaluate(x, m, N);
  bool improved = true;

  while (improved) {
    improved = false;
    for (int i = 0; i < N; ++i) {
      for (int j = i + 1; j < N; ++j) {
        if (x[i][j] != x[j][i]) {
          // Try flipping
          swap(x[i][j], x[j][i]);
          int new_score = evaluate(x, m, N);
          if (new_score > best_score) {
            best_score = new_score;
            improved = true;
          } else {
            // Revert if no improvement
            swap(x[i][j], x[j][i]);
          }
        }
      }
    }
  }

  return corrector(x, m, N);
}

int main() {
  int N = 3;

  vector<vector<int>> m(N, vector<int>(N, 0));
  cout << "Insert matrix values" << endl;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cin >> m[i][j];
    }
  }

  // Optional: print to verify
  cout << "Matrix m:" << endl;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cout << m[i][j] << " ";
    }
    cout << endl;
  }

  cout << "The only greedy Solution:" << endl;
  vector<vector<int>> x = only_greedy(m, N);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cout << x[i][j] << " ";
    }
    cout << endl;
  }

  cout << "The greedy + local search Solution:" << endl;
  vector<vector<int>> y = local_search(x, m, N);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cout << y[i][j] << " ";
    }
    cout << endl;
  }

  return 0;
}