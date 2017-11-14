#ifndef  _MESHLIB_VERTEX_H_
#define _MESHLIB_VERTEX_H_


#include <stdlib.h>
#include <string>
#include "../Geometry/Point.h"
#include "../Geometry/Point2.h"


namespace MeshLib {

class CHalfEdge;

class CVertex
{
    public:
    CVertex() {
        m_halfedge = NULL;
        m_boundary = false;
        enable_normal = false;
        enable_rgb = false;
    };
    ~CVertex() {};

    CPoint& point()    {
        return m_point;
    };

    CPoint& normal()   {
        return m_normal;
    };

    CPoint& rgb()   {
        return m_rgb;
    };

    CPoint2& uv()       {
        return m_uv;
    };

    //if on boundary, find the he on the edge that is CCW prior to the most_CCW_IN which is on boundary 
    CHalfEdge* most_ccw_out_halfedge();

    //if on boundary, find the boundary he that pointers out from this vertex
    CHalfEdge* most_clw_out_halfedge();

    //if on boundary, find the boundary he that pointers to this vertex
    //change the m_he
    CHalfEdge* most_ccw_in_halfedge();

    //if on boundary, find the he that is on the edge which is
    //CLW before the on boundary he pointering out from this vertex
    //change the m_he
    CHalfEdge* most_clw_in_halfedge();

    CHalfEdge*& halfedge() {
        return m_halfedge;
    };
    std::string& string() {
        return m_string;
    };

    int&   id() {
        return m_id;
    };
    bool& boundary() {
        return m_boundary;
    };


    private:
    int  m_id ;

    CPoint m_point;     //position
    CPoint m_normal;
    CPoint2 m_uv;
    CPoint m_rgb;


    //neighboring h-e, which means a he "pointering in" this vertex
    //if it is not on the boundary, it should be the most ccw he which pointers in this vertex,
    //  and the most "clw in" halfedge is the he next to m_halfedge following "CCW" ??
    CHalfEdge* m_halfedge;

    bool              m_boundary;
    std::string     m_string;

public:
    double m_mapx;
    double m_mapy;

    bool enable_normal;
    bool enable_rgb;
}; //class CVertex

}//name space MeshLib

#endif //_MESHLIB_VERTEX_H_defined