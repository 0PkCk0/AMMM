#include "iostream"
#include <cstdio>
#include <ostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

vector<vector<int>> corrector(vector<vector<int>> x, const vector<vector<int>> m, int N)
{
  vector<float> averages(N, 0);

  for (int i = 0; i < N; ++i)
  {
    float sum = 0;
    for (int j = 0; j < N; ++j)
    {
      sum += m[i][j];
    }
    averages[i] = sum / N;
  }

  int max_i = 0;
  for (int i = 1; i < N; ++i)
  {
    if (averages[i] > averages[max_i])
    {
      max_i = i;
    }
  }

  for (int i = 0; i < N; ++i)
  {
    if (x[i][max_i] != 0)
    {
      x[i][max_i] = 0;
      int max_loc = 0;
      for (int j = 0; j < N; ++j)
      {
        if (x[max_i][j] == 0 && m[max_i][j] > m[max_i][max_loc] && j != max_i)
        {
          max_loc = j;
        }
      }
      x[max_i][max_loc] = 1;
    }
  }

  return x;
}

struct grasp_struct
{
  int index;
  int value;
  grasp_struct(int i, int v) : index(i), value(v) {}
  grasp_struct() : index(0), value(0) {}
};
bool comp(const grasp_struct &a, const grasp_struct &b)
{
  return a.value > b.value;
}

vector<vector<int>> grasp_greedy(const vector<vector<int>> m, int N, float alpha)
{
  vector<grasp_struct> averages(N, grasp_struct(0, 0));
  vector<vector<int>> x(N, vector<int>(N, 0));

  for (int i = 0; i < N; ++i)
  {
    float sum = 0;
    for (int j = 0; j < N; ++j)
    {
      sum += m[i][j];
    }
    averages[i].value = sum / N;
    averages[i].index = i;
  }

  sort(averages.begin(), averages.end(), comp);
  averages.resize(N * alpha);
  srand(time(0));
  for (int i = 0; i < N; i++)
  {
    int random_index = rand() % N * alpha;
    int struct_index = 0;
    for (int j = 0; j < N * alpha; j++)
    {
      if (averages[j].index == random_index)
      {
        struct_index = j;
        break;
      }
    }
    x[random_index][i];
  }

  return corrector(x, m, N);
}

vector<vector<int>> only_greedy(vector<vector<int>> m, int N)
{
  vector<vector<int>> x(N, vector<int>(N, 0));

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      if (i != j && x[i][j] == 0 && x[j][i] == 0)
      {
        if (m[i][j] > m[j][i])
        {
          x[i][j] = 1;
        }
        else
        {
          x[j][i] = 1;
        }
      }
    }
  }

  return corrector(x, m, N);
}

int evaluate(const vector<vector<int>> &x, const vector<vector<int>> &m,
             int N)
{
  int score = 0;

  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      if (x[i][j] != 0)
      {
        score += m[i][j];
      }
    }
  }

  return score;
}

vector<vector<int>> local_search(vector<vector<int>> x, vector<vector<int>> m,
                                 int N)
{
  int best_score = evaluate(x, m, N);
  bool improved = true;

  while (improved)
  {
    improved = false;
    for (int i = 0; i < N; ++i)
    {
      for (int j = i + 1; j < N; ++j)
      {
        if (x[i][j] != x[j][i])
        {
          // Try flipping
          swap(x[i][j], x[j][i]);
          int new_score = evaluate(x, m, N);
          if (new_score > best_score)
          {
            best_score = new_score;
            improved = true;
          }
          else
          {
            // Revert if no improvement
            swap(x[i][j], x[j][i]);
          }
        }
      }
    }
  }

  return corrector(x, m, N);
}

int main()
{
  int N;
  vector<vector<int>> m;

  ifstream infile("input.txt");
  string line;

  // Read N
  while (getline(infile, line))
  {
    if (line.find("N") != string::npos)
    {
      size_t eq = line.find('=');
      if (eq != string::npos)
      {
        N = stoi(line.substr(eq + 1));
      }
      break;
    }
  }

  // Skip to matrix
  while (getline(infile, line))
  {
    if (line.find('[') != string::npos)
      break;
  }

  // Read matrix rows
  for (int i = 0; i < N; ++i)
  {
    getline(infile, line);
    vector<int> row;
    stringstream ss(line);
    char ch;
    int val;
    while (ss >> ch)
    {
      if (isdigit(ch) || ch == '-')
      {
        ss.putback(ch);
        ss >> val;
        row.push_back(val);
      }
    }
    m.push_back(row);
  }

  cout << "Matrix m:" << endl;
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      cout << m[i][j] << " ";
    }
    cout << endl;
  }

  cout << "The only greedy Solution:" << endl;
  vector<vector<int>> x = only_greedy(m, N);
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      cout << x[i][j] << " ";
    }
    cout << endl;
  }

  cout << "The greedy + local search Solution:" << endl;
  vector<vector<int>> y = local_search(x, m, N);
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      cout << y[i][j] << " ";
    }
    cout << endl;
  }

  cout << "The grasp solution:" << endl;
  vector<vector<int>> z = grasp_greedy(m, N, 0.1);
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      cout << y[i][j] << " ";
    }
    cout << endl;
  }

  return 0;
}