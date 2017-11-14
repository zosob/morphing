#include "../Mesh/HalfEdge.h"
#include "../Mesh/Vertex.h"
#include "../Mesh/Boundary.h"
#include "../commoninclude/commoninclude.h"
using namespace MeshLib;
using namespace std;

set<vector<CHalfEdge*>> boundary_hes;
set<vector<CVertex*>> boundary_vertices;

//need to be optimized
bool checkpicked(CHalfEdge *pHe)
{
    set<vector<CHalfEdge*>>::const_iterator it_set = boundary_hes.begin();
    while (it_set != boundary_hes.end())
    {
        vector<CHalfEdge*>::const_iterator it_he = (*it_set).begin();
        while (it_he != (*it_set).end())
        {
            if (*it_he == pHe)
            {
                return true;
            }

            it_he++;
        }

        it_set++;
    }

    return false;
}

void findboundary(CHalfEdge *pHe)
{
    CHalfEdge *pDummy = pHe->ccw_rotate_about_target();
    //locate a he on boundary
    while (pHe->ccw_rotate_about_target())
    {
        pHe = pHe->ccw_rotate_about_target();
        if (pHe == pDummy)
        {
            //already rotated for a full cicle
            //not on boundaries
            return;
        }
    }

    //check if it has already been picked
    //this is the function which needs to be optimized
    if (checkpicked(pHe))
        return;

    vector<CHalfEdge*> hes;
    vector<CVertex*> vertices;
    cout << endl << "--- hole " << boundary_vertices.size() + 1 << " has been located. ---" << endl;

    //pick up the first he & it's target vertex on the boundary
    hes.push_back(pHe);
    vertices.push_back(pHe->target());
    cout << "vertex " << vertices.size() << " from hole " << boundary_hes.size() + 1 <<
        " has already been picked" << endl << "  id: " << pHe->target()->id() << endl;

    //follow the boundary, pick the rest
    pDummy = pHe;
    pHe = pHe->target()->most_clw_out_halfedge();
    while (pDummy != pHe)
    {
        cout << "vertex " << vertices.size() << " from hole " << boundary_hes.size() + 1 <<
            " has already been picked" << endl << "  id: " << pHe->target()->id() << endl;

        hes.push_back(pHe);
        vertices.push_back(pHe->target());
        //the next he following this one and on the boundary
        pHe = pHe->target()->most_clw_out_halfedge();
    }

    boundary_hes.insert(hes);
    boundary_vertices.insert(vertices);

    cout << endl << endl;
    return;
}