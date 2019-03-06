#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
#include<string>
#include "monsterMap.h"
#include "heap.h"

/* The node_path_t is used to factor in weights and postions of nodes on the graph to decided where the monster should go. */
class node_path {
  public:
  heap_node_t *hn;
  uint8_t pos[2];
  int32_t cost;
};

/* Compare method used when initializing the heap. */
int node_cmp(const void *key, const void *with) {
  return ((node_path *) key)->cost - ((node_path *) with)->cost;
}

/* Tunneling map used by monsters that can tunnel through rock. Hardness of rock is used to determine distances on map. */
int tunnelingMap(int ioCostArr[21][80], uint8_t hardness[21][80], uint8_t pcLocation[2])
{
	int dim_y = 0;
		int dim_x = 1;
		node_path path[21][80], *p;
		int x,y;
		heap_t h;
		
		for (y = 0; y < 21; y++)
			{
				for (x = 0; x < 80; x++) 
				{
					path[y][x].pos[dim_y] = y;
					path[y][x].pos[dim_x] = x;
				}
			}
		
		for (y = 0; y < 21; y++)
			{
				for (x = 0; x < 80; x++) 
				{
						path[y][x].cost = INT_MAX;	
					
				}
			}
		
		path[pcLocation[1]][pcLocation[0]].cost = 0;	
			
		heap_init(&h, node_cmp, NULL);

		for (y = 0; y < 21; y++)
		{
			for (x = 0; x < 80; x++)
				{
					if (hardness[y][x] < 255)
					{
						path[y][x].hn = heap_insert(&h, &path[y][x]);
					} else {
						path[y][x].hn = NULL;
					}	
				}
		}
		
		while ((p = (node_path *) heap_remove_min(&h)))
		{
			p->hn = NULL;
			
			//Direction 1
			if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost > p->cost + ((hardness[p->pos[dim_y] - 1][p->pos[dim_x]] / 85) + 1)))
			{
				path[p->pos[dim_y] - 1][p->pos[dim_x]].cost = p->cost + ((hardness[p->pos[dim_y] - 1][p->pos[dim_x]] / 85) + 1);
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x]].hn);
			}
			
			//Direction 2
			if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost > p->cost + ((hardness[p->pos[dim_y] + 1][p->pos[dim_x]] / 85) + 1)))
			{
				path[p->pos[dim_y] + 1][p->pos[dim_x]].cost = p->cost + ((hardness[p->pos[dim_y] + 1][p->pos[dim_x]] / 85) + 1);
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]].hn);
			}
			
			//Direction 3
			if ((path[p->pos[dim_y]][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y]][p->pos[dim_x] - 1].cost > p->cost + ((hardness[p->pos[dim_y]][p->pos[dim_x] - 1] / 85) + 1)))
			{
				path[p->pos[dim_y]][p->pos[dim_x] - 1].cost = p->cost + ((hardness[p->pos[dim_y]][p->pos[dim_x] - 1] / 85) + 1);
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] - 1].hn);
			}
			
			//Direction 4
			if ((path[p->pos[dim_y]][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y]][p->pos[dim_x] + 1].cost > p->cost + ((hardness[p->pos[dim_y]][p->pos[dim_x] + 1] / 85) + 1)))
			{
				path[p->pos[dim_y]][p->pos[dim_x] + 1].cost = p->cost + ((hardness[p->pos[dim_y]][p->pos[dim_x] + 1] / 85) + 1);
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] + 1].hn);
			}
		
			//Direction 5
			if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost > p->cost + ((hardness[p->pos[dim_y] - 1][p->pos[dim_x] - 1] / 85) + 1)))
			{
				path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost = p->cost + ((hardness[p->pos[dim_y] - 1][p->pos[dim_x] - 1] / 85) + 1);
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn);
			}
			
			//Direction 6
			if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost > p->cost + ((hardness[p->pos[dim_y] - 1][p->pos[dim_x] + 1] / 85) + 1)))
			{
				path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost = p->cost + ((hardness[p->pos[dim_y] - 1][p->pos[dim_x] + 1] / 85) + 1);
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn);
			}
			
			//Direction 7
			if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost > p->cost + ((hardness[p->pos[dim_y] + 1][p->pos[dim_x] - 1] / 85) + 1)))
			{
				path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost = p->cost + ((hardness[p->pos[dim_y] + 1][p->pos[dim_x] - 1] / 85) + 1);
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn);
			}
			
			//Direction 8
			if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost > p->cost + ((hardness[p->pos[dim_y] + 1][p->pos[dim_x] + 1] / 85) + 1)))
			{
				path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost = p->cost + ((hardness[p->pos[dim_y] + 1][p->pos[dim_x] + 1] / 85) + 1);
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn);		
			}
			
		}
		
		int i,j;
		for(i = 0; i < 21; i++)
		{
			for(j = 0; j < 80; j++)
			{
				ioCostArr[i][j] = path[i][j].cost;
			}
		}
		
		heap_delete(&h);
	
	return 0;
}

