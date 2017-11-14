#pragma warning(disable: 4996)
#include "Vertex.h"
#include "HalfEdge.h"
#include "Edge.h"
#include "Face.h"
#include "Mesh.h"
#include <fstream>
#include <map>
#include <vector>
#include <cstring>

using namespace MeshLib;

//access e->v
CMesh::tVertex CMesh::edgeVertex1(CMesh::tEdge  e)
{
    assert(e->halfedge(0) != NULL);
    return e->halfedge(0)->source();
};

//access e->v
CMesh::tVertex CMesh::edgeVertex2(CMesh::tEdge  e)
{
    assert(e->halfedge(0) != NULL);
    return e->halfedge(0)->target();
};

//access e->f
CMesh::tFace CMesh::edgeFace1(CMesh::tEdge  e)
{
    assert(e->halfedge(0) != NULL);
    return e->halfedge(0)->face();
};

//access e->f
CMesh::tFace CMesh::edgeFace2(CMesh::tEdge  e)
{
    assert(e->halfedge(1) != NULL);
    return e->halfedge(1)->face();
};

//access he->f
CMesh::tFace CMesh::halfedgeFace(CMesh::tHalfEdge  he)
{
    return he->face();
};


//access he->v
CMesh::tVertex CMesh::halfedgeVertex(CMesh::tHalfEdge  he)
{
    return he->vertex();
};

bool CMesh::isBoundary(CMesh::tVertex  v)
{
    return v->boundary();
};

bool CMesh::isBoundary(CMesh::tEdge  e)
{
    if(e->halfedge(0) == NULL || e->halfedge(1) == NULL) return true;
    return false;
};

bool CMesh::isBoundary(CMesh::tHalfEdge  he)
{
    if(he->he_sym() == NULL) return true;
    return false;
};

int CMesh::numVertices()
{
    return (int) m_verts.size();
};

int CMesh::numEdges()
{
    return (int) m_edges.size();
};

int CMesh::numFaces()
{
    return (int) m_faces.size();
};


//Euler operation

CMesh::tHalfEdge CMesh::vertexMostClwOutHalfEdge(CMesh::tVertex  v)
{
    return v->most_clw_out_halfedge();
};

CMesh::tHalfEdge CMesh::vertexMostCcwOutHalfEdge(CMesh::tVertex  v)
{
    return v->most_ccw_out_halfedge();
};

CMesh::tHalfEdge CMesh::corner(tVertex v, tFace f)
{
    CMesh::tHalfEdge he = f->halfedge();
    do {
        if(he->vertex() == v)
            return he;
        he = he->he_next();
    } while(he != f->halfedge());
    return NULL;
};

CMesh::tHalfEdge CMesh::vertexNextCcwOutHalfEdge(CMesh::tHalfEdge  he)
{
    return he->ccw_rotate_about_source();
};

CMesh::tHalfEdge CMesh::vertexNextClwOutHalfEdge(CMesh::tHalfEdge  he)
{
    assert(he->he_sym() != NULL);
    return he->clw_rotate_about_source();
};

CMesh::tHalfEdge CMesh::vertexMostClwInHalfEdge(CMesh::tVertex  v)
{
    return v->most_clw_in_halfedge();
};

CMesh::tHalfEdge CMesh::vertexMostCcwInHalfEdge(CMesh::tVertex  v)
{
    return v->most_ccw_in_halfedge();
};

CMesh::tHalfEdge CMesh::vertexNextCcwInHalfEdge(CMesh::tHalfEdge  he)
{
    assert(he->he_sym() != NULL);
    return he->ccw_rotate_about_target();
};

CMesh::tHalfEdge CMesh::vertexNextClwInHalfEdge(CMesh::tHalfEdge  he)
{
    return he->clw_rotate_about_target();
};

CMesh::tHalfEdge CMesh::faceNextClwHalfEdge(CMesh::tHalfEdge  he)
{
    return he->he_prev();
};

CMesh::tHalfEdge CMesh::faceNextCcwHalfEdge(CMesh::tHalfEdge  he)
{
    return he->he_next();
};

CMesh::tHalfEdge CMesh::faceMostCcwHalfEdge(CMesh::tFace  face)
{
    return face->halfedge();
};

