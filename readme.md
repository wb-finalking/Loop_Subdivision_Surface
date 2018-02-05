# Loop Subdivision Surface

### 开发环境

**VS2013**+**QT5.8**

### 功能实现

输入：通过菜单栏选择任意的多边形obj文件

输出：窗口模型显示

操作：**鼠标左键**--旋转模型

​	    **鼠标右键**--进行loop 细分曲面

​	    **按键F**--线框显示与关闭

### 核心算法描述

##### Model Triangulation

对于多余3条边的多边形进行三角分割，在多边形的中间插入一个新的顶点（多变形的中心），然后将各边与中心连接形成多个三角形。

![](https://github.com/wb-finalking/Loop_Subdivision_Surface/blob/master/0.jpg?raw=true)

##### Loop Subdivision Surface

Loop细分曲面主要分为两个步骤：创建新的顶点和创建新的面

**1>顶点的创建**

每一条实体边会产生一个新的顶点，原来的顶点需要更新

* 网格内部边的顶点创建

  设内部边的两个端点为$v_0$、$v_1$，相对的两个顶点为$v_2$、$v_3$，则新增加的顶点位置为$v = 3/8*(v_0 + v_1) + 1/8*(v_2 + v_3)$

  ![](https://github.com/wb-finalking/Loop_Subdivision_Surface/blob/master/2.jpg?raw=true)

* 网格边界边的顶点创建

  设边界边的两个端点为$v_0$、$v_1$，则新增加的顶点位置为$v = 1/2*(v_0 + v_1)$

  ![](https://github.com/wb-finalking/Loop_Subdivision_Surface/blob/master/3.jpg?raw=true)

* 网格内部顶点的更新

  设内部顶点v0的相邻点为$v_1$、$v_2$，…，$v_n$，则该顶点更新后位置为$v=(1-n\beta)v_0+\beta\sum_{i=1}^nv_i$，其中$\beta=\frac{1}{n}\left[\frac{5}{8}-\left(\frac{3}{8}+\frac{1}{4}\cos\frac{2\pi}{n}\right)^2\right]$

  ![](https://github.com/wb-finalking/Loop_Subdivision_Surface/blob/master/4.jpg?raw=true)

* 网格边界顶点的更新

  设边界顶点$v_0$的两个相邻点为$v_1$、$v_2$，则该顶点更新后位置为$v = \frac{3}{4}*v_0 + \frac{1}{8}*\left(v_1 + v_2\right)$

  ![](https://github.com/wb-finalking/Loop_Subdivision_Surface/blob/master/5.jpg?raw=true)

**2>创建新面**

对于原来每个三角形面会产生6个新的顶点，由这6个新的顶点构成4个新的面

![](https://github.com/wb-finalking/Loop_Subdivision_Surface/blob/master/6.jpg?raw=true)

### 核心代码分析

##### 数据结构

```C++
struct Vertex {
  	//存放顶点的x、y、z坐标
	Vector4* vector;
	//存放与其相关的半边id，此半边终点是此顶点
	int halfEdge;
};

struct Face {
	//按逆时针存放此面的顶点id
	vector<int> verts;
};

struct HalfEdge {
	//由于loop细分曲面只用到每条半边的对边，所以在此只存每个半边的对边id
	int pair;
};
```

##### ReadObj类

读取obj文件

```C++
bool  ReadObj::readfile(std::string filename)
{
	std::ifstream file(filename);
  	//判断文件是否存在
	if (!file) {
		std::cerr << "Invalid file name!" << std::endl;
		return false;
	}
	//一行一行读取文件
	for (std::string line; std::getline(file, line);)
	{
      	//如果是以v开头则代表是顶点
		if (line[0] == 'v' && line[1] == ' ')
		{
			//把顶点压入顶点vector
		}
      	//如果是以f开头则代表是面
		else if (line[0] == 'f' && line[1] == ' ')
		{
			//将顶点按顺序一个一个读入面的顶点数组，然后将面压入面vector
		}

	}
	//将模型中心移动到坐标原点，并且将顶点都放缩到[-1 , 1]区间
	for (int i = 0; i < vertices.size(); ++i)
	{
		*vertices[i].vector -= tmp_vector;
		*vertices[i].vector *= scale;
	}
	//模型简单三角化
	triangulator();
	//构建半边结构信息并返回
	return generateHalfEdge();
}
```

构建半边结构信息

```C++
bool ReadObj::generateHalfEdge()
{
  	//存放与一个顶点相关的其他半边信息
	std::vector<std::vector<int> > edgeMap(vertices.size(), std::vector<int>());
	//根据面存储的顶点，获得每一个半边
	for (int he_id = 0; he_id < 3 * faces.size(); ++he_id) {
		//半边所在面id
		int face_id = he_id / 3;
      	//半边的起点与终点
		int s_v = he_id % 3;
		int e_v = (he_id + 1) % 3;
      	//将此半边id信息赋值给与其相关的终点
		vertices[faces[face_id].verts[e_v]].halfEdge = he_id;

		//寻找与此半边终点相关的其他半边
		for (std::vector<int>::iterator it = edgeMap[faces[face_id].verts[e_v]].begin(); it != edgeMap[faces[face_id].verts[e_v]].end(); ++it) {
			int tmp_he_id = *it;
			int tmp_face_id = tmp_he_id / 3;
			int tmp_e_v = (tmp_he_id + 1) % 3;
			
			//如果找到的半边的终点是此半边的起点，那么这两条吧半边是一对
			if (faces[face_id].verts[s_v] == faces[tmp_face_id].verts[tmp_e_v]) {
				pair = tmp_he_id;
				//将配对pair信息复制给对应的半边结构
				halfEdges[tmp_he_id].pair = he_id;
				edgeMap[faces[face_id].verts[e_v]].erase(it);
				break;
			}
		}
		//没有找到此半边的配对半边则将其压入与其起点相关的vector
		if (pair == -1) {
			edgeMap[faces[face_id].verts[s_v]].push_back(he_id);
		}

		//将此半边的pair结果信息放入半边结构
		halfEdges[he_id].pair = pair;

	}

	std::cerr << "Loading complete!" << std::endl;
	return true;
}
```

##### Render类

loop细分曲面

```C++
void Render::loop_subdivision()
{
	//***************更新原来顶点坐标*****************//
  	//将原来顶点一个一个更新
	for (int vertex_id = 0; vertex_id < vertices.size(); ++vertex_id) 
    {
      	//找到与此顶点相关的一条半边
		int start_edge = vertices[vertex_id].halfEdge;
		int current_edge = start_edge;
		bool flag = false;
		//默认是内部顶点，循环一周将此顶点的所有相邻顶点找到
		do {
			//找到当前半边的对边
			current_edge = halfEdges[current_edge].pair;
			//如果此半边的对边为-1，则此顶点为边界顶点，停止循环重新计算
			if (current_edge == -1)
			{
				flag = true;
				break;
			}
			//相邻顶点坐标累加到sum
			//sum += *(vertices[相邻顶点id].vector);
			//相邻顶点个数累计n++;
			
			//找到于此顶点相关且与当前半边同在一个面的另一条半边
			current_edge = 3 * (current_edge / 3) + ((current_edge + 2) % 3);
          
          //如果当前半边与其实半边相同则循环结束
		} while (current_edge != start_edge);

		Vector4 nov;
		//如果顶点时内部顶点则可以直接更新
		if (!flag)
		{	
			//计算beta（b）并更新顶点坐标
			nov = *vertices[vertex_id].vector * (1 - b) + sum * (b / n);
		}
		else
		{	
			//如果是边界顶点则找到其两条边界半边
			bool r_flag = false;
			bool l_flag = false;
			//初始化两条边界半边
			int l_current_edge = start_edge;
			int r_current_edge = l_current_edge;
			//如果右边界半边的对边是-1，则找到了，否则将其指向其对边
			if (halfEdges[r_current_edge].pair == -1)
				r_flag = true;
			else
				r_current_edge = halfEdges[l_current_edge].pair;
			//由一条半边没找到则继续循环
			while ( (r_flag&&l_flag) == false)
			{
				if (!l_flag)
				{	
					//找到于此顶点相关且与当前半边同在一个面的另一条半边
					//l_current_edge = ......
					//如果左边界半边的对边是-1，则找到了，否则将其指向其对边
					if (halfEdges[l_current_edge].pair == -1)
						l_flag = true;
					else
						l_current_edge = halfEdges[l_current_edge].pair;
				}
				if (!r_flag)
				{
					//找到于此顶点相关且与当前半边同在一个面的另一条半边
					//r_current_edge = ......
					//如果右边界半边的对边是-1，则找到了，否则将其指向其对边
					if (halfEdges[r_current_edge].pair == -1)
						r_flag = true;
					else
						r_current_edge = halfEdges[r_current_edge].pair;
				}
			}
             //更新当前顶点坐标
			//sum=......
			nov = *vertices[vertex_id].vector * (3.0 / 4.0) + sum * (1.0 / 8.0);
		}
		//将更新后的顶点压入新的顶点vector
		Vertex tmp_v;
		tmp_v.vector = new Vector4(nov);
		tmp_v.halfEdge = -1;
		new_vertices.push_back(tmp_v);
	}

	//****************创建边上的新顶点**********************//
	//记录半边对应的实体边是否被计算过
	edgeMap.clear();
	edgeMap = std::vector<int>(old_halfedge_num, -1);

	int unused_slot = 0;
	//对每条半边计算新的顶点
	for (int h = 0; h < old_halfedge_num; ++h)
    {
		//如果此半边对应的实体边没有被计算过
		if (edgeMap[h] == -1) 
        {
			//如果此半边是内部半边 
			if (halfEdges[h].pair != -1)
			{
				//找到此半边的2个邻点以及2个对点计算新的顶点
				Vector4 vec = (*vertices[s_v].vector + *vertices[e_v].vector) * 3.0 / 8.0
					+ (*vertices[neighbor_v0].vector + *vertices[neighbor_v1].vector) 						*1.0 / 8.0;
				//将新的顶点压入新的顶点vector
				new_vertices.push_back({ new Vector4(vec), -1 });
			}
			//如果此半边是边界半边 
			else
			{
				//找到此半边的2个邻点计算新的顶点
				Vector4 vec = (*vertices[s_v].vector + *vertices[e_v].vector) / 2.0;
				//将新的顶点压入新的顶点vector
				new_vertices.push_back({ new Vector4(vec), -1 });
			}
		}
	}

	//****************构建新的面**********************//
	//对每个面构建4个新的面
	for (int f = 0; f < faces.size(); ++f) 
    {
		//按原来的顶点顺序构建4个新的面

		//顺便对6个新的顶点依据现在的面id更新数据，以便下一次细分
		new_vertices[new_faces[4 * f].verts[1]].halfEdge = f * 12;
		new_vertices[new_faces[4 * f + 1].verts[1]].halfEdge = f * 12 + 3;
		new_vertices[new_faces[4 * f + 2].verts[1]].halfEdge = f * 12 + 6;

		new_vertices[edgeMap[f * 3]].halfEdge = f * 12 + 1;
		new_vertices[edgeMap[f * 3 + 1]].halfEdge = f * 12 + 4;
		new_vertices[edgeMap[f * 3 + 2]].halfEdge = f * 12 + 2;
	}

	//*****************更新半边结构信息*********************//
	//依次对每一个面中的4个新面所对应的半边更新对边信息
	for (int i = 0; i < faces.size(); ++i)
    {
		int b = 12 * i;
		//对于原来面边上的新边可根据原来的对边信息更新对边信息
 		//对于原来面内部的新边可直接得到其对边
		//有6条内部边和6条边上的边
		new_halfEdges[b] = { nIndex((i * 3) + 2, 1) };
		new_halfEdges[b + 1] = { nIndex((i * 3), 0) };
		new_halfEdges[b + 2] = { b + 9 };

		new_halfEdges[b + 3] = { nIndex((i * 3), 1) };
		new_halfEdges[b + 4] = { nIndex((i * 3) + 1, 0) };
		new_halfEdges[b + 5] = { b + 10 };

		new_halfEdges[b + 6] = { nIndex((i * 3) + 1, 1) };
		new_halfEdges[b + 7] = { nIndex((i * 3) + 2, 0) };
		new_halfEdges[b + 8] = { b + 11 };

		new_halfEdges[b + 9] = { b + 2 };
		new_halfEdges[b + 10] = { b + 5 };
		new_halfEdges[b + 11] = { b + 8 };

	}

	//**************更新原来的结构****************//
	//清空原来的额顶点vector
  	for (Vertex v : vertices) {
		delete v.vector;
	}
	vertices.clear();
	//移动赋值给顶点vector
	vertices = std::move(new_vertices);
  
	//清空面vector并移动赋值
	faces.clear();
	faces = std::move(new_faces);

	//清空半边vector并移动赋值
	halfEdges.clear();
	halfEdges = std::move(new_halfEdges);
}
```

### 效果展示

在此展示三个具有代表新的模型：全是三角面片的兔子、存在边界半边的茶壶、具有多边形面片的F-16飞机![](https://github.com/wb-finalking/Loop_Subdivision_Surface/blob/master/bunny.jpg?raw=true)

![](https://github.com/wb-finalking/Loop_Subdivision_Surface/blob/master/teapot.jpg?raw=true)

![](https://github.com/wb-finalking/Loop_Subdivision_Surface/blob/master/f.jpg?raw=true)