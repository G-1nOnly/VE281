#include "shortestP2P.hpp"

void ShortestP2P::readGraph()
{
  unsigned int v, e, start, end;
  int w;
  std::cin >> v >> e;
  num_v = v;
  num_e = e;
  edge = new int[num_v * num_v];
  for (unsigned int i = 0; i < num_v; i++)
  {
    for (unsigned int j = 0; j < num_v; j++)
    {
      if (j == i)
        edge[i * num_v + j] = 0;
      else
        edge[i * num_v + j] = INF;
    }
  }
  for (unsigned int i = 0; i < num_e; i++)
  {
    std::cin >> start >> end >> w;
    edge[start * num_v + end] = w;
  }
  find_shortest();
}

void ShortestP2P::distance(unsigned int A, unsigned int B)
{
  if (edge[A * num_v + B] == INF)
    std::cout << "INF" << endl;
  else
    std::cout << edge[A * num_v + B] << endl;
}

void ShortestP2P::find_shortest()
{
  for (unsigned int k = 0; k < num_v; k++)
  {
    for (unsigned int i = 0; i < num_v; i++)
    {
      unsigned int base = i * num_v;
      if (edge[base + k] == INF)
        continue;
      for (unsigned int j = 0; j < num_v; j++)
      {
        unsigned int pos = k * num_v;
        if (edge[pos + j] == INF)
          continue;
        if (edge[base + k] + edge[pos + j] < edge[base + j])
          edge[base + j] = edge[base + k] + edge[pos + j];
        if (i == j && edge[base + j] < 0)
        {
          std::cout << "Invalid graph. Exiting." << endl;
          std::exit(0);
        }
      }
    }
  }
}

ShortestP2P::~ShortestP2P()
{
  delete[] edge;
}