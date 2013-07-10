

/*! \mainpage Triangle++

	\section intro Introduction

<table border="0">
<tr>

<td>
If you do not know, what a delaunay triangulation is, you can read more about it 
<a href="http://www.compgeom.com/~piyush/teach/5930/slides/lecture8.ppt">here</a> and 
<a href="http://en.wikipedia.org/wiki/Delaunay_triangulation">here</a>.
This C++ library module is just a wrapper class on the 
<a href="http://www.cs.berkeley.edu/~jrs/">Triangle</a> 
package of <a href="http://www.cs.berkeley.edu/~jrs/">Jonathan</a>.
Many times I have had to use triangle in C++ code bases of mine
and have been forced to use C. At last I thought I would put a 
wrapper on his cool C code and it seems that this is what I got.
The design is not perfect and the code was written in a day,
but it does compile and run on the machines I tried (cygwin/redhat).
The C++ wrapper will certainly slow access down if you want to
mess with the triangulation but the basic delaunay triangulation
should be as fast as triangle. Look at the del_interface.hpp file
for getting started on what this wrapper can do for you. Also
have a look at main.cpp which shows an example of using this
class. The class is thread-safe.

<p>
<b>Requirements</b> : Python, make and C++ compilers. 
Supported C/C++ Compilers: g++ / icpc (Intel C++). 
Also needed is doxygen for generating documentation.
</p>

<p>
<b>Compilation</b> : Just type 'make'
</p>

<p>
<b>Testing</b> : Goto the bin directory, and type 
'./test ../data/input.dat' (after compilation of course).
</p>


</td>

<td><img src="http://upload.wikimedia.org/wikipedia/en/9/92/Delaunay_triangulation.png" 
alt="Delaunay Triangulation Example"></td>


</tr>
</table>


\section Downloads

You can download the latest version of the source code from <a href="triangle++.tar.gz">here</a>.


\section authors Authors
	<ul>
		<li><a href="http://compgeom.com/~piyush">Piyush Kumar</a></li>
		<li>Hopefully more to come... (please feel free to extend this wrapper)</li>
	</ul>
        
\section changelog Change Log

10/21/06: Replaced vertexsort with C++ sort.<br>
10/25/06: Wrapped in tpp namespace for usage with other libraries with similar names. 
Added some more documentation/small changes. Used doxygen 1.5.0 and dot. Tested compilation with
icc 9.0/9.1, gcc-4.1/3.4.6. <br>
11/03/06: Fixed the compilation system. <br>

\todo

<ol> 
    <li> Intel Compiler Warnings with -Wall </li>
    <ul>
         <li> remove the compiler warnings for icpc 9.0/9.1</li>
    </ul>

    <li> Implement vertexmedian() in C++. </li>
    <li> Implement the flip operator as a member function of Delaunay. </li>
</ol>

	
 */




//-----------------------------------------------------------





/*! \file del_interface.hpp
        \brief The main delaunay C++ class for users.

		All one needs to do is include this header and
		understand how to call the member functions and
		what they do. You should be ready to write your
		own delaunay triangulation packages based on this
		c++ wrapper of triangle.
 */
#ifndef DEL_INTERFACE
#define DEL_INTERFACE

#include "dpoint.hpp"
#include <vector>
#include <string>

//! The main namespace in which the Triangle++ project lives
namespace tpp {



//!  The main Delaunay Class that wraps around triangle.
/*!
  This is a C++ wrapper of the triangle package by JRS.
  It currently uses the dpoint class written by me (the
  point class is a d-dimensional point class but for this
  application it only uses the d=2 case).
*/
class Delaunay {
public:
	//! Point Typedef
    /*! Warning: If you want to use your own point class, 
	    you might have to work hard...
	*/
	typedef  reviver::dpoint <double, 2> Point; 

private:
	
	std::vector< Point >  PList;/*! Stores the input point list. */
	void *in;					/*! Used for intput to triangle  */
	void *delclass;				/*! triangle is wrapped in this pointer. */
	void *pmesh;				/*! pointer to triangle mesh */
	void *pbehavior;
	bool Triangulated;

	void Triangulate(std::string& triswitches);

public:
	//! The main constructor.
    /*!
      Takes a vector of 2 dimensional points where
	  each of the coordinates is expressed as double.
    */
	Delaunay(std::vector< Point >& v){
		PList.assign(v.begin(), v.end());
		Triangulated = false;
	}

