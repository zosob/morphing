#ifndef _MESHLIB_MESH_H_
#define _MESHLIB_MESH_H_

#define MAX_LINE 2048

#include <assert.h>
#include <cmath>
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Vertex.h"
#include "../Geometry/Point.h"
#include <list>
#include <map>


namespace MeshLib {

class CVertex;
class CHalfEdge;
class CEdge;
class CFace;
class CWMesh;

class CMesh
{
public:
    // pointer to Vertices, Halfedges, Edges, Face and Solid
    typedef CVertex*    tVertex;
    typedef CHalfEdge*  tHalfEdge;
    typedef CEdge*      tEdge;
    typedef CFace*      tFace;
    typedef CMesh*      tMesh;

    //constructor and destructor
    CMesh();
    ~CMesh();

    //copy operator
    void copy(CMesh& mesh);

    //file io
//    void read(  std::istream & is );
//    void write( std::ostream & os );

    void read_obj(const char* input);
    void write_obj(const char* output);

    void read_m(const char* input);
    void write_m(const char* output);

    //number of vertices, faces, edges
    int  numVertices();
    int  numEdges();
    int  numFaces();

    //is boundary
    bool    isBoundary(tVertex  v);
    bool    isBoundary(tEdge    e);
    bool    isBoundary(tHalfEdge  he);

    //acess vertex - id
    tVertex idVertex(int id);
    int     vertexId(tVertex  v);

    //access face - id
    tFace   idFace(int id);
    int     faceId(tFace  f);

    //access edge - edge key, vertex
    tEdge   vertexEdge(tVertex v0, tVertex v1);

    //access halfedge - halfedge key, vertex
    tHalfEdge   vertexHalfedge(tVertex v0, tVertex v1);
    tHalfEdge   corner(tVertex v, tFace f);

    //halfedge->face
    tFace   halfedgeFace(tHalfEdge he);
    //halfedge->vertex
    tVertex halfedgeVertex(tHalfEdge he);

    //edge->vertex
    tVertex edgeVertex1(tEdge  e);
    tVertex edgeVertex2(tEdge  e);

    //edge->face
    tFace edgeFace1(tEdge  e);
    tFace edgeFace2(tEdge  e);

    //Euler operations
    tHalfEdge vertexMostClwOutHalfEdge(tVertex  v);
    tHalfEdge vertexNextCcwOutHalfEdge(tHalfEdge  he);

    tHalfEdge vertexMostCcwOutHalfEdge(tVertex  v);
    tHalfEdge vertexNextClwOutHalfEdge(tHalfEdge  he);

    tHalfEdge vertexMostClwInHalfEdge(tVertex  v);
    tHalfEdge vertexNextCcwInHalfEdge(tHalfEdge  he);

    tHalfEdge vertexMostCcwInHalfEdge(tVertex  v);
    tHalfEdge vertexNextClwInHalfEdge(tHalfEdge  he);

    tHalfEdge faceMostClwHalfEdge(tFace  f);
    tHalfEdge faceMostCcwHalfEdge(tFace  f);
    tHalfEdge faceNextCcwHalfEdge(tHalfEdge  he);
    tHalfEdge faceNextClwHalfEdge(tHalfEdge  he);


    double edgeLength(tEdge e);

    std::list<tEdge>   & edges()        {
        return m_edges;
    };
    std::list<tFace>   & faces()        {
        return m_faces;
    };
    std::list<tVertex> & vertices() {
        return m_verts;
    };

public:
    CVertex* get_vertex(int id);

public:
    std::list<tEdge>                    m_edges;
    std::list<tVertex>                  m_verts;
    std::list<tFace>                    m_faces;

    //maps
    std::map<int, tVertex>              m_map_vert;
    std::map<int, tFace>                m_map_face;
    std::map<CEdgeKey, tEdge>           m_map_edge;

public:
    tVertex   createVertex(int id = -1);            
    tEdge     createEdge(tVertex v1, tVertex v2);   

    // how to guarantee the right direction?
    tFace     createFace(CVertex* v[], int id = -1);      //create a triangle. it will invoke createEdge()
    tFace     createFace(CVertex* v0, CVertex* v1, CVertex* v2, int id = -1);      //create a triangle. it will invoke createEdge()

public:
    bool removeEdge(tVertex vertex0, tVertex vertex1);
    bool removeEdge(CEdge* edge);
    bool removeFace(tVertex vertex0, tVertex vertex1, tVertex vertex2);
    bool removeFace(tEdge edge0, tEdge edge1, tEdge edge2);
    bool removeFace(CFace *face);

    friend class CWMesh;
};

}//name space MeshLib

#endif //_MESHLIB_MESH_H_ defined
