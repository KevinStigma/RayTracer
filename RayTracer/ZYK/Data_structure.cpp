#include "Data_structure.h"
#include "Objects.h"

namespace zyk
{
	void Camera::init(float width,float height)
	{
		aspect_ratio=width/height;
		viewport_width=width;
		viewport_height=height;
		fov=45.0f;
		near_clip_z=0.1f;
		far_clip_z=1000.0f;
		view_dist=near_clip_z;
		backup_target=target=Vec3(0,0,0);
		backup_pos=pos=Vec3(0,0,10);
		n=-(target-pos).normalized();

		//u=Vec3(-1,0,0);
		v=Vec3(0,1,0);
		u=v.cross(n);
		v=n.cross(u);

		view_height=near_clip_z*tan(fov*DEG_TO_RAD*0.5f)*2.0f;
		view_width=view_height*aspect_ratio;
		view_plane=Vec4(-view_width*0.5f,view_width*0.5f,view_height*0.5f,-view_height*0.5f);

		reset_camera_matrix();
		reset_perspective_proj_matrix();
		reset_viewport_matrix();
	}

	void Camera::reset_camera_matrix()
	{
		Mat4 m1,m2;
		m1.row(0)=Vec4(u[0],u[1],u[2],0);
		m1.row(1)=Vec4(v[0],v[1],v[2],0);
		m1.row(2)=Vec4(n[0],n[1],n[2],0);
		m1.row(3)=Vec4(0,0,0,1);
		m2=Mat4::Identity();
		for(int i=0;i<3;i++)
			m2(i,3)=-pos[i];
		mcam=m1*m2;
	}

	void Camera::reset_perspective_proj_matrix()
	{
		float fsn_inv=1.0f/(far_clip_z-near_clip_z);

		mper<<2.f*near_clip_z/view_width,0,0,0,
			0,2.f*near_clip_z/view_height,0,0,
			0,0,(near_clip_z+far_clip_z)*(-1)*fsn_inv,-2.f*far_clip_z*near_clip_z*fsn_inv,
			0,0,-1,0;
	}

	void Camera::reset_viewport_matrix()
	{
		float alpha=0.5f*viewport_width;
		float beta=0.5f*viewport_height;
		mscr<<alpha,0,0,alpha-0.5f,
			0,-beta,0,-beta+viewport_height-0.5f,
			0,0,1,0,
			0,0,0,1;
	}
	void Camera::update_arcball_matrix(const float mat[])
	{
		arcball_trans_cam<<mat[0],mat[4],mat[8],mat[12],
			mat[1],mat[5],mat[9],mat[13],
			mat[2],mat[6],mat[10],mat[14],
			mat[3],mat[7],mat[11],mat[15];
	}

	bool Camera::Out_of_Frustum(const Vec4& vert)//the vertex must be in the camera coordinate
	{
		Vec4 vertex=vert;
		if(!FCMP(vertex(3),1.0f))
		{
			for(int i=0;i<4;i++)
				vertex(i)/=vert(3);
		}
		return Out_of_Frustum(Vec3(vertex(0),vertex(1),vertex(2)));
	}

	bool Camera::Out_of_Frustum(const Vec3& vert)
	{
		float val;
		val=abs(vert(2))*tan(fov*0.5*DEG_TO_RAD);
		if(abs(vert(1))>val)
			return true;
		val=val*aspect_ratio;
		if(abs(vert(0))>val)
			return true;
		if(vert(2)>-near_clip_z||vert(2)<-far_clip_z)
			return true;
		return false;
	}

	void PolyFace::draw_lines(QPainter& painter)
	{
		painter.drawLine(QPointF(tlist[0][0],tlist[0][1]),QPointF(tlist[1][0],tlist[1][1]));
		painter.drawLine(QPointF(tlist[0][0],tlist[0][1]),QPointF(tlist[2][0],tlist[2][1]));
		painter.drawLine(QPointF(tlist[1][0],tlist[1][1]),QPointF(tlist[2][0],tlist[2][1]));
	}

	void PolyFace::draw_vertices(QPainter&painter)
	{
		QPen pen;
		for(int i=0;i<3;i++)
		{
			Vec4 color=v_color[i]*255+Vec4(0.5f,0.5f,0.5f,0.5f);
			pen.setColor(QColor(color(0),color(1),color(2)));
			painter.setPen(pen);
			//QPointF(tlist[i][0]+0.5f,tlist[i][1]+0.5f)
			painter.drawPoint(tlist[i][0],tlist[i][1]);
		}
		//painter.drawPoint(QPointF(tlist[1][0],tlist[1][1]));
		//painter.drawPoint(QPointF(tlist[2][0],tlist[2][1]));
	}

	void load_texture(Material&mat,const std::string& filename)
	{
		mat.texture_file=filename;
		mat.texture.loadImg(filename.c_str());
	}

	void RenderList::outputPolygon(std::string& filename)
	{
		std::ofstream out(filename);
		for(int i=0;i<num_polys;i++)
		{
			out<<"poly "<<i<<std::endl;
			const zyk::PolyFace& poly_face=*poly_ptrs[i];
			out<<"color \n"<<poly_face.color(0)<<" "<<poly_face.color(1)<<" "<<poly_face.color(2)<<" "<<poly_face.color(3)<<std::endl;
			out<<"vertex:"<<std::endl;
			for(int j=0;j<3;j++)
				out<<poly_face.tlist[j](0)<<" "<<poly_face.tlist[j](1)<<" "<<poly_face.tlist[j](2)<<std::endl;
		}
		out.close();
	}

	tag_Light::tag_Light():light_source_mesh(NULL){}

	void Light::getIlluminatinInfo(const Vec3& point,Vec3& light_dir,Vec4& light_intensity)const
	{
		light_dir=getLightingDirection(point);
		if(type==DIRECTION_LIGHT)
			light_intensity=Vec4(1.0,1.0,1.0,1.0);
		else if(type==SPOT_LIGHT)
		{
			light_dir=getLightingDirection(point);
			Vec3 light_vec=pos-point;
			float d=light_vec.norm();
			float value=1.0/(kc+kl*d);
			light_intensity=Vec4(value,value,value,1.0f);
		}
		else if(type==AREA_LIGHT)
			light_intensity=Vec4(1.0,1.0,1.0,1.0);
	}

	Vec3 Light::getLightingDirection(const Vec3& point)const
	{
		if(type==DIRECTION_LIGHT)
		{
			return dir;
		}
		else if(type==SPOT_LIGHT)
		{
			Vec3 light_vec=pos-point;
			return light_vec.normalized();
		}
		else if(type==AREA_LIGHT)
		{
			assert(light_source_mesh&&light_source_mesh->getOBB());
			Vec3 rand_light_pos=light_source_mesh->getOBB()->getRandomPt();
			return (rand_light_pos-point).normalized();
		}
		return Vec3(0,0,0);
	}
};