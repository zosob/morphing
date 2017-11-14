#include "../Mesh/HalfEdge.h"
#include "../Mesh/Vertex.h"
#include "../Mesh/Boundary.h"
#include "../commoninclude/commoninclude.h"
#include "../commoninclude/stringfuncs.h"
#include "../harmonic_map/harmonic_map.h"

using namespace MeshLib;
using namespace zoso;
using namespace std;


int main(int argc, char *argv[])
{
	string meshfile1, meshfile2;
	meshfile1 = "C:\\Users\\kalya\\Downloads\\data set 1\\data set 1\\sad.m";
	meshfile2 = "C:\\Users\\kalya\\Downloads\\data set 1\\data set 1\\smile.m";
    CMesh mesh1, mesh2;
    mesh1.read_m("C:\\Users\\kalya\\Downloads\\data set 1\\data set 1\\sad.m");
    mesh2.read_m("C:\\Users\\kalya\\Downloads\\data set 1\\data set 1\\smile.m");

    // map from mesh2 to mesh1
    map<int, int> fixed_pair;

    morph_map(mesh1, mesh2, fixed_pair);

	string dest_m = "C:\\Users\\kalya\\Downloads\\data set 1\\data set 1\\sad_remapped.m";
    mesh2.write_m(dest_m.c_str());

    cout << "After merging the files" << endl;
    merge(dest_m.c_str(), meshfile2.c_str());
    cout << "Morphing complete. Please press Enter to exit." << endl;
    getchar();
    return 0;
}