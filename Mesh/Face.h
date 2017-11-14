#ifndef _MESHLIB_FACE_H_
#define _MESHLIB_FACE_H_

#include <assert.h>
#include <string>


namespace MeshLib {


class CHalfEdge;

class CFace
{
    public:

        CFace() {
            m_halfedge = NULL;
            m_mark = false;
        };
        ~CFace() {};

        CHalfEdge*& halfedge() {
            return m_halfedge;
        };

        CHalfEdge* get_halfedge() {
            return m_halfedge;
        };

        int& id() {
            return m_id;
        };

        const int id() const {
            return m_id;
        };

        std::string& string()     {
            return m_string;
        };
        

    private:
        int         m_id;
        CHalfEdge*  m_halfedge;
        std::string m_string;

    public:
        bool        m_mark;

    private:    //disable
        inline CFace& operator=(const CFace& rhs);
        CFace(const CFace& uv);
};


}//name space MeshLib

#endif //_MESHLIB_FACE_H_ defined