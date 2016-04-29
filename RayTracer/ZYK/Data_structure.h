#ifndef SOFT_RENDER_DATASTRUCTURE
#define SOFT_RENDER_DATASTRUCTURE

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if(p){delete (p);(p)=NULL;}}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) {if(p){delete[](p);(p)=NULL;}}
#endif

#define OBJECT4DV1_MAX_VERTICES 5000
#define OBJECT4DV1_MAX_POLYS 10000
#define RENDERLIST_MAX_POLYS 10000

#define INV_2 0.5;
#define INV_3 0.333333
#define INV_PI 0.3183098

#define RAD_TO_DEG 180.0/3.14159265
#define DEG_TO_RAD 3.14159265/180.0

#define EPSILON 1e-3

#define FCMP(a,b) ((fabs((a)-(b))<EPSILON)?1:0)
#define SWAP(a,b,c) {(c)=(a);(a)=(b);(b)=(c);} 

#define CULL_OBJECT_X_PLANE 0x0001
#define CULL_OBJECT_Y_PLANE 0x0002
#define CULL_OBJECT_Z_PLANE 0x0004
#define CULL_OBJECT_XYZ_PLANE (CULL_OBJECT_X_PLANE|CULL_OBJECT_Y_PLANE\
								|CULL_OBJECT_Z_PLANE)

#define MEMORY_LEAK_CHECK _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|\
_CRTDBG_LEAK_CHECK_DF);

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <QPainter>
#include <Eigen/geometry>
#include <Eigen/dense>
#include <eigen\Core>
#include <eigen\Svd>
#include <eigen\Sparse>
#include "Image.h"

using namespace Eigen;
using namespace Eigen::internal;
using namespace Eigen::Architecture;
//typedef Matrix<float,3,1> Vec3;
typedef Vector2f Vec2;
typedef Vector3f Vec3;
typedef Vector4f Vec4;
typedef Matrix3f Mat3;
typedef Matrix4f Mat4;
typedef Vec4* PointList;

namespace zyk
{
	enum MaterialType {SOLID,DIELECTRIC,LIGHTSOURCE,MIRROW,ANISOTROPY};
	union UCHAR3
	{
		unsigned char m[3];
		struct
		{
			unsigned char x,y,z;
		};
	};

	union TexCoord
	{
		struct 
		{
			float u,v;
		};
		float m[2]; 
	};

	struct Vertex
	{
		Vec4 vlocal;
		Vec4 vtrans;
		Vec4 local_nor;
		Vec4 trans_nor;
		TexCoord texc;
	};

	typedef struct tag_Mat
	{
		int state;           // state of material
		int id;              // id of this material, index into material array
		std::string name;       // name of material
		int  attr;           // attributes, the modes for shading, constant, flat, 
		// gouraud, fast phong, environment, textured etc.
		// and other special flags...

		UCHAR3 color;            // color of material
		float ka, kd, ks,kr,power; // ambient, diffuse, specular, 
		// coefficients, note they are 
		// separate and scalars since many 
		// modelers use this format
		// along with specular power

		float rough_exponent;
		Vec4 ra, rd, rs;       // the reflectivities/colors pre-
		// multiplied, to more match our 
		// definitions, each is basically
		// computed by multiplying the 
		// color by the k's, eg:
		// rd = color*kd etc.

		std::string texture_file;   // file location of texture
		Image texture;    // actual texture map (if any)
		MaterialType type;
		float rei; //refractive index
		float specular_ratio;

		int   iaux1, iaux2;      // auxiliary vars for future expansion
		float faux1, faux2;
		void *ptr;
		tag_Mat()
		{
			//texture=NULL;
		}

	}Material;
	
	enum Light_type
	{
		DIRECTION_LIGHT,
		SPOT_LIGHT,
		AREA_LIGHT
	};

	class TriMesh;
	typedef struct tag_Light
	{
		tag_Light();
		int state; // state of light
		int id;    // id of light
		Light_type type;  // type of light, and extra qualifiers

		Vec4 c_ambient;   // ambient light intensity
		Vec4 c_diffuse;   // diffuse light intensity
		Vec4 c_specular;  // specular light intensity

		Vec3  pos;       // position of light
		Vec3 dir;       // direction of light
		float kc, kl, kq;   // attenuation factors
		float spot_inner;   // inner angle for spot light
		float spot_outer;   // outer angle for spot light
		float pf;           // power factor/falloff for spot lights

		int   iaux1, iaux2; // auxiliary vars for future expansion
		float faux1, faux2;
		TriMesh* light_source_mesh;
		void getIlluminatinInfo(const Vec3& point,Vec3& light_dir,Vec4& light_intensity,Vec3& light_pos)const;
		Vec3 getLightingDirection(const Vec3& point, Vec3&light_pos)const;
		
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	}Light, *Light_Ptr;

