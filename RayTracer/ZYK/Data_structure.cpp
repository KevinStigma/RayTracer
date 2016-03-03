#include "Data_structure.h"
#include "CPARSERV1.h"

namespace zyk
{
	FILE* fp_error=NULL;
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
		backup_pos=pos=Vec3(0,0,5);
		n=-(target-pos).normalized();

		//u=Vec3(-1,0,0);
		v=Vec3(0,1,0);
		u=v.cross(n);
		v=n.cross(u);

		view_height=near_clip_z*tan(fov*DEG_TO_RAD*0.5f)*2.0f;
		view_width=view_height*aspect_ratio;
		view_plane=Vec4(-view_width*0.5f,view_width*0.5f,-view_height*0.5f,view_height*0.5f);
		
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

	void tag_POLY::draw_lines(QPainter& painter)
	{
		/*Vec4 vertex[3];
		for(int i=0;i<3;i++)
			vertex[i]=vlist[vert[i]];

		painter.drawLine(QPointF(vertex[0][0],vertex[0][1]),QPointF(vertex[1][0],vertex[1][1]));
		painter.drawLine(QPointF(vertex[0][0],vertex[0][1]),QPointF(vertex[2][0],vertex[2][1]));
		painter.drawLine(QPointF(vertex[1][0],vertex[1][1]),QPointF(vertex[2][0],vertex[2][1]));*/
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

	int ObjectMesh::cull_object(const Camera& pCam,int cull_flags)
	{
		Vec4 center_cam=pCam.mcam*pCam.arcball_trans_cam*world_center;

		if(cull_flags&CULL_OBJECT_Z_PLANE)
		{
			if((center_cam(2)-max_radius>-pCam.near_clip_z)||
				(center_cam(2)+max_radius<-pCam.far_clip_z))
				return 1;
		}

		float ztest;

		if(cull_flags&CULL_OBJECT_X_PLANE)
		{
			ztest=-1*pCam.view_width*0.5f/pCam.view_dist*center_cam(2);
			if((center_cam(0)-max_radius>ztest)||
				(center_cam(0)+max_radius<-ztest))
				return 1;
		}

		if(cull_flags&CULL_OBJECT_Y_PLANE)
		{
			ztest=-1*pCam.view_height*0.5f/pCam.view_dist*center_cam(2);
			if((center_cam(1)-max_radius>ztest)||
				(center_cam(1)+max_radius<-ztest))
				return 1;
		}
		return 0;
	}

	void ObjectMesh::computeRadius()
	{
		Vec4 cent(0,0,0,0);
		for(int i=0;i<num_vertices;i++)
			cent+=vert[i].vlocal;
		cent/=(float)num_vertices;
		world_center=Vec4(cent(0)+world_pos(0),cent(1)+world_pos(1),cent(2)+world_pos(2),1);

		for(int i=0;i<num_vertices;i++)
		{
			float length=(cent-vert[i].vlocal).norm();
			if(max_radius<length)
				max_radius=length;
		}
	}
	

	void ObjectMesh::computeNormal()
	{
		Vec3 vec[2];
		Vec3 normal;
		std::vector<std::vector<int>> vert_2_face(num_vertices);
		for(int i=0;i<num_polys;i++)
		{
			zyk::Polygon& poly=plist[i];
			if(poly.state)
				continue;  //the poly has been culled

			vec[0]=zyk::Vector4FSub_V3(poly.vlist[poly.vert[1]].vtrans,poly.vlist[poly.vert[0]].vtrans);
			vec[1]=zyk::Vector4FSub_V3(poly.vlist[poly.vert[2]].vtrans,poly.vlist[poly.vert[0]].vtrans);
			//normal=vec[0].cross(vec[1]);
			normal=vec[1].cross(vec[0]);
			normal.normalize();
			poly.local_normal=Vec4(normal(0),normal(1),normal(2),0.f);
			for(int j=0;j<3;j++)
				vert_2_face[poly.vert[j]].push_back(i);
		}

		for(int i=0;i<num_vertices;i++)
		{
			Vec4 normal_avg(0,0,0,0);
			for(int j=0;j<vert_2_face[i].size();j++)
			{
				int index=vert_2_face[i][j];
				normal_avg+=plist[index].local_normal;
			}
			normal_avg/=(float)(vert_2_face[i].size());
			vert[i].local_nor=normal_avg;
		}
	}

	void ObjectMesh::centralize()
	{
		Vec4 center(0,0,0,0);
		for(int i=0;i<num_vertices;i++)
			center+=vert[i].vlocal;
		center/=(float)num_vertices;
		for(int i=0;i<num_vertices;i++)
		{
			vert[i].vlocal-=center;
			vert[i].vtrans=vert[i].vlocal;
			vert[i].vlocal(3)=1.f;
			vert[i].vtrans(3)=1.f;
		}
	}

	int Write_Error(char *string, ...)
	{
		// this function prints out the error string to the error file

		char buffer[1024]; // working buffer

		va_list arglist; // variable argument list

		// make sure both the error file and string are valid
		if (!string || !fp_error)
			return(0);

		// print out the string using the variable number of arguments on stack
		va_start(arglist,string);
		vsprintf(buffer,string,arglist);
		va_end(arglist);

		// write string to file
		fprintf(fp_error,buffer);

		// flush buffer incase the system bails
		fflush(fp_error);

		// return success
		return(1);
	} // end Write_Error

	int ReplaceChars(char *string_in, char *string_out, char *replace_chars, char rep_char, int case_on)
	{
		// this function simply replaces the characters from the input string that
		// are listed in replace with the replace char, the results are stored in 
		// string_out, string_in and isn't touched, the number of replacments is 
		// returned. if case_on = 1 then case is checked, other it's case insensitive

		int num_replacements = 0,  // tracks number of characters replaced
			index_in     = 0,      // curr index into input
			index_out    = 0,      // curr index into output
			sindex,                // loop var into strip array
			slength = strlen(replace_chars); // length of strip string

		// do some error checking
		if (!string_in || !string_out || strlen(string_in) == 0)
			return(0);

		// nothing to replace
		if (!replace_chars || strlen(replace_chars)==0)
		{
			strcpy(string_out, string_in);
			return(0);
		} // end if

		// determine if case is important
		if (case_on==1)
		{
			// perform char by char copy
			while(string_in[index_in])
			{
				for (sindex = 0; sindex < slength; sindex++)
					if (string_in[index_in] == replace_chars[sindex])
					{
						// replace it
						string_out[index_out++] = rep_char;
						index_in++;
						num_replacements++;
						break;
					} // end if

					// was a replacement performed?, no just copy then
					if (sindex >= slength)
						string_out[index_out++] = string_in[index_in++];

			} // end while
		} // end if case_on
		else
		{
			// perform char by char copy with case insensitivity
			while(string_in[index_in])
			{
				for (sindex = 0; sindex < slength; sindex++)
					if (toupper(string_in[index_in]) == toupper(replace_chars[sindex]))
					{
						// replace it
						string_out[index_out++] = rep_char;
						index_in++;
						num_replacements++;
						break;
					} // end if

					// was a strip char found?
					if (sindex >= slength)
						string_out[index_out++] = string_in[index_in++];

			} // end while
		} // end if case_off

		// terminate output string
		string_out[index_out] = 0;

		// return extracts
		return(num_replacements);

	} // end ReplaceChars

	char *Get_Line_PLG(char *buffer, int maxlength, FILE *fp)
	{
		// this little helper function simply read past comments 
		// and blank lines in a PLG file and always returns full 
		// lines with something on them on NULL if the file is empty

		int index = 0;  // general index
		int length = 0; // general length

		// enter into parsing loop
		while(1)
		{
			// read the next line
			if (!fgets(buffer, maxlength, fp))
				return(NULL);

			// kill the whitespace
			for (length = strlen(buffer), index = 0; isspace(buffer[index]); index++);

			// test if this was a blank line or a comment
			if (index >= length || buffer[index]=='#') 
				continue;

			// at this point we have a good line
			return(&buffer[index]);
		} // end while

	} // end Get_Line_PLG

	int Load_OBJECT4DV1_PLG(ObjectMesh& obj, char *filename,const Vec3& scale,    
		const Vec3& pos, const Vec4& rot)
	{
		// this function loads a plg object in off disk, additionally
		// it allows the caller to scale, position, and rotate the object
		// to save extra calls later for non-dynamic objects

		FILE *fp;          // file pointer
		char buffer[256];  // working buffer

		char *token_string;  // pointer to actual token text, ready for parsing

		// file format review, note types at end of each description
		// # this is a comment

		// # object descriptor
		// object_name_string num_verts_int num_polys_int

		// # vertex list
		// x0_float y0_float z0_float
		// x1_float y1_float z1_float
		// x2_float y2_float z2_float
		// .
		// .
		// xn_float yn_float zn_float
		//
		// # polygon list
		// surface_description_ushort num_verts_int v0_index_int v1_index_int ..  vn_index_int
		// .
		// .
		// surface_description_ushort num_verts_int v0_index_int v1_index_int ..  vn_index_int

		// lets keep it simple and assume one element per line
		// hence we have to find the object descriptor, read it in, then the
		// vertex list and read it in, and finally the polygon list -- simple :)

		// Step 1: clear out the object and initialize it a bit
		obj.num_polys=obj.num_vertices=0;
		obj.state =0;

		// set position of object
		obj.world_pos = pos;

		// Step 2: open the file for reading
		if (!(fp = fopen(filename, "r")))
		{
			std::cout<<"Couldn't open PLG file "<<filename<<std::endl;
			return(0);
		} // end if

		// Step 3: get the first token string which should be the object descriptor
		if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
		{
			std::cout<<"PLG file error with file "<<filename<<"(object descriptor invalid).";
			return(0);
		} // end if

		std::cout<<"Object Descriptor: "<<token_string<<std::endl;
		
		char mesh_name[255];
		// parse out the info object
		sscanf(token_string, "%s %d %d",mesh_name, &obj.num_vertices, &obj.num_polys);
		obj.name=mesh_name;

		// Step 4: load the vertex list
		for (int vertex = 0; vertex < obj.num_vertices; vertex++)
		{
			// get the next vertex
			if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
			{
				std::cout<<"PLG file error with file "<<filename<<"(vertex list invalid).";
				return(0);
			} // end if

			// parse out vertex
			float a,b,c;
			sscanf(token_string, "%f %f %f", &a, 
				&b, 
				&c);    

			obj.vert[vertex].vlocal(0)=a;
			obj.vert[vertex].vlocal(1)=b;
			obj.vert[vertex].vlocal(2)=c;
			// scale vertices
			obj.vert[vertex].vlocal(0)*=scale(0);
			obj.vert[vertex].vlocal(1)*=scale(1);
			obj.vert[vertex].vlocal(2)*=scale(2);

			obj.vert[vertex].vlocal=QUAT::rotate(rot(0)*DEG_TO_RAD,Vec3(rot(1),rot(2),rot(3)),obj.vert[vertex].vlocal);
		} // end for vertex

		// compute average and max radius
		//obj.computeRadius();

		int poly_surface_desc = 0; // PLG/PLX surface descriptor
		int poly_num_verts    = 0; // number of vertices for current poly (always 3)
		char tmp_string[8];        // temp string to hold surface descriptor in and
		// test if it need to be converted from hex

		// Step 5: load the polygon list
		for (int poly=0; poly <obj.num_polys; poly++)
		{
			// get the next polygon descriptor
			if (!(token_string = Get_Line_PLG(buffer, 255, fp)))
			{
				std::cout<<"PLG file error with file "<<filename<<"(polygon descriptor invalid).";
				return(0);
			} // end if

			// each vertex list MUST have 3 vertices since we made this a rule that all models
			// must be constructed of triangles
			// read in surface descriptor, number of vertices, and vertex list
			sscanf(token_string, "%s %d %d %d %d", tmp_string,
				&poly_num_verts, // should always be 3 
				&obj.plist[poly].vert[0],
				&obj.plist[poly].vert[1],
				&obj.plist[poly].vert[2]);

			// since we are allowing the surface descriptor to be in hex format
			// with a leading "0x" we need to test for it
			if (tmp_string[0] == '0' && toupper(tmp_string[1]) == 'X')
				sscanf(tmp_string,"%x", &poly_surface_desc);
			else
				poly_surface_desc = atoi(tmp_string);

			obj.plist[poly].vlist = obj.vert; 
			obj.plist[poly].state=0;
		} // end for poly

		// close the file
		fclose(fp);

		// return success
		return(1);
	}

	int Load_OBJECT4DV1_COB(ObjectMesh& obj,char *filename,const Vec3& scale,const Vec3& pos,
		const Vec4& rot,int vertex_flags)
	{
		fp_error=fopen("log.txt","w");
		CPARSERV1 parser; 

		char seps[16];          // seperators for token scanning
		char token_buffer[256]; // used as working buffer for token
		char *token;            // pointer to next token

		int r,g,b;              // working colors

		// cache for texture vertices
		Vec2 texture_vertices[1024];

		int num_texture_vertices = 0;

		Mat4 mat_local,  // storage for local transform if user requests it in cob format
			mat_world;  // "   " for local to world " "

		// initialize matrices
		mat_local=Mat4::Identity();
		mat_world=Mat4::Identity();

		// Step 1: clear out the object and initialize it a bit
		obj.state =0;

		// set position of object is caller requested position
		obj.world_pos=pos;

		// Step 2: open the file for reading using the parser
		if (!parser.Open(filename))
		{
			Write_Error("Couldn't open .COB file %s.", filename);
			return(0);
		} // end if

		// Step 3: 
		
		// lets find the name of the object first 
		while(1)
		{
			// get the next line, we are looking for "Name"
			if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				Write_Error("Image 'name' not found in .COB file %s.", filename);
				return(0);
			} // end if

			// check for pattern?  
			if ( parser.Pattern_Match(parser.buffer, "['Name'] [s>0]") )
			{
				// name should be in second string variable, index 1
				
				obj.name=parser.pstrings[1];   
				
				//Write_Error("\nCOB Reader Object Name: %s", obj.name);
				//std::cout<<"flag"<<std::endl;
				break;    
			} // end if

		} // end while

		
		// step 4: get local and world transforms and store them
		while(1)
		{
			// get the next line, we are looking for "center"
			if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				Write_Error("Center not found in .COB file %s.", filename);
				return(0);
			} // end if

			// check for pattern?  
			if ( parser.Pattern_Match(parser.buffer, "['center'] [f] [f] [f]") )
			{
				// the "center" holds the translation factors, so place in
				// last row of homogeneous matrix, note that these are row vectors
				// that we need to drop in each column of matrix
				mat_local(0,3)=-parser.pfloats[0]; // center x
				mat_local(1,3)=-parser.pfloats[1]; // center y
				mat_local(2,3)=-parser.pfloats[2]; // center z

				// ok now, the next 3 lines should be the x,y,z transform vectors
				// so build up   

				// "x axis" 
				parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
				parser.Pattern_Match(parser.buffer, "['x'] ['axis'] [f] [f] [f]");

				// place row in x column of transform matrix
				mat_local(0,0) = parser.pfloats[0]; // rxx
				mat_local(0,1) = parser.pfloats[1]; // rxy
				mat_local(0,2) = parser.pfloats[2]; // rxz

				// "y axis" 
				parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
				parser.Pattern_Match(parser.buffer, "['y'] ['axis'] [f] [f] [f]");

				// place row in y column of transform matrix
				mat_local(1,0) = parser.pfloats[0]; // ryx
				mat_local(1,1) = parser.pfloats[1]; // ryy
				mat_local(1,2) = parser.pfloats[2]; // ryz

				// "z axis" 
				parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
				parser.Pattern_Match(parser.buffer, "['z'] ['axis'] [f] [f] [f]");

				// place row in z column of transform matrix
				mat_local(2,0) = parser.pfloats[0]; // rzx
				mat_local(2,1) = parser.pfloats[1]; // rzy
				mat_local(2,2) = parser.pfloats[2]; // rzz

				//Print_Mat_4X4(&mat_local, "Local COB Matrix:");

				break;    
			} // end if

		} // end while