CMesh::tHalfEdge CMesh::faceMostClwHalfEdge(CMesh::tFace  face)
{
    return face->halfedge()->he_next();
};


CMesh::CMesh()
{
};

CMesh::~CMesh()
{
    //remove vertices

    for(std::list<CVertex*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter ++)
    {
        CVertex* pV = *viter;
        delete pV;
    }
    m_verts.clear();

    //remove faces

    for(std::list<CFace*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter ++)
    {
        CFace* pF = *fiter;

        tHalfEdge he = pF->halfedge();

        std::list<CHalfEdge*> hes;
        do {
            he = he->he_next();
            hes.push_back(he);
        } while(he != pF->halfedge());

        for(std::list<CHalfEdge*>::iterator hiter = hes.begin(); hiter != hes.end(); hiter ++)
        {
            CHalfEdge* pH = *hiter;
            delete pH;
        }
        hes.clear();

        delete pF;
    }
    m_faces.clear();

    //remove edges
    for(std::list<CEdge*>::iterator eiter = m_edges.begin(); eiter != m_edges.end(); eiter ++)
    {
        CEdge* pE = *eiter;
        delete pE;
    }

    m_edges.clear();

    m_map_vert.clear();
    m_map_face.clear();
    m_map_edge.clear();
};



double CMesh::edgeLength(CMesh::tEdge e)
{
    CVertex* v1 = edgeVertex1(e);
    CVertex* v2 = edgeVertex2(e);

    return (v1->point() - v2->point()).norm();
}


CMesh::tVertex CMesh::createVertex(int id)
{
    tVertex v = new CVertex();
    assert(v != NULL);
    if (id < 0)
    {
        if (m_map_vert.size() == 0)
        {
            // first vertex
            id = 0;
        }
        else
        {
            // get the maximun vertex id, then plus 1
            std::map<int, CVertex*>::iterator it_map = m_map_vert.end();
            id = (--it_map)->first + 1;
        }
    }

    v->id() = id;
    m_verts.push_back(v);
    m_map_vert.insert(std::pair<int,CVertex*>(id,v));
    return v;//insert a new vertex, with id as the key
};



void CMesh::read_obj(const char* filename)
{
//  TRACE("load obj file %s\n",filename);

    FILE* f = fopen(filename, "r");
    if(f == NULL) return;

    char cmd[1024];
    char seps[] = " /,\t\n";
    int  vid = 1;
    int  fid = 1;


    while(true)
    {
        if(fgets(cmd, 1024, f) == NULL)
            break;

        char* token = strtok(cmd, seps);

        if(token == NULL)
            continue;


        if(strcmp(token, "v") == 0)
        {
            CPoint p;
            for(int i = 0; i < 3; i ++)
            {
                token = strtok(NULL, seps);
                p[i] = atof(token);
            }

            CVertex* v = createVertex(vid);
            v->point() = p;
            vid ++;
            continue;
        }

        if(strcmp(token, "vt") == 0)
        {
            CPoint p;
            for(int i = 0; i < 2; i ++)
            {
                token = strtok(NULL, seps);
                p[i] = atof(token);
            }
            continue;
        }

        if(strcmp(token, "vn") == 0)
        {
            CPoint p;
            for(int i = 0; i < 3; i ++)
            {
                token = strtok(NULL, seps);
                p[i] = atof(token);
            }
            continue;
        }




        if(strcmp(token, "f") == 0)
        {
            CVertex* v[3];
            for(int i = 0 ; i < 3; i ++)
            {
                token  = strtok(NULL, seps);
                int id  = atoi(token);
                v[i] = m_map_vert[id];
            }
            createFace(v, fid++);
        }
    }
    fclose(f);

    //Label boundary edges
    for(std::list<CEdge*>::iterator eiter= m_edges.begin() ; eiter != m_edges.end() ; ++ eiter)
    {
        CMesh::tEdge     edge = *eiter;
        CMesh::tHalfEdge he[2];

        he[0] = edge->halfedge(0);
        he[1] = edge->halfedge(1);

        assert(he[0] != NULL);


        if(he[1] != NULL)
        {
            assert(he[0]->target() == he[1]->source() && he[0]->source() == he[1]->target());

            if(he[0]->target()->id() < he[0]->source()->id())
            {
                edge->halfedge(0) = he[1];
                edge->halfedge(1) = he[0];
            }

            assert(edgeVertex1(edge)->id() < edgeVertex2(edge)->id());
        }
        else
        {
            he[0]->vertex()->boundary() = true;
            he[0]->he_prev()->vertex()->boundary()  = true;
        }

    }

    std::list<CVertex*> dangling_verts;
    //Label boundary edges
    for(std::list<CVertex*>::iterator viter = m_verts.begin();  viter != m_verts.end() ; ++ viter)
    {
        CMesh::tVertex     v = *viter;
        if(v->halfedge() != NULL) continue;
        dangling_verts.push_back(v);
    }

    for(std::list<CVertex*>::iterator  viter = dangling_verts.begin() ; viter != dangling_verts.end(); ++ viter)
    {
        CMesh::tVertex v = *viter;
        m_verts.remove(v);
        delete v;
        v = NULL;
    }

    //Arrange the boundary half_edge of boundary vertices, to make its halfedge
    //to be the most ccw in half_edge

    for(std::list<CVertex*>::iterator viter = m_verts.begin();  viter != m_verts.end() ; ++ viter)
    {
        CMesh::tVertex     v = *viter;
        if(!v->boundary()) continue;

        CHalfEdge* he = v->halfedge();
        while(he->he_sym() != NULL)
        {
            he = he->ccw_rotate_about_target();
        }

        v->halfedge() = he;
    }

}