	//! Delaunay Triangulate the input points
    /*!
      This function calls triangle to delaunay triangulate
	  points given as input to the constructor of this class.
    */
	void Triangulate() { std::string s = "vzQ"; Triangulate( s ); } ;

	//! Output a geomview .off file containing the delaunay triangulation
    /*!
      \param fname output file name.
    */
	void writeoff(std::string& fname);

	//! Number of edges in the triangulation
	 /*!
      \return Number of Edges
      Remember to call Triangulate before using this function.
    */
	int nedges();
	
	//! Number of triangles in the triangulation
	 /*!
      \return Number of Triangles
      Remember to call Triangulate before using this function.
    */
	
	int ntriangles();

	//! Number of edges in the triangulation
	 /*!
      \return Number of Edges
      Remember to call Triangulate before using this function.
    */
	
 	 //! Number of vertices in the triangulation
	 /*!
      \return Number of Vertices
      Remember to call Triangulate before using this function.
    */
	int nvertices();

	 //! Number of vertices on the convex hull.
	 /*!
      \return Number of vertices on the convex hull.
      Remember to call Triangulate before using this function.
    */
	int hull_size();

	//! The main destructor.
    /*!
      Does memory cleanup mostly.
    */
	
	~Delaunay();

	///////////////////////////////
	//
	// Vertex Iterator
	//
	///////////////////////////////

	//!  The vertex iterator for the Delaunay class
	class vIterator {
		private:
			vIterator(Delaunay *);  //! To set container
			Delaunay *MyDelaunay;     //! Which container do I point

			void *vloop;		  //! Triangles Internal data.

		public:
			vIterator operator++();
			vIterator():vloop(NULL) {};
			Point &operator*() const; 
			~vIterator();

		friend class Delaunay;
		friend bool operator==(vIterator const &, vIterator const &);
		friend bool operator!=(vIterator const &, vIterator const &);
	};
	
	//! Vertex iterator begin function
    vIterator vbegin(){ return vIterator(this);} ;
	//! Vertex iterator end function
	vIterator vend();

	//! Given an iterator, find its index in the input vector of points.
	int vertexId(vIterator const &);

	//! Given an index, return the actual double Point
	const Point& point_at_vertex_id(int i) {return PList[i];};


	friend class vIterator; 


	///////////////////////////////
	//
	// Face Iterator
	//
	///////////////////////////////

	//!  The face iterator for the Delaunay class
	class fIterator {
                private:

					struct tdata {
						double ***tri;
						int orient;
					};

					typedef struct tdata  poface;

                        fIterator(Delaunay *);  //! To set container
                        Delaunay *MyDelaunay;     //! Which container do I point

                        //void *floop;              //! Triangles Internal data.
						poface floop;
						
                public:
                        void operator++();
						fIterator() {floop.tri = NULL;};
			            ~fIterator();

                friend class Delaunay;
                friend bool operator==(fIterator const &, fIterator const &);
                friend bool operator!=(fIterator const &, fIterator const &);
        };

		//! Face iterator begin function
        fIterator fbegin(){ return fIterator(this);	} ;
		//! Face iterator end function
        fIterator fend();
        int faceId(fIterator const &);


	//! Access the origin (Org) vertex of a face.
	 /*!
	  \param fit  Face interator.
      \return Index of the vertex in pList.

	  A triangle abc has origin (org) a,destination (dest) b, and apex (apex)  
      c.  These vertices occur in counterclockwise order about the triangle.  
      Remember to call Triangulate before using this function.
	  Do not use it on a null iterator.
    */
	int Org (fIterator const & fit);

	//! Access the destination (Dest) vertex of a face.
	 /*!
	  \param fit  Face interator.
      \return Index of the vertex in pList.

	  A triangle abc has origin (org) a,destination (dest) b, and apex (apex)  
      c.  These vertices occur in counterclockwise order about the triangle.  
      Remember to call Triangulate before using this function.
	  Do not use it on a null iterator.
    */

	int Dest(fIterator const & fit);

	//! Access the apex (Apex) vertex of a face.
	 /*!
	  \param fit  Face interator.
      \return Index of the vertex in pList.

	  A triangle abc has origin (org) a,destination (dest) b, and apex (apex)  
      c.  These vertices occur in counterclockwise order about the triangle.  
      Remember to call Triangulate before using this function.
	  Do not use it on a null iterator.
    */

	int Apex(fIterator const & fit);


