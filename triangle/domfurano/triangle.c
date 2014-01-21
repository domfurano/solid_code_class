/*
 * Dominic Furano
 * January 2013
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

/* Represents a point and the squared distance of the side
   opposite. */
typedef struct
{
   long long x;
   long long y;
   long long sqd_d;
}pt;

/* Returns the dot product of two pts. If the two pts are acute,
   the dot product is positive. If the two pts are obtuse, the dot product
   is negative. If the two pts are perpendicular, the dot product is zero.
   Can overflow with invalid inputs. Will not overflow with valid inputs. */
long long dot_product(long long x0, long long y0, long long x1, long long y1)
{
   return (x0 * x1) + (y0 * y1);
}

/* Returns the squared distance between two vertices. Will not overflow with valid inputs. */
long long sqd_dst(long long x0, long long y0, long long x1, long long y1)
{
   long long a = (x1 - x0);
   long long b = (y1 - y0);
   return a * a + b * b;
}

/* Comparison routine for qsort. */
int compare (const void * a, const void * b)
{
   return (*(pt*)a).sqd_d < (*(pt*)b).sqd_d;
}

/* Determines the type of angle opposite the largest side. */
void angle_type(pt pts[3])
{
   /* Find longest angle. */
   qsort(pts, 3, sizeof(pt), compare);
   /* Calculate vectors with one endpoint at the origin. */
   long long type = dot_product(pts[2].x - pts[0].x, pts[2].y - pts[0].y, pts[1].x - pts[0].x, pts[1].y - pts[0].y);
    
   if (type == 0)
      printf("right\n");
   else if (type > 0)
      printf("acute\n");
   else
      printf("obtuse\n");
}

/* Determines triangle type based on lengths of sides. */
void edge_type(pt pts[3])
{
   /* Check for collinearity. */
   if ((pts[0].x * (pts[1].y - pts[2].y) +
	pts[1].x * (pts[2].y - pts[0].y) +
	pts[2].x * (pts[0].y - pts[1].y)) == 0)
   {
      printf("not a triangle\n");
      exit(EXIT_SUCCESS);
   }
    
   /* Compute the squared length of sides of triangle. */
   pts[0].sqd_d = sqd_dst(pts[1].x, pts[1].y, pts[2].x, pts[2].y);
   pts[1].sqd_d = sqd_dst(pts[0].x, pts[0].y, pts[2].x, pts[2].y);
   pts[2].sqd_d = sqd_dst(pts[0].x, pts[0].y, pts[1].x, pts[1].y);
    
   /* Output type of triangle. Equilateral triangles with integer vertices are impossible. */
   if ((pts[0].sqd_d == pts[1].sqd_d) || (pts[0].sqd_d == pts[2].sqd_d) || (pts[1].sqd_d == pts[2].sqd_d))
      printf("isosceles ");
   else
      printf("scalene ");
}

int main(int argc, char *argv[])
{
   pt pts[3]; /* Coordinate values. */
    
   /* Convert and save command line parameters. */
   long long x;
   long long y;
   for (int i = 1; i < argc; i += 2)
   {
      sscanf(argv[i], "%lld", &x);
      sscanf(argv[i + 1], "%lld", &y);
      assert(x >= 0 && y >= 0 && x <= INT_MAX && y <= INT_MAX); /* Do not allow invalid inputs. */
      pts[i/2].x = x;
      pts[i/2].y = y;
   }
    
   edge_type(pts);
   angle_type(pts);
    
   exit(EXIT_SUCCESS);
}