CFace* CMesh::createFace(CVertex* v0, CVertex* v1, CVertex* v2, int id)
{
    CVertex *v[3];
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    return this->createFace(v, id);
}

CMesh::tFace CMesh::createFace(CVertex* v[] , int id)
{
    tFace f = new CFace();
    assert(f != NULL);

    if (id < 0)
    {
        if (m_map_face.size() == 0)
        {
            // first one
            id = 0;
        }
        else
        {
            // get the maximun id, then plus 1
            std::map<int, CFace*>::iterator it_map = m_map_face.end();
            id = (--it_map)->first + 1;
        }
    }

    f->id() = id;
    m_faces.push_back(f);
    m_map_face.insert(std::pair<int,tFace>(id,f));

    //create halfedges
    tHalfEdge hes[3];

    for(int i = 0; i < 3; i ++)
    {
        hes[i] = new CHalfEdge;
        assert(hes[i]);
        CVertex* vert =  v[i];
        hes[i]->vertex() = vert;
        vert->halfedge() = hes[i];
    }

    //linking to each other
    for(int i = 0; i < 3; i ++)
    {
        hes[i]->he_next() = hes[(i+1)%3];
        hes[i]->he_prev() = hes[(i+2)%3];
    }

    //linking to face
    for(int i = 0; i < 3; i ++)
    {
        hes[i]->face()   = f;
        f->halfedge()    = hes[i];
    }

    //connecting with edge
    for(int i = 0; i < 3; i ++)
    {
        tEdge e = createEdge(v[i], v[(i+2)%3]);
        if(e->halfedge(0)  == NULL)
        {
            e->halfedge(0) = hes[i];
        }
        else
        {
            assert(e->halfedge(1) == NULL);
            e->halfedge(1) = hes[i];
        }
        hes[i]->edge() = e;
    }

    return f;
};

bool erase_halfedge_for_edge(CHalfEdge *halfedge)
{
    CEdge *edge = halfedge->edge();
    if (halfedge->edge()->halfedge(0) == halfedge)
    {
        delete edge->halfedge(0);
        edge->halfedge(0) = NULL;
    }
    else
    {
        delete edge->halfedge(1);
        edge->halfedge(1) = NULL;
    }

    if (!edge->halfedge(0) && !edge->halfedge(1))
    {
        // need to delete the edge
        return true;
    }

    return false;
}

