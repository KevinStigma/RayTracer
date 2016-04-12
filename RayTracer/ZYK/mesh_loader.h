#ifndef MESH_LOADER
#define MESH_LOADER
#include "mathematics.h"
namespace zyk
{
	typedef struct tag_POLY
	{
		tag_POLY()
		{
			vlist=NULL;
			state=0;
		}
		int state;    // state information
		int attr;     // physical attributes of polygon
		Vec4 color;    // color of polygon

		Vertex * vlist; // the vertex list itself
		int vert[3];       // the indices into the vertex list
		Vec4 local_normal;
		Vec4 trans_normal;
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	}Facet,*Facet_Ptr;
	
	typedef struct tag_ObjectMesh
	{
		tag_ObjectMesh()
		{
			name="nothing";
			num_vertices=0;
			num_polys=0;
			max_radius=0;
			//local_center=Vec4(0,0,0,1);
			state=0;
		}
		int cull_object(const Camera& cam,int cull_flags);
		void reset_object()
		{
			state=0;
			for(int i=0;i<num_polys;i++)
				plist[i].state=0;
		}
		void computeRadius();
		void centralize();
		void computeNormal();
		int  id;           // numeric id of this object
		std::string name;     // ASCII name of object just for kicks
		int  state;        // state of object
		int  attr;         // attributes of object
		float avg_radius;  // average radius of object used for collision detection
		float max_radius;  // maximum radius of object

		Vec3 world_pos;  // position of object in world

		Vec3 dir;       // rotation angles of object in local
		// cords or unit direction vector user defined???

		//Vec4 local_center;
		Vec3 ux,uy,uz;  // local axes to track full orientation
		// this is updated automatically during
		// rotation calls

		int num_vertices;   // number of vertices of this object
		Vertex vert[OBJECT4DV1_MAX_VERTICES];
		Vec4 world_center;

		int num_polys;        // number of polygons in object mesh
		Facet plist[OBJECT4DV1_MAX_POLYS]; // array of polygony
		Material* material;
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	}ObjectMesh,*ObjectPtr;


	int Load_OBJECT4DV1_PLG(ObjectMesh& obj, char *filename,const Vec3& scale,    
		const Vec3& pos, const Vec4& rot);
	int Load_OBJECT4DV1_COB(ObjectMesh& obj,char *filename,const Vec3& scale,const Vec3& pos,
		const Vec4& rot,int vertex_flags=0);
	int Load_OBJECT4DV1_OBJ(ObjectMesh& obj,char *filename,const Vec3& scale,const Vec3& pos,
		const Vec4& rot,int vertex_flags=0);
}
#endif