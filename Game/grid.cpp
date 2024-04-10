#include "pch.h"
#include "grid.h"

grid::grid(float _width, float _depth, int m, int n)
{
	const int m_mapwidth = 20;
	const int m_mapDepth = 20;
	int map[m_mapwidth][m_mapDepth] = { {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
										};

	int vertexCount = m * n;
	/*int TriangleCount = (m - 1) * (n - 1) * 2;

	float halfWidth = 0.5f * _width;
	float halfDepth = 0.5f * _depth;

	float dx = _width / (n - 1);
	float dz = _depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);*/
	Vector3 v;

	m_centre = Vector3(m_mapwidth * _width / 2, 0.0f, m_mapDepth * _depth / 2);

	for(int i=0; i< m_mapDepth; ++i)
	{
		//float z = halfDepth - i * dx;
		float z = _depth * i;
		for (int j = 0; j < m_mapwidth; ++j)
		{
			//float x = -halfWidth + j * dx;
			float x = _width * j;

			v = Vector3(x, 0.0f, z);
			if(map[j][i] == 0)
			{
				Tile tile = Tile(j, i,Vector3(x, 0.0f, z), Vector3::One, _width, _depth, true);
				m_gridmap.push_back(tile);
				m_pos.push_back(v);
			}
			else
			{
				Tile tile = Tile(j, i, Vector3(x, 0.0f, z), Vector3::One, _width, _depth, false);
				m_gridmap.push_back(tile);
				m_pos.push_back(v);
				m_walls.push_back(tile);
			}
			

			
		}
	}
}

grid::~grid()
{

}

float grid::GetTileWidth()
{
	return m_gridmap[0].GetWidth();
}
float grid::GetTileDepth()
{
	return m_gridmap[0].GetDepth();
}

std::vector<GridLocation>& grid::Neighbours(GridLocation _currentTile)
{
	GridLocation direction[4] = { {1,0} ,{0,1},{-1,0},{0,-1} };
	std::vector<GridLocation> result;

	for (int i = 0; i < 4.; ++i)
	{
		GridLocation neighbour = { _currentTile.x + direction[i].x, _currentTile.y + direction[i].y };
		if (m_gridmap[neighbour.x + neighbour.y * 20].isReachable() && neighbour.x >= 0 && neighbour.x <= 20 && neighbour.y >= 0 && neighbour.y <= 20)
		{
			result.push_back(neighbour);
		}
	}

	return result;
}