bool CMesh::removeFace(CFace *face)
{
    if (!face)
        return false;

    // delete the three halfedges for the three corresponding edges
    CHalfEdge *halfedge0 = face->get_halfedge();
    CHalfEdge *halfedge1 = halfedge0->he_next();
    CHalfEdge *halfedge2 = halfedge0->he_prev();
    CVertex *he0_source = halfedge0->source();
    CVertex *he0_target = halfedge0->target();
    CVertex *he1_source = halfedge1->source();
    CVertex *he1_target = halfedge1->target();
    CVertex *he2_source = halfedge2->source();
    CVertex *he2_target = halfedge2->target();
    bool flag0 = erase_halfedge_for_edge(halfedge0);
    bool flag1 = erase_halfedge_for_edge(halfedge1);
    bool flag2 = erase_halfedge_for_edge(halfedge2);
    if (flag0)
    {
        this->removeEdge(he0_source, he0_target);
    }

    if (flag1)
    {
        this->removeEdge(he1_source, he1_target);
    }

    if (flag2)
    {
        this->removeEdge(he2_source, he2_target);
    }

    // remove the face from the list
    std::list<tFace>::iterator it_faces = m_faces.begin();
    while (it_faces != m_faces.end())
    {
        if (*it_faces == face)
        {
            m_faces.erase(it_faces);
            break;
        }
        it_faces++;
    }

    // remove the face from the map
    m_map_face.erase(face->id());

    delete face;
    return true;
}

bool CMesh::removeFace(tVertex vertex0, tVertex vertex1, tVertex vertex2)
{
    std::list<tFace>::iterator it_faces = m_faces.begin();
    while (it_faces == m_faces.end())
    {
        CFace *face = *it_faces;
        CHalfEdge *halfedge0 = face->get_halfedge();
        CHalfEdge *halfedge1 = halfedge0->he_next();
        CHalfEdge *halfedge2 = halfedge0->he_prev();

        bool flag = false;

        CVertex *vertex = halfedge0->target();
        if (vertex == vertex0 || vertex == vertex1 || vertex == vertex2)
        {
            CVertex *vertex = halfedge1->target();
            if (vertex == vertex0 || vertex == vertex1 || vertex == vertex2)
            {
                CVertex *vertex = halfedge2->target();
                if (vertex == vertex0 || vertex == vertex1 || vertex == vertex2)
                {
                    flag = true;
                }
            }
        }

        if (flag)
        {
            return this->removeFace(halfedge0->edge(), halfedge1->edge(), halfedge2->edge());
        }

        it_faces++;
    }

    return false;
}

void pushback_faces_from_edge(CEdge *edge, std::vector<CFace*> &faces)
{
    if (edge->halfedge(0))
    {
        faces.push_back(edge->get_halfedge(0)->face());
    }

    if (edge->halfedge(1))
    {
        faces.push_back(edge->get_halfedge(1)->face());
    }
}

bool CMesh::removeFace(tEdge edge0, tEdge edge1, tEdge edge2)
{
    std::vector<CFace*> faces;
    pushback_faces_from_edge(edge0, faces);
    pushback_faces_from_edge(edge1, faces);
    pushback_faces_from_edge(edge2, faces);
    std::vector<CFace*>::iterator it_faces_a = faces.begin();
    while (it_faces_a != faces.end())
    {
        int equal_count = 0;
        // find any equal faces
        std::vector<CFace*>::iterator it_faces_b = faces.begin();
        while (it_faces_b != faces.end())
        {
            if (it_faces_a != it_faces_b && *it_faces_a == *it_faces_b)
            {
                equal_count++;
            }
            it_faces_b++;
        }

        if (equal_count == 3)
        {
            return this->removeFace(*it_faces_a);
        }

        it_faces_a++;
    }

    return false;
}

//access id->v
CMesh::tVertex CMesh::idVertex(int id)
{
    return m_map_vert[id];
};

//access v->id
int CMesh::vertexId(CMesh::tVertex  v)
{
    return v->id();
};

//access id->f
CMesh::tFace CMesh::idFace(int id)
{
    return m_map_face[id];
};

//acess f->id
int CMesh::faceId(CMesh::tFace  f)
{
    return f->id();
};

CMesh::tEdge CMesh::createEdge(CMesh::tVertex v1, CMesh::tVertex v2)
{
    CEdgeKey key(v1,v2);

    tEdge e = NULL;

    if(m_map_edge.find(key) != m_map_edge.end())
    {
        e = m_map_edge[key];
        return e;
    }

    e = new CEdge;
    m_map_edge.insert(std::pair<CEdgeKey, CMesh::tEdge>(key,e));
    m_edges.push_back(e);

    return e;

};