	typedef struct tag_Camera
	{
		void init(float width,float height);
		void reset_camera_matrix();
		void reset_perspective_proj_matrix();
		void reset_viewport_matrix();
		void update_arcball_matrix(const float mat[]);
		bool Out_of_Frustum(const Vec4& vert);
		bool Out_of_Frustum(const Vec3& vert);
		int state;      // state of camera
		int attr;       // camera attributes

		Vec3 pos;    // world position of camera used by both camera models

		Vec3 backup_pos;
		Vec3 dir;   // angles or look at direction of camera for simple

		// euler camera models, elevation and heading for
		// uvn model

		Vec3 u;     // extra vectors to track the camera orientation
		Vec3 v;     // for more complex UVN camera model
		Vec3 n;        

		Vec3 target; // look at target
		Vec3 backup_target;

		float view_dist;   // focal length 
		float view_width;  // width in the view_plane
		float view_height; // height in the view_plane
		Vec4  view_plane;  // first 2 floats are x coordinate of two endpoints, the last are y coord 

		float fov;         

		// 3d clipping planes
		// if view volume is NOT 90 degree then general 3d clipping
		// must be employed
		float near_clip_z;     // near z=constant clipping plane
		float far_clip_z;      // far z=constant clipping plane                   

		// aspect ratio
		float aspect_ratio;

		float viewport_width;
		float viewport_height;

		// these matrices are not necessarily needed based on the method of
		// transformation, for example, a manual perspective or screen transform
		// and or a concatenated perspective/screen, however, having these 
		// matrices give us more flexibility         

		Mat4 mcam;   // storage for the world to camera transform matrix
		Mat4 arcball_trans_cam;
		Mat4 mper;   // storage for the camera to perspective transform matrix
		Mat4 mscr;   // storage for the perspective to screen transform matrix
	}Camera,*CameraPtr;

	typedef struct tag_PolyFace
	{
		tag_PolyFace()
		{
			state=0;
			mat_ptr=NULL;
		}
		int state;
		Vec4 color;
		Vec4 vlist[3];
		Vec4 tlist[3];
		Vec3 v_normal[3];
		Vec4 v_color[3];
		TexCoord texc[3];
		Vec3 normal_camera;
		Material* mat_ptr;
		void draw_lines(QPainter& painter);
		void draw_vertices(QPainter&painter);
	}PolyFace,*PolyFace_Ptr;


	typedef struct tag_RenderList
	{
		tag_RenderList():num_polys(0){}
		void reset(){num_polys=0;}
		PolyFace_Ptr poly_ptrs[RENDERLIST_MAX_POLYS];
		PolyFace plist[RENDERLIST_MAX_POLYS];
		int num_polys;
		void outputPolygon(std::string& filename);
	}RenderList,*RenderList_Ptr;

	inline Vec4 Vector4FAdd_V4(const Vec4& vec1,const Vec4& vec2)
	{
		return Vec4(vec1(0)+vec2(0),vec1(1)+vec2(1),vec1(2)+vec2(2),1);
	}
	inline Vec4 Vector4FSub_V4(const Vec4& vec1,const Vec4& vec2)
	{
		return Vec4(vec1(0)-vec2(0),vec1(1)-vec2(1),vec1(2)-vec2(2),1);
	}
	inline Vec3 Vector4FAdd_V3(const Vec4& vec1,const Vec4& vec2)
	{
		return Vec3(vec1(0)+vec2(0),vec1(1)+vec2(1),vec1(2)+vec2(2));
	}
	inline Vec3 Vector4FSub_V3(const Vec4& vec1,const Vec4& vec2)
	{
		return Vec3(vec1(0)-vec2(0),vec1(1)-vec2(1),vec1(2)-vec2(2));
	}

	inline Vec3 dot_multV3(const Vec3&v1,const Vec3& v2)
	{
		return Vec3(v1(0)*v2(0),v1(1)*v2(1),v1(2)*v2(2));
	}
	inline Vec4 dot_multV4(const Vec4&v1,const Vec4& v2)
	{
		return Vec4(v1(0)*v2(0),v1(1)*v2(1),v1(2)*v2(2),v1(3)*v2(3));
	}
	inline Vec3 transformNormal(const Vec3& normal,const Mat4&mat)
	{
		Vec4 v=mat*Vec4(normal(0),normal(1),normal(2),0);
		return Vec3(v(0),v(1),v(2));
	}
	inline Vec4 transformNormal(const Vec4& normal,const Mat4&mat)
	{
		Vec4 vec=normal;
		vec(3)=0.f;
		return mat*vec;
	}
	inline Vec3 transformVertex(const Vec3& vertex,const Mat4&mat)
	{
		Vec4 v=mat*Vec4(vertex(0),vertex(1),vertex(2),1);
		return Vec3(v(0),v(1),v(2));
	}
	inline Vec4 transformVertex(const Vec4& vertex,const Mat4&mat)
	{
		Vec4 vec=vertex;
		vec(3)=0.f;
		return mat*vec;
	}
	inline Vec3 V4toV3(const Vec4& v)
	{
		return Vec3(v(0),v(1),v(2));
	}
	inline Vec4 V3toV4(const Vec3&v, int set)
	{
		(set==0)?set=0:set=1;
		return Vec4(v(0),v(1),v(2),set);
	}

	inline void clip_0_to_1(float& value)
	{
		if(value<0)
			value=0;
		else if(value>1)
			value=1;
	}
	inline void clip_0_to_1(Vec4& v)
	{
		for(int i=0;i<4;i++)
		{
			if(v(i)<0)
				v(i)=0;
			else if(v(i)>1.f)
				v(i)=1;
		}
	}

	int Draw_Clip_Line(int x0,int y0, int x1, int y1, zyk::UCHAR3 color,zyk::UCHAR3 *dest_buffer, int lpitch);
	int Draw_Line(int x0, int y0,int x1, int y1,zyk::UCHAR3 color,zyk::UCHAR3*vb_start, int lpitch);
	int Clip_Line(int &x1,int &y1,int &x2, int &y2);
	
	void load_texture(Material&mat,const std::string& filename);
};
#endif