	//! Access the triangle adjoining edge i
	 /*!
	  \param fit  Face Iterator
	  \param i  edge number  
          \return The vertex on the opposite face

	  A triangle abc has origin (org) a,destination (dest) b, and apex (apex)  
          c.  These vertices occur in counterclockwise order about the triangle.  
	  <ul>
	  <li>sym(abc, 0) -> ba*</li>
	  <li>sym(abc, 1) -> cb*</li>
	  <li>sym(abc, 2) -> ac*</li>
	  </ul>
	  * is the farthest vertex on the adjoining triangle whose index
	  is returned. A -1 is returned if the edge is part of the convex hull.
      Remember to call Triangulate before using this function.
	  Do not use it on a null iterator.
    */

	int Sym(fIterator const & fit, char i);


	//! Access the triangle opposite to current edge of the face
	 /*!
	  \param fit  Face iterator
          \return The iterator of the opposite face

	  A triangle abc has origin (org) a,destination (dest) b, and apex (apex)  
          c.  These vertices occur in counterclockwise order about the triangle.  
	  The iterator
	  to the triangle is returned. The iterator is empty if the edge
	  is on the convex hull.
          Remember to call Triangulate before using this function.
	  Do not use it on a null iterator.
    */

	fIterator Sym(fIterator const & fit);


	//! Is the iterator empty?
	 /*!
	  \param fit  Face interator.
      \return true if the iterator is empty
    */

	inline bool empty(fIterator const & fit)
		{ return fit.floop.tri == NULL; };

	//! Is the iterator pointing to the dummy triangle?
	 /*!
	  \param fit  Face interator.
      \return true if the iterator is of the dummy triangle.
    */

	bool isdummy(fIterator const & fit);


     
	//! Find the next edge (counterclockwise) of a triangle.   
	 /*!
	  \param fit  face iterator
      \return The face iterator corresponding to the next counterclockwise edge of a triangle

	  Lnext(abc) -> bca.
	  Remember to call Triangulate before using this function.
	  Do not use it on a null iterator.
    */
	fIterator Lnext(fIterator const & fit);

	//! Find the previous edge (clockwise) of a triangle.   
	 /*!
	  \param fit  face iterator
      \return The face iterator corresponding to the previous clockwise edge of a triangle

	  Lprev(abc) -> cab.
	  Remember to call Triangulate before using this function.
	  Do not use it on a null iterator.
    */
	fIterator Lprev(fIterator const & fit);

	//! Find the next edge (counterclockwise) of a triangle with the same origin
	 /*!
	  \param fit  face iterator
      \return The face iterator corresponding to the next edge counterclockwise with the same origin.

	  Onext(abc) -> ac*.
	  Remember to call Triangulate before using this function.
	  Do not use it on a null iterator.
    */
	fIterator Onext(fIterator const & fit);

	
	//! Find the next edge clockwise with the same origin.
	 /*!
	  \param fit  face iterator
      \return The face iterator corresponding to the next edge clockwise with the same origin.

	  Onext(abc) -> a*b.
	  Remember to call Triangulate before using this function.
	  Do not use it on a null iterator.
    */
	fIterator Oprev(fIterator const & fit);


// TODO List: (for face iterators)
/*  dnext:  Find the next edge counterclockwise with the same destination.   */
/*  dnext(abc) -> *ba                                                        */
/*                                                                           */
/*  dprev:  Find the next edge clockwise with the same destination.          */
/*  dprev(abc) -> cb*                                                        */
/*                                                                           */
/*  rnext:  Find the next edge (counterclockwise) of the adjacent triangle.  */
/*  rnext(abc) -> *a*                                                        */
/*                                                                           */
/*  rprev:  Find the previous edge (clockwise) of the adjacent triangle.     */
/*  rprev(abc) -> b**                                                        */



	//! Calculate incident triangles around a vertex.
	 /*!
	  \param vertexid The vertex for which you want incident triangles.
          \param ivv Returns triangles around a vertex in counterclockwise order.

	  Note that behaviour is undefined if vertexid is greater than 
	  number of vertices - 1. Remember to call Triangulate before using this function.
	  All triangles returned have Org(triangle) = vertexid.
	  All triangles returned are in counterclockwise order.
    */

	void trianglesAroundVertex(int vertexid, std::vector<int>& ivv );

	//! Calculate the area of a face. 
	 /*!
	  \param fit  Face interator.
      \return area of the face associated with the iterator.

    */

	double area(fIterator const & fit);



	//! Point locate a vertex v
	/*!
   	  \param vertexid  vertex id
          \return a face iterator whose origin is v.
        */

	fIterator locate(int vertexid);



    friend class fIterator;


}; // Class delaunay

} // namespace tpp ends.

#endif