bool CMesh::removeEdge(tVertex vertex0, tVertex vertex1)
{
    // first£¬get the edge through edgekey(v1, v2)
    CEdgeKey key(vertex0, vertex1);
    std::map<CEdgeKey, tEdge>::iterator it_map_edge = m_map_edge.find(key);
    if (it_map_edge == m_map_edge.end())
        return false;

    CEdge *edge = m_map_edge[key];

    // then, erase the edge in the list
    std::list<tEdge>::iterator it_edges = m_edges.begin();
    while (it_edges != m_edges.end())
    {
        if (*it_edges == edge)
        {
            m_edges.erase(it_edges);
            break;
        }
        it_edges++;
    }

    // remove the two(or one) he
    if (edge->halfedge(0))
    {
        delete edge->halfedge(0);
    }

    if (edge->halfedge(1))
    {
        delete edge->halfedge(1);
    }

    m_map_edge.erase(key);
    delete edge;
    return true;
}

/*
bool CMesh::removeEdge(CEdge* edge)
{
    CHalfEdge *he = edge->get_halfedge(0);
    if (!he)
    {
        he = edge->get_halfedge(1);
    }

    if (he)
    {
        return this->removeEdge(he->vertex(), he->source());
    }

    return false;
}
*/

//access vertex->edge
CMesh::tEdge CMesh::vertexEdge(CMesh::tVertex v0, CMesh::tVertex v1)
{
    CEdgeKey key(v0,v1);
    return m_map_edge[key];
};



//access vertex->edge
CMesh::tHalfEdge CMesh::vertexHalfedge(tVertex v0, tVertex v1)
{
    tEdge e = vertexEdge(v0, v1);
    assert(e != NULL);
    tHalfEdge he = e->halfedge(0);
    if(he->vertex() == v1 && he->he_prev()->vertex() == v0) return he;
    he = e->halfedge(1);
    assert(he->vertex() == v1 && he->he_prev()->vertex() == v0);
    return he;
};



bool get_normal_from(char *line, CPoint &normal)
{
    char *p = strtok(line, "{");
    if (!p)
        return false;

    p = strtok(NULL, "{");  // after "{"

    p = strtok(p, " ");     // first " "
    // find the "normal"
    while (p && p[0] != 'n')
    {
        p = strtok(NULL, " ");
    }

    if (!p)
        return false;

    // now separate is " ", and start from "normal=("
    while ((*p < '0' || *p > '9') && *p != '-')
    {
        p++;
    }
    normal.v[0] = atof(p);
    p = strtok(NULL, " ");
    normal.v[1] = atof(p);
    p = strtok(NULL, " ");
    normal.v[2] = atof(p);
    return true;
}


bool get_rgb_from(char *line, CPoint &rgb)
{
    char *p = strtok(line, "{");
    if (!p)
        return false;

    p = strtok(NULL, "{");  // after "{"

    p = strtok(p, " ");     // first " "
    // find the "rgb"
    while (p && p[0] != 'r')
    {
        p = strtok(NULL, " ");
    }

    if (!p)
        return false;

    // now separate is " ", and start from "rgb=("
    while ((*p < '0' || *p > '9') && *p != '-')
    {
        p++;
    }
    rgb.v[0] = atof(p);
    p = strtok(NULL, " ");
    rgb.v[1] = atof(p);
    p = strtok(NULL, " ");
    rgb.v[2] = atof(p);
    return true;
}