		// now "Transform"
		while(1)
		{
			// get the next line, we are looking for "Transform"
			if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				Write_Error("Transform not found in .COB file %s.", filename);
				return(0);
			} // end if

			// check for pattern?  
			if ( parser.Pattern_Match(parser.buffer, "['Transform']") )
			{

				// "x axis" 
				parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
				parser.Pattern_Match(parser.buffer, "[f] [f] [f]");

				// place row in x column of transform matrix
				mat_world(0,0) = parser.pfloats[0]; // rxx
				mat_world(0,1) = parser.pfloats[1]; // rxy
				mat_world(0,2) = parser.pfloats[2]; // rxz

				// "y axis" 
				parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
				parser.Pattern_Match(parser.buffer, "[f] [f] [f]");

				// place row in y column of transform matrix
				mat_world(1,0) = parser.pfloats[0]; // ryx
				mat_world(1,1) = parser.pfloats[1]; // ryy
				mat_world(1,2) = parser.pfloats[2]; // ryz

				// "z axis" 
				parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS);
				parser.Pattern_Match(parser.buffer, "[f] [f] [f]");

				// place row in z column of transform matrix
				mat_world(0,2) = parser.pfloats[0]; // rzx
				mat_world(2,1) = parser.pfloats[1]; // rzy
				mat_world(2,2) = parser.pfloats[2]; // rzz

