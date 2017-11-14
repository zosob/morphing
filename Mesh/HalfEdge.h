#ifndef _MESHLIB_HALFEDGE_H_
#define _MESHLIB_HALFEDGE_H_

#include  <assert.h>
#include <cmath>
#include <string>
#include "Edge.h"


namespace MeshLib {

class CVertex;
class CEdge;
class CFace;

class CHalfEdge
{
    public:

        CHalfEdge() {
            m_edge = NULL;
            m_vertex = NULL;
            m_prev = NULL;
            m_next = NULL;
            m_face = NULL;
        };
        ~CHalfEdge() {};

        CEdge*&            edge()    {
            return m_edge;
        };

        CEdge* get_edge() {
            return m_edge;
        };

        //get the target vertex
        CVertex*&          vertex()  {
            return m_vertex;
        };
        CVertex*&          target()  {
            return m_vertex;
        };

        CVertex*&          source()  {
            return m_prev->vertex();
        };
        CHalfEdge*&     he_prev() {
            return m_prev;
        };
        CHalfEdge*&     he_next() {
            return m_next;
        };

        //get the opposite he
        //it could be NULL if this he is on the boundary
        CHalfEdge*& he_sym()
        {
            return m_edge->other(this);
        };

        CFace*&      face()    {
            return m_face;
        };

        //find the next he which also pointers to the target vertex next to this following ccw
        //the key is how to choose the previous_he (m_prev) which represent the ccw
        //step1: get the opposite he which is he_sym() of this
        //step2: get the previous(opposite(this))  opposite->he_prev();
        //
        //it would return NULL if on boundary
        CHalfEdge*    ccw_rotate_about_target();

        //find the next he which also pointers to the target vertex next to this following CLW
        //the key is how to choose the next_he (m_next) which represent the clw
        //step1: get the next_he of this
        //setp2: get the opposite(next(this))   he_next()->he_sym();
        //
        //if return NULL, the edge next to this he CLW around target is on the boundary
        CHalfEdge*    clw_rotate_about_target();

        //if return NULL, the edge CCW next to this he is on the boundary
        CHalfEdge*    ccw_rotate_about_source();

        //if return NULL, this he is on boundary
        CHalfEdge*    clw_rotate_about_source();

        std::string& string() {
            return m_string;
        };
        
    private:

        CEdge*          m_edge;
        CFace*          m_face;

        CVertex*        m_vertex;   //target vertex
        CHalfEdge*      m_prev;     //the m_prev->vertex() is the source vertex
        CHalfEdge*      m_next;

        std::string     m_string;
        
};


}//namespace MeshLib

#endif //_MESHLIB_HALFEDGE_H_ defined