void CMesh::read_m(const char* input)
{
    FILE* is = fopen(input, "r");
    if(is == NULL)
    {
        fprintf(stderr,"Error is opening file %s\n", input);
        return;
    }

    char line[MAX_LINE];
    int id;

    while(!feof(is))
    {
        if(fgets(line, MAX_LINE, is) == NULL) break;
        if(strlen(line)< 4) break;

        // get normal and rgb from this line, if they are existed
        std::string strline = line;
        CPoint normal;
        bool enable_normal = get_normal_from((char*)strline.c_str(), normal);
        strline = line;
        CPoint rgb;
        bool enable_rgb = get_rgb_from((char*)strline.c_str(), rgb);

        char* str = strtok(line, " \r\n");
        if(strcmp(str, "Vertex") == 0)
        {
            str = strtok(NULL, " \r\n");
            id = atoi(str);
            CPoint p;
            for(int i = 0 ; i < 3; i ++)
            {
                str = strtok(NULL," \r\n");
                p[i] = atof(str);
            }

            tVertex v  = createVertex(id);
            v->point() = p;
            v->normal() = normal;
            v->rgb() = rgb;
            v->id() = id;
            v->enable_normal = enable_normal;
            v->enable_rgb = enable_rgb;

            str = strtok(NULL, "\r\n");
            if(str == NULL) continue;

            std::string s(str);
            int sp = (int) s.find("{");
            int ep = (int) s.find("}");

            if(sp >= 0 && ep >= 0)
            {
//                v->string() = s.substr(sp+1, ep-sp-1);
            }
            continue;
        }


        if(strcmp(str,"Face") == 0)
        {
            str = strtok(NULL, " \r\n");
            id = atoi(str);

            CVertex* v[3];
            for(int i = 0; i < 3; i ++)
            {
                str = strtok(NULL," \r\n");
                v[i] = idVertex(atoi(str));
            }

            tFace f = createFace(v, id);

            str = strtok(NULL, "\r\n");
            if(str == NULL)       continue;

            std::string s(str);

            int sp = (int) s.find("{");
            int ep = (int) s.find("}");

            if(sp >= 0 && ep >= 0)
            {
                f->string() = s.substr(sp+1, ep-sp-1);
            }
            continue;
        }

        //read in edge attributes
        if(strcmp(str,"Edge")==0)
        {
            str = strtok(NULL, " \r\n");
            int id0 = atoi(str);

            str = strtok(NULL, " \r\n");
            int id1 = atoi(str);

            CVertex* v0 = idVertex(id0);
            CVertex* v1 = idVertex(id1);

            tEdge edge = vertexEdge(v0, v1);

            str = strtok(NULL, "\r\n");

            std::string s(str);
            int sp = (int) s.find("{");
            int ep = (int) s.find("}");

            if(sp >= 0 && ep >= 0)
            {
                edge->string() = s.substr(sp+1, ep-sp-1);
            }
            continue;
        }

        //read in edge attributes
        if(strcmp(str,"Corner")==0)
        {
            str = strtok(NULL," \r\n");
            int id0 = atoi(str);

            str = strtok(NULL," \r\n");
            int id1 = atoi(str);


            CVertex* v = idVertex(id0);
            CFace*    f = idFace(id1);
            tHalfEdge he = corner(v, f);

            str = strtok(NULL,"\r\n");
            std::string s(str);

            int sp = (int) s.find("{");
            int ep = (int) s.find("}");

            if(sp >= 0 && ep >= 0)
            {
                he->string() = s.substr(sp+1, ep-sp-1);
            }
            continue;
        }
    }

    //Label boundary edges
    for(std::list<CEdge*>::iterator eiter= m_edges.begin() ; eiter != m_edges.end() ; ++ eiter)
    {
        CMesh::tEdge     edge = *eiter;
        CMesh::tHalfEdge he[2];

        he[0] = edge->halfedge(0);
        he[1] = edge->halfedge(1);

        assert(he[0] != NULL);

        if(he[1] != NULL)
        {
            assert(he[0]->target() == he[1]->source() && he[0]->source() == he[1]->target());

            if(he[0]->target()->id() < he[0]->source()->id())
            {
                edge->halfedge(0) = he[1];
                edge->halfedge(1) = he[0];
            }

            assert(edgeVertex1(edge)->id() < edgeVertex2(edge)->id());
        }
        else
        {
            he[0]->vertex()->boundary() = true;
            he[0]->he_prev()->vertex()->boundary()  = true;
        }
    }

    std::list<CVertex*> dangling_verts;
    //Label boundary edges
    for(std::list<CVertex*>::iterator viter = m_verts.begin();  viter != m_verts.end() ; ++ viter)
    {
        CMesh::tVertex     v = *viter;
        if(v->halfedge() != NULL) continue;
        dangling_verts.push_back(v);
    }

    for(std::list<CVertex*>::iterator  viter = dangling_verts.begin() ; viter != dangling_verts.end(); ++ viter)
    {
        CMesh::tVertex v = *viter;
        m_verts.remove(v);
        delete v;
        v = NULL;
    }

    //Arrange the boundary half_edge of boundary vertices, to make its halfedge
    //to be the most ccw in half_edge

    for(std::list<CVertex*>::iterator viter = m_verts.begin();  viter != m_verts.end() ; ++ viter)
    {
        CMesh::tVertex     v = *viter;
        if(!v->boundary()) continue;

        CHalfEdge* he = v->halfedge();
        while(he->he_sym() != NULL)
        {
            he = he->ccw_rotate_about_target();
        }
        v->halfedge() = he;
    }
};


