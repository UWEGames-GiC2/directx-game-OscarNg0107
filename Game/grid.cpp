#include "pch.h"
#include "grid.h"

grid::grid(float _width, float _depth, int m, int n)
{
	const int m_mapwidth = 20;
	const int m_mapDepth = 20;
	int map[m_mapwidth][m_mapDepth] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
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
				Tile tile = Tile(Vector3(x, 0.0f, z), Vector3::One, _width, _depth, true);
				m_gridmap.push_back(tile);
				m_pos.push_back(v);
			}
			else
			{
				Tile tile = Tile(Vector3(x, 0.0f, z), Vector3::One, _width, _depth, false);
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