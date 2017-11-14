#ifndef _MESHLIB_EDGE_H_
#define _MESHLIB_EDGE_H_

#include <assert.h>
#include <stdlib.h>
#include <cmath>
#include <string>


namespace MeshLib {

class CHalfEdge;
class CVertex;

class CEdge
{
public:
    CEdge()
    {
        m_halfedge[0] = NULL;
        m_halfedge[1] = NULL;
    };
    ~CEdge() {};

    CHalfEdge*& halfedge(int i)
    {
        assert(0<=i && i<2);
        return m_halfedge[i];
    };

    CHalfEdge* get_halfedge(int i)
    {
        assert(0 <= i && i<2);
        return m_halfedge[i];
    };

    bool boundary()
    {
        return (m_halfedge[0] == NULL && m_halfedge[1] != NULL)
            || (m_halfedge[0] != NULL && m_halfedge[1] == NULL);
    };

    //get the opposite h-e
    CHalfEdge*& other(CHalfEdge* he)
    {
        return (he != m_halfedge[0]) ? m_halfedge[0]:m_halfedge[1];
    };

    std::string& string()
    {
        return m_string;
    };

private:

    CHalfEdge*      m_halfedge[2];
    std::string     m_string;

public:
    double m_k;

};

class CEdgeKey
{
public:
    CEdgeKey(CVertex* v1, CVertex* v2);
    ~CEdgeKey() {};
    bool operator< (const CEdgeKey& key) const;
    bool operator==(const CEdgeKey& key) const;

protected:
    CVertex* m_verts[2];
};

}//name space MeshLib

#endif //_MESHLIB_EDGE_H_ defined