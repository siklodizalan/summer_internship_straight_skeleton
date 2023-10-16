/*
 * Siklodi Zalan
 * Nyari gyakorlat 2023
 */

/*
  How to run this program:
  - /mnt/d/Info/University/Nyari_gyakorlat/CGAL/CGAL-5.6/scripts/cgal_create_CMakeLists
  - cmake -DCGAL_DIR=/mnt/d/Info/University/Nyari_gyakorlat/CGAL/CGAL-5.6 -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=/mnt/d/Info/University/Nyari_gyakorlat/CGAL/boost_1_83_0
  - make
  ./skeleton
*/

/*
 * read_polygon(Polygon_2 poly)
 *    return value:
 *        void
 *    input parameters:
 *        Polygon_2 poly
 *    - beolvassa az INPUT_FILE_PATH-ben talalhato filebol a poligon pontjait,
 *      amelyek soronkent egy pontot kell tartalmazzanak, szokozzel elvalasztva
 *    - ezek a pontok az oramutato jarasaval ellentetes iranyban kell legyenek megadva
 *
 * write_polygon_with_straight_skeleton(SsPtr &iss)
 *    return value:
 *        void
 *    input parameters:
 *        SsPtr iss
 *    - amiutan letrehoztuk a poligon belso skeletonjat, ez a program kiirja magat a
 *      poligon pontjait, "CH"-val jelolve ezeket, a poligon szogfelezoit,
 *      "BH"-val jelolve ezeket es a straigh skeleton pontjait, "IB"-vel jelolve ezeket,
 *      megfelelo formatumban, ahhoz, hogy a masik program fel tudja hasznalni azt.
 */

/*
 * Barmilyen kerdes merul fel, elerheto vagyok a kovetkezo email cimen: zalansiklodi@yahoo.com
 */

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Polygon_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/Straight_skeleton_2/IO/print.h>
#include <CGAL/draw_straight_skeleton_2.h>

#include <boost/shared_ptr.hpp>

#include <cassert>

#include <iostream>
#include <fstream>
#include <string>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Straight_skeleton_2<K> Ss;

typedef boost::shared_ptr<Ss> SsPtr;

const std::string INPUT_FILE_PATH = "/mnt/d/Info/University/Nyari_gyakorlat/CGAL/skeleton/skeleton_input.txt";
const std::string OUTPUT_FILE_PATH = "/mnt/d/Info/University/Nyari_gyakorlat/CGAL/skeleton/polygon.txt";

void read_polygon(Polygon_2 &poly)
{
  std::fstream fin;
  fin.open(INPUT_FILE_PATH, std::ios::in);

  if (fin.is_open())
  {
    int first, second;
    while (fin >> first >> second)
    {
      poly.push_back(Point(first, second));
    }
    fin.close();
  }

  assert(poly.is_counterclockwise_oriented());
}

void write_polygon_with_straight_skeleton(SsPtr &iss)
{
  typedef typename Ss::Vertex_const_handle Vertex_const_handle;
  typedef typename Ss::Halfedge_const_handle Halfedge_const_handle;
  typedef typename Ss::Halfedge_const_iterator Halfedge_const_iterator;

  Halfedge_const_handle null_halfedge;
  Vertex_const_handle null_vertex;

  std::ofstream fout(OUTPUT_FILE_PATH);

  for (Halfedge_const_iterator h = iss->halfedges_begin(); h != iss->halfedges_end(); ++h)
  {
    if (h->id() % 2 != 0)
    {
      continue;
    }
    if (h->is_inner_bisector())
    {
      fout << "IB"
           << " " << std::flush;
    }
    else if (h->is_bisector())
    {
      fout << "BH"
           << " " << std::flush;
    }
    else
    {
      fout << "CH"
           << " " << std::flush;
    }

    fout << h->prev()->vertex()->point().x() << " " << h->prev()->vertex()->point().y();
    fout << " ==> " << std::flush;
    fout << h->vertex()->point().x() << " " << h->vertex()->point().y();
    fout << std::endl;
  }

  fout.close();
}

int main()
{
  Polygon_2 poly;

  read_polygon(poly);

  // letrehozzuk a belso straight skeletonjat a poligonnak
  SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());

  write_polygon_with_straight_skeleton(iss);

  return 0;
}