/* Map for non-tunneling monsters. Only corridors and rooms are considered when the function is called. The function consists of the BFS algorithm. */
int nonTunnelingMap(int ioCostArr[21][80], uint8_t hardness[21][80], uint8_t pcLocation[2])
{
		int dim_y = 0;
		int dim_x = 1;
		node_path path[21][80], *p;
		int x,y;
		heap_t h;
		
		for (y = 0; y < 21; y++)
			{
				for (x = 0; x < 80; x++) 
				{
					path[y][x].pos[dim_y] = y;
					path[y][x].pos[dim_x] = x;
				}
			}
		
		for (y = 0; y < 21; y++)
			{
				for (x = 0; x < 80; x++) 
				{
						path[y][x].cost = INT_MAX;	
					
				}
			}
		
		path[pcLocation[1]][pcLocation[0]].cost = 0;	
			
		heap_init(&h, node_cmp, NULL);

		for (y = 0; y < 21; y++)
		{
			for (x = 0; x < 80; x++)
				{
					if (hardness[y][x] == 0)
					{
						path[y][x].hn = heap_insert(&h, &path[y][x]);
					} else {
						path[y][x].hn = NULL;
					}	
				}
		}
		
		while ((p = (node_path *) heap_remove_min(&h)))
		{
			p->hn = NULL;
			
			//Direction 1
			if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost > p->cost + 1))
			{
				path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost = p->cost + 1;
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn);
			}
			
			//Direction 2
			if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost > p->cost + 1))
			{
				path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost = p->cost + 1;
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn);
			}
			
			//Direction 3
			if ((path[p->pos[dim_y]][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y]][p->pos[dim_x] - 1].cost > p->cost + 1))
			{
				path[p->pos[dim_y]][p->pos[dim_x] - 1].cost = p->cost + 1;
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] - 1].hn);
			}
			
			//Direction 4
			if ((path[p->pos[dim_y]][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y]][p->pos[dim_x] + 1].cost > p->cost + 1))
			{
				path[p->pos[dim_y]][p->pos[dim_x] + 1].cost = p->cost + 1;
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] + 1].hn);
			}
			
			//Direction 5
			if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost > p->cost + 1))
			{
				path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost = p->cost + 1;
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn);
			}
			
			//Direction 6
			if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost > p->cost + 1))
			{
				path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost = p->cost + 1;
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn);
			}
			
			//Direction 7
			if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost > p->cost + 1))
			{
				path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost = p->cost + 1;
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn);
			}
			
			//Direction 8
			if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost > p->cost + 1))
			{
				path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost = p->cost + 1;
				heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn);
			}
			
		}
		
		int i,j;
		for(i = 0; i < 21; i++)
		{
			for(j = 0; j < 80; j++)
			{
				ioCostArr[i][j] = path[i][j].cost;
			}
		}
		
		heap_delete(&h);
			
		return 0;	
}
