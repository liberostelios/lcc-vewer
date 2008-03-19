// file: Iso_rectangle_2_query.C

#include <CGAL/Simple_cartesian.h>
#include <cassert>
#include <CGAL/Kd_tree.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/Fuzzy_iso_box.h>

#include <vector>
#include <iostream>

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_2 Point;
typedef K::Vector_2 Vector;
typedef K::Iso_rectangle_2 Iso_rectangle;
typedef CGAL::Random_points_in_square_2<Point> Random_points_iterator;
typedef CGAL::Counting_iterator<Random_points_iterator> N_Random_points_iterator;
typedef CGAL::Search_traits_2<K> Traits;
typedef CGAL::Fuzzy_iso_box<Traits> Fuzzy_box;	
typedef CGAL::Kd_tree<Traits> Tree;


int main() {

  const int N=10000;
  
  // generator for random data points in the square ( (-1,-1), (1,1) ) 
  Random_points_iterator rpit( 1.0);
  
  // construct list containing N random points
  std::list<Point> all_points(N_Random_points_iterator(rpit,0),
			      N_Random_points_iterator(N));
  
  // Insert also the N points in the tree
  Tree tree(all_points.begin(),all_points.end());
  
  Point p(0.1, 0.2);
  Point q(0.3, 0.5);

  Iso_rectangle exact_ic(p,q);

  Fuzzy_box exact_range(p,q);

  std::list<Point> result;
  // Searching the box r exactly
  tree.search( std::back_inserter( result ), exact_range);
  
  // test the results of the exact query
  std::list<Point> copy_all_points(all_points);
  std::list<Point>::iterator pt;
  for (pt=result.begin(); (pt != result.end()); ++pt) {
    assert(! exact_ic.has_on_unbounded_side(*pt) || exact_ic.has_on_boundary(*pt));
    copy_all_points.remove(*pt);
  }
  
  for (pt=copy_all_points.begin(); (pt != copy_all_points.end()); ++pt) {
    assert(exact_ic.has_on_unbounded_side(*pt) || exact_ic.has_on_boundary(*pt));
  }


  result.clear();
  // approximate range searching using value 0.1 for fuzziness parameter
  Fuzzy_box approximate_range(p,q,0.05);
  Iso_rectangle inner_ic(p+ 0.05*Vector(1,1),q-0.05*Vector(1,1));
  Iso_rectangle outer_ic(p- 0.05*Vector(1,1),q+0.05*Vector(1,1));

  tree.search(std::back_inserter( result ), approximate_range);
  // test the results of the approximate query
  for (pt=result.begin(); (pt != result.end()); ++pt) {
    // a point we found may be slighlty outside the isorectangle
    assert(! outer_ic.has_on_unbounded_side(*pt) || outer_ic.has_on_boundary(*pt));
    all_points.remove(*pt);
  }
  
  for (pt=all_points.begin(); (pt != all_points.end()); ++pt) {
    assert(inner_ic.has_on_unbounded_side(*pt) || inner_ic.has_on_boundary(*pt));
  }
  std::cout << "done" << std::endl;

  return 0;
}