std::string FloatToStr(const double f)
{
    static char cBuff[20];
    ::sprintf(cBuff, "%f", f);
    return std::string(cBuff);
}


void CMesh::write_m(const char* output)
{
    FILE* _os = fopen(output,"w");
    assert(_os);

    //remove vertices
    for(std::list<CVertex*>::iterator viter = m_verts.begin(); viter != m_verts.end(); viter ++)
    {
        tVertex v = *viter;

        fprintf(_os,"Vertex %d ", v->id());

        for(int i = 0; i < 3; i ++)
        {
            fprintf(_os,"%g ",v->point()[i]);
        }

/*
        if(v->string().size() > 0)
        {
            fprintf(_os,"{%s}", v->string().c_str());
        }
*/
        std::string rgb_string, normal_string, ext;
        if (v->enable_rgb || v->enable_normal)
        {
            if (v->enable_rgb)
            {
                rgb_string = "rgb=(" + FloatToStr(v->rgb().v[0]) + " "
                    + FloatToStr(v->rgb().v[1]) + " "
                    + FloatToStr(v->rgb().v[2]) + ")";
            }

            if (v->enable_normal)
            {
                normal_string = "normal=(" + FloatToStr(v->normal().v[0]) + " "
                    + FloatToStr(v->normal().v[1]) + " "
                    + FloatToStr(v->normal().v[2]) + ")";
            }

            if (v->enable_rgb)
            {
                ext = "{" + rgb_string;
                if (v->enable_normal)
                {
                    ext = ext + " " + normal_string + "}";
                }
                else
                {
                    ext = ext + "}";
                }
            }
            else
            {
                ext = "{" + normal_string + "}";
            }

            fprintf(_os, ext.c_str());
        }

        fprintf(_os,"\n");
    }

    for(std::list<CFace*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter ++)
    {
        tFace f = *fiter;
        fprintf(_os,"Face %d",f->id());

        tHalfEdge he = f->halfedge();
        do {
            fprintf(_os," %d", he->target()->id());
            he = he->he_next();
        } while(he != f->halfedge());

        if(f->string().size() > 0)
        {
            fprintf(_os,"{%s}", f->string().c_str());
        }

        fprintf(_os,"\n");
    }

    for(std::list<CEdge*>::iterator eiter = m_edges.begin(); eiter != m_edges.end(); eiter ++)
    {
        tEdge e = *eiter;
        if(e->string().size() > 0)
        {
            fprintf(_os,"Edge %d %d {%s} \n", edgeVertex1(e)->id(), edgeVertex2(e)->id(), e->string().c_str());
        }
    }

    for(std::list<CFace*>::iterator fiter = m_faces.begin(); fiter != m_faces.end(); fiter ++)
    {
        tFace f = *fiter;

        tHalfEdge he = f->halfedge();

        do {
            if(he->string().size() > 0)
            {
                fprintf(_os,"Corner %d %d {%s}\n", he->vertex()->id(), f->id(), he->string().c_str());
            }
            he = he->he_next();
        } while(he != f->halfedge());

    }

    fclose(_os);
};

CVertex* CMesh::get_vertex(int id)
{
    std::map<int, tVertex>::iterator it = m_map_vert.find(id);
    if (it == m_map_vert.end())
        return NULL;

    return it->second;
}