				//Print_Mat_4X4(&mat_world, "World COB Matrix:");

				// no need to read in last row, since it's always 0,0,0,1 and we don't use it anyway
				break;    

			} // end if

		} // end while

		// step 6: get number of vertices and polys in object
		while(1)
		{
			// get the next line, we are looking for "World Vertices" 
			if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				Write_Error("'World Vertices' line not found in .COB file %s.", filename);
				return(0);
			} // end if

			// check for pattern?  
			if (parser.Pattern_Match(parser.buffer, "['World'] ['Vertices'] [i]") )
			{
				// simply extract the number of vertices from the pattern matching 
				// output arrays
				obj.num_vertices = parser.pints[0];

				Write_Error("\nCOB Reader Num Vertices: %d", obj.num_vertices);
				break;    

			} // end if

		} // end while

		// Step 7: load the vertex list
		// now read in vertex list, format:
		// "d.d d.d d.d"
		for (int vertex = 0; vertex < obj.num_vertices; vertex++)
		{
			// hunt for vertex
			while(1)
			{
				// get the next vertex
				if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
				{
					Write_Error("\nVertex list ended abruptly! in .COB file %s.", filename);
					return(0);
				} // end if

				// check for pattern?  
				if (parser.Pattern_Match(parser.buffer, "[f] [f] [f]"))
				{
					// at this point we have the x,y,z in the the pfloats array locations 0,1,2
					obj.vert[vertex].vlocal(0) = parser.pfloats[0];
					obj.vert[vertex].vlocal(1) = parser.pfloats[1];
					obj.vert[vertex].vlocal(2) = parser.pfloats[2];

					// scale vertices
					if (!FCMP(scale(0),1.0f)||!FCMP(scale(2),1.0f)||!FCMP(scale(1),1.0f))
					{
						obj.vert[vertex].vlocal(0)*=scale(0);
						obj.vert[vertex].vlocal(1)*=scale(1);
						obj.vert[vertex].vlocal(2)*=scale(2);
					} // end if

					obj.vert[vertex].vlocal=QUAT::rotate(rot(0)*DEG_TO_RAD,Vec3(rot(1),rot(2),rot(3)),obj.vert[vertex].vlocal);
					// found vertex, break out of while for next pass
					break;

				} // end if

			} // end while

		} // end for vertex
		//obj.computeRadius();

		// step 8: get number of texture vertices
		while(1)
		{
			// get the next line, we are looking for "Texture Vertices ddd" 
			if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				Write_Error("'Texture Vertices' line not found in .COB file %s.", filename);
				return(0);
			} // end if

			// check for pattern?  
			if (parser.Pattern_Match(parser.buffer, "['Texture'] ['Vertices'] [i]") )
			{
				// simply extract the number of texture vertices from the pattern matching 
				// output arrays
				num_texture_vertices = parser.pints[0];

				//Write_Error("\nCOB Reader Texture Vertices: %d", num_texture_vertices);
				break;    

			} // end if

		} // end while

		// Step 9: load the texture vertex list in format "U V"
		// "d.d d.d"
		for (int tvertex = 0; tvertex < num_texture_vertices; tvertex++)
		{
			// hunt for texture
			while(1)
			{
				// get the next vertex
				if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
				{
					//Write_Error("\nTexture Vertex list ended abruptly! in .COB file %s.", filename);
					return(0);
				} // end if

				// check for pattern?  
				if (parser.Pattern_Match(parser.buffer, "[f] [f]"))
				{
					// at this point we have the U V in the the pfloats array locations 0,1 for this 
					// texture vertex, although we do nothing with them at this point with this parser
					texture_vertices[tvertex](0) = parser.pfloats[0];
					texture_vertices[tvertex](1) = parser.pfloats[1];

					// found vertex, break out of while for next pass
					break;

				} // end if

			} // end while

		} // end for

		// step 10: load in the polygons
		// poly list starts off with:
		// "Faces ddd:"
		while(1)
		{
			// get next line
			if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				Write_Error("\n'Faces' line not found in .ASC file %s.", filename);
				return(0);
			} // end if

			// check for pattern?  
			if (parser.Pattern_Match(parser.buffer, "['Faces'] [i]"))
			{
				Write_Error("\nCOB Reader found face list in .COB file %s.", filename);

				// finally set number of polys
				obj.num_polys = parser.pints[0];

				break;
			} // end if
		} // end while


		int poly_surface_desc    = 0; // ASC surface descriptor/material in this case
		int poly_num_verts       = 0; // number of vertices for current poly (always 3)
		int num_materials_object = 0; // number of materials for this object

		for (int poly=0; poly < obj.num_polys; poly++)
		{
			// hunt until next face is found
			while(1)
			{
				// get the next polygon face
				if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
				{
					Write_Error("\nface list ended abruptly! in .COB file %s.", filename);
					return(0);
				} // end if

				// check for pattern?  
				if (parser.Pattern_Match(parser.buffer, "['Face'] ['verts'] [i] ['flags'] [i] ['mat'] [i]"))
				{
					// test if number of vertices is 3
					if (parser.pints[0]!=3)
					{
						Write_Error("\nface not a triangle! in .COB file %s.", filename);
						return(0);
					} // end if

					// now read out the vertex indices and texture indices format:
					// <vindex0, tindex0>  <vindex1, tindex1> <vindex1, tindex1> 
					if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
					{
						Write_Error("\nface list ended abruptly! in .COB file %s.", filename);
						return(0);
					} // end if

					// lets replace ",<>" with ' ' to make extraction easy
					ReplaceChars(parser.buffer, parser.buffer, ",<>",' ',1);      
					parser.Pattern_Match(parser.buffer, "[i] [i] [i] [i] [i] [i]");

					// 0,2,4 holds vertex indices
					// 1,3,5 holds texture indices -- unused for now, no place to put them!

					// insert polygon, check for winding order invert
				    // leave winding order alone
					poly_num_verts           = 3;
					if(!vertex_flags)
					{
						obj.plist[poly].vert[0] = parser.pints[0];
						obj.plist[poly].vert[1] = parser.pints[2];
						obj.plist[poly].vert[2] = parser.pints[4];
					}
					else
					{
						obj.plist[poly].vert[0] = parser.pints[0];
						obj.plist[poly].vert[1] = parser.pints[4];
						obj.plist[poly].vert[2] = parser.pints[2];
					}
					obj.plist[poly].vlist = obj.vert; 

					// set polygon to active
					obj.plist[poly].state =0;    

					// found the face, break out of while for another pass
					break;

				} // end if

			} // end while           
		} // end for poly

		// now find materials!!! and we are out of here!
		for (int curr_material = 0; curr_material < num_materials_object; curr_material++)
		{
			// hunt for the material header "mat# ddd"
			while(1)
			{
				// get the next polygon material 
				if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
				{
					Write_Error("\nmaterial list ended abruptly! in .COB file %s.", filename);
					return(0);
				} // end if

				// check for pattern?  
				if (parser.Pattern_Match(parser.buffer, "['mat#'] [i]") )
				{
					// extract the material that is being defined 
					int material_index = parser.pints[0];

					// get color of polygon, although it might be irrelevant for a textured surface
					while(1)
					{
						// get the next line
						if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
						{
							Write_Error("\nRGB color ended abruptly! in .COB file %s.", filename);
							return(0);
						} // end if

						// replace the , comma's if there are any with spaces
						ReplaceChars(parser.buffer, parser.buffer, ",", ' ', 1);

						// look for "rgb float,float,float"
						if (parser.Pattern_Match(parser.buffer, "['rgb'] [f] [f] [f]") )
						{
							break; // while looking for rgb
						} // end if

					} // end while    

					// although our engine will have minimal support for these, we might as well get them
					while(1)
					{
						// get the next line
						if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
						{
							Write_Error("\nmaterial properties ended abruptly! in .COB file %s.", filename);
							return(0);
						} // end if

						// look for "alpha float ka float ks float exp float ior float"
						if (parser.Pattern_Match(parser.buffer, "['alpha'] [f] ['ka'] [f] ['ks'] [f] ['exp'] [f]") )
						{

							break;
						} // end if

					} // end while    

					while(1)
					{
						// get the next line
						if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
						{
							Write_Error("\nshader class ended abruptly! in .COB file %s.", filename);
							return(0);
						} // end if

						if (parser.Pattern_Match(parser.buffer, "['Shader'] ['class:'] ['color']") )
						{
							break;
						} // end if

					} // end while

					// now look for the shader name for this class
					// Shader name: "plain color" or Shader name: "texture map"
					while(1)
					{
						// get the next line
						if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
						{
							Write_Error("\nshader name ended abruptly! in .COB file %s.", filename);
							return(0);
						} // end if

						// replace the " with spaces
						ReplaceChars(parser.buffer, parser.buffer, "\"", ' ', 1);

						// is this a "plain color" poly?
						if (parser.Pattern_Match(parser.buffer, "['Shader'] ['name:'] ['plain'] ['color']") )
						{
							// not much to do this is default, we need to wait for the reflectance type
							// to tell us the shading mode

							break;
						} // end if

						// is this a "texture map" poly?
						if (parser.Pattern_Match(parser.buffer, "['Shader'] ['name:'] ['texture'] ['map']") )
						{
							while(1)
							{
								// get the next line
								if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
								{
									Write_Error("\ncouldnt find texture name! in .COB file %s.", filename);
									return(0);
								} // end if

								// replace the " with spaces
								ReplaceChars(parser.buffer, parser.buffer, "\"", ' ', 1);

								// is this the file name?
								if (parser.Pattern_Match(parser.buffer, "['file'] ['name:'] ['string'] [s>0]") )
								{
									// ok, simply convert to a real file name by changing the slashes
									ReplaceChars(parser.pstrings[3], parser.pstrings[3], "\\", '/',1);

									// and save the filename
									//strcpy(materials[material_index + num_materials].texture_file, parser.pstrings[3]);

									break;
								} // end if

							} // end while

							break;
						} // end if

					} // end while 

					// alright, finally! Now we need to know what the actual shader type, now in the COB format
					// I have decided that in the "reflectance" class that's where we will look at what kind
					// of shader is supposed to be used on the polygon

					//  look for the "Shader class: reflectance"
					while(1)
					{
						// get the next line
						if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
						{
							Write_Error("\nshader reflectance class not found in .COB file %s.", filename);
							return(0);
						} // end if

						// look for "Shader class: reflectance"
						if (parser.Pattern_Match(parser.buffer, "['Shader'] ['class:'] ['reflectance']") )
						{
							// now we know the next "shader name" is what we are looking for so, break

							break;
						} // end if

					} // end while    

					while(1)
					{
						// get the next line
						if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
						{
							Write_Error("\nshader name ended abruptly! in .COB file %s.", filename);
							return(0);
						} // end if

						// get rid of those quotes
						ReplaceChars(parser.buffer, parser.buffer, "\"",' ',1);

						// did we find the name?
						if (parser.Pattern_Match(parser.buffer, "['Shader'] ['name:'] [s>0]" ) )
						{
							// figure out which shader to use
							if (strcmp(parser.pstrings[2], "constant") == 0)
							{
								// set the shading mode flag in material
								//SET_BIT(materials[material_index + num_materials].attr, MATV1_ATTR_SHADE_MODE_CONSTANT);
							} // end if
							else
								if (strcmp(parser.pstrings[2], "matte") == 0)
								{
									// set the shading mode flag in material
									//SET_BIT(materials[material_index + num_materials].attr, MATV1_ATTR_SHADE_MODE_FLAT);
								} // end if
								else
									if (strcmp(parser.pstrings[2], "plastic") == 0)
									{
										// set the shading mode flag in material
										//SET_BIT(materials[curr_material + num_materials].attr, MATV1_ATTR_SHADE_MODE_GOURAUD);
									} // end if
									else
										if (strcmp(parser.pstrings[2], "phong") == 0)
										{
											// set the shading mode flag in material
											//SET_BIT(materials[material_index + num_materials].attr, MATV1_ATTR_SHADE_MODE_FASTPHONG);
										} // end if
										else
										{
											// set the shading mode flag in material
											//SET_BIT(materials[material_index + num_materials].attr, MATV1_ATTR_SHADE_MODE_FLAT);
										} // end else

										break;
						} // end if

					} // end while

					// found the material, break out of while for another pass
					break;

				} // end if found material

			} // end while looking for mat#1

		} // end for curr_material
		fclose(fp_error);
		// return success
		return(1);
	}

	int Load_OBJECT4DV1_OBJ(ObjectMesh& obj,char *filename,const Vec3& scale,const Vec3& pos,
		const Vec4& rot,int vertex_flags)
	{
		std::ifstream in(filename);

		if(!in.good())
		{
			std::cout << "ERROR: loading obj:(" << filename << ") file is not good" << "\n";
			return 0;
		}

		char buffer[256], str[255];
		float f1,f2,f3;

		int vcount=0,fcount=0;
		obj.world_pos=pos;
		while(!in.getline(buffer,255).eof())
		{
			buffer[255]='\0';
			sscanf_s(buffer,"%s",str,255);

			// reading a vertex
			if (buffer[0]=='v' && (buffer[1]==' '  || buffer[1]==32) )
			{
				if ( sscanf(buffer,"v %f %f %f",&f1,&f2,&f3)==3)
				{
					if(vcount==OBJECT4DV1_MAX_VERTICES)
					{
						std::cout<<"The number of vertices is out of range"<<std::endl;
						return 0;
					}
					obj.vert[vcount].vlocal=Vec4(f1*scale(0),f2*scale(1),f3*scale(2),1.f);
					obj.vert[vcount].vlocal=QUAT::rotate(rot(0)*DEG_TO_RAD,Vec3(rot(1),rot(2),rot(3)),obj.vert[vcount].vlocal);
					vcount++;
				}
				else
				{
					std::cout << "ERROR: vertex not in wanted format in OBJLoader" << "\n";
					return 0;
				}
			}
			// reading FaceMtls 
			else if (buffer[0]=='f' && (buffer[1]==' ' || buffer[1]==32) )
			{
				//TriangleFace f;
				int i1,i2,i3;
				int nt = sscanf(buffer,"f %d %d %d",&i1,&i2,&i3);
				if( nt!=3 )
				{
					std::cout << "ERROR: I don't know the format of that FaceMtl" << "\n";
					return 0;
				}
				if(fcount==OBJECT4DV1_MAX_POLYS)
				{
					std::cout<<"The number of polygons is out of range"<<std::endl;
					return 0;
				}
				if(!vertex_flags)
				{
					obj.plist[fcount].vert[0]=i1-1;
					obj.plist[fcount].vert[1]=i2-1;
					obj.plist[fcount].vert[2]=i3-1;
				}
				else
				{
					obj.plist[fcount].vert[0]=i1-1;
					obj.plist[fcount].vert[1]=i3-1;
					obj.plist[fcount].vert[2]=i2-1;
				}
				obj.plist[fcount].vlist=obj.vert;
				fcount++;
			}
		}
		obj.num_polys=fcount;
		obj.num_vertices=vcount;
		return 1;
	}
	void load_texture(Material&mat,const std::string& filename)
	{
		mat.texture_file=filename;
		mat.texture.loadImg(filename.c_str());
	}

	QUAT QUAT::Identify=QUAT(1,0,0,0);
	Vec3 QUAT::rotate(float angle/*radian*/,const Vec3& axis,const Vec3& point)
	{
		float cos_v=cos(angle*0.5f);
		float sin_v=sin(angle*0.5f);
		QUAT vq(0,point(0),point(1),point(2));
		QUAT rot(cos_v,sin_v*axis[0],sin_v*axis[1],sin_v*axis[2]);
		rot.normalize();
		QUAT vq2=rot.conjugate()*vq*rot;//counterclockwise in right hand coordinate
		return Vec3(vq2.m(1),vq2.m(2),vq2.m(3));
	}

	Vec4 QUAT::rotate(float angle,const Vec3& axis,const Vec4&point)
	{
		Vec3 p(point(0),point(1),point(2));
		Vec3 rp=rotate(angle,axis,p);
		return Vec4(rp(0),rp(1),rp(2),1.f);
	}
	inline QUAT QUAT::operator+(const QUAT&q)
	{
		return QUAT(m+q.m);
	}

	inline QUAT QUAT::operator-(const QUAT&q)
	{
		return QUAT(m-q.m);
	}

	inline QUAT QUAT::operator*(const QUAT&q)
	{
		Mat4 mat;
		mat<<m(3),-m(2),m(1),m(0),
			m(2),m(3),-m(0),m(1),
			-m(1),m(0),m(3),m(2),
			-m(0),-m(1),-m(2),m(3);
		return QUAT(mat*q.m);
	}

	inline QUAT QUAT::operator*(const float&val)
	{
		return QUAT(val*m);
	}

	inline QUAT QUAT::operator=(const QUAT&q)
	{
		m=Vec4(q.m);
		return (*this);
	}

	inline QUAT QUAT::conjugate()
	{
		return QUAT(m(0),-m(1),-m(2),-m(3));
	}

	inline float QUAT::norm()
	{
		return m.norm();
	}

	inline QUAT QUAT::inverse()
	{
		return conjugate()/(m(0)*m(0)+m(1)*m(1)+m(2)*m(2)+m(3)*m(3));
	}
	inline void QUAT::normalize()
	{
		m.normalize();
	}
	inline QUAT QUAT::normalized()
	{
		return QUAT(m.normalized());
	}

	inline QUAT QUAT::operator/(const float&va)
	{
		return QUAT(m/va);
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

	bool Sphere::intersect(const Vec3& origin,const Vec3& dir,float& t)
	{
		t=0;
		float tmp_val=dir.dot(origin-center);
		Vec3 tmp_vec=origin-center;
		float val=tmp_val*tmp_val-dir.dot(dir)*(tmp_vec.dot(tmp_vec)-radius*radius);

		float inv_dot=1/dir.dot(dir);
		if(val<0)
			return false;
		else if(FCMP(val,0.0))
		{
			t=-tmp_val*inv_dot;
			if(t<0)
				return false;
		}
		else
		{
			val=sqrt(val);
			float t1=(-tmp_val+val)*inv_dot;
			float t2=(-tmp_val-val)*inv_dot;
			if(t1<0&&t2<0)
				return false;
			else if(t1<0)
				t=t2;
			else if(t2<0)
				t=t1;
			else
				t=std::min(t1,t2);
		}
		return true;
	}
};