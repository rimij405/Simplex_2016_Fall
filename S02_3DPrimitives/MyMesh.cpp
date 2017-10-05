#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();
	
	// Replace this with your code
	// GenerateCone(a_fRadius, a_fHeight, a_nSubdivisions, a_v3Color);
	// -------------------------------

	uint uicount_triangles = 0;

	//  b - c - d
	// /  \ | /  \
	//i-----a-----e
	// \  / | \  /
	//  h - g - f

	// Height and bottom values.
	float fheight = (a_fHeight / 2.0f);
	float fbottom = -(fheight);

	// Get the center point.
	vector3 center = vector3(0.0f, fbottom, 0.0f);

	// Get the tip.
	vector3 tip = vector3(0.0f, fheight, 0.0f);

	// Create collection.
	std::vector<vector3> vertices = std::vector<vector3>();
	uint uicount = 0;
	
	// Find points uniformly distributed on circle of provided radius.
	for (uint i = 0; i < a_nSubdivisions; i++) {
		
		// Calculate current angle.
		float theta = 2.0f * PI * ((float) i/(float) a_nSubdivisions); // Gets angle for the current point, in relation to the center.

		// Use polar coordinates to get x and z.
		float x = a_fRadius * cosf(theta);
		float z = a_fRadius * sinf(theta);

		// Push new vertex onto collection.
		vertices.push_back(vector3(x, fbottom, z));
		uicount++;
	}

	std::cout << "\n \tGenerating triangles for <Cone> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	// Create the base.
	for (uint i = 0; i < uicount; i++) {
		
		// If we haven't reached the last vertex in the collection:
		if ((i + 1) < uicount) {
			vector3 bl = vertices[i]; // Bottom Left.
			vector3 br = vertices[i + 1]; // Bottom Right.
			vector3 tl = center; // Top Left.

			// std::cout << "Cone: <Base> [L|R|T] [" << glm::to_string(bl) << "|" << glm::to_string(br) << "|" << glm::to_string(tl) << "|" << " (" << i << " out of " << uicount << " vertices).\n";
			AddTri(bl, br, tl); // Add CCW triangle.
			uicount_triangles++;
		}
		// Once you've reached the last vertex in the collection.
		else {
			vector3 bl = vertices[i]; // Bottom Left.
			vector3 br = vertices[0]; // Bottom Right.
			vector3 tl = center; // Top Left.

			// std::cout << "Cone: <Base> [L|R|T] [" << glm::to_string(bl) << "|" << glm::to_string(br) << "|" << glm::to_string(tl) << "|" << " (" << i << " out of " << uicount << " vertices).\n";
			AddTri(bl, br, tl); // Add CCW triangle.
			uicount_triangles++;
		}

	}

	std::cout << "\n \tGenerating triangles for <Cone> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	// Create the side triangles.
	for (uint i = 0; i < uicount; i++) {

		if ((i + 1) < uicount) {
			vector3 bl = vertices[i + 1]; // Bottom Left.
			vector3 br = vertices[i]; // Bottom Right.
			vector3 tl = tip; // Top Left.

			// std::cout << "Cone: <Sides> [L|R|T] [" << glm::to_string(bl) << "|" << glm::to_string(br) << "|" << glm::to_string(tl) << "|" << " (" << i << " out of " << uicount << " vertices).\n";
			AddTri(bl, br, tl); // Add CCW triangle.
			uicount_triangles++;
		}
		else {
			vector3 bl = vertices[0]; // Bottom Left.
			vector3 br = vertices[i]; // Bottom Right.
			vector3 tl = tip; // Top Left.

			// std::cout << "Cone: <Sides> [L|R|T] [" << glm::to_string(bl) << "|" << glm::to_string(br) << "|" << glm::to_string(tl) << "|" << " (" << i << " out of " << uicount << " vertices).\n";
			AddTri(bl, br, tl); // Add CCW triangle.
			uicount_triangles++;
		}

	}

	std::cout << "\n \tGenerating triangles for <Cone> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	std::cout << "\n\tGenerated a <Cone> with {Total " << std::to_string(uicount_triangles) << " triangles.}";

	std::cout << "\n\tCompiling a <Cone> with {Total " << std::to_string(uicount_triangles) << " triangles.}";

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();

	std::cout << "\n\tCompiled a <Cone> with {Total " << std::to_string(uicount_triangles) << " triangles.}\n";
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// GenerateCylinder(a_fRadius, a_fHeight, a_nSubdivisions, a_v3Color);
	// -------------------------------

	uint uicount_triangles = 0;
	
	// Height and bottom values.
	float fheight = (a_fHeight / 2.0f);
	float fbottom = -(fheight);

	// Get the center point.
	vector3 center_base = vector3(0.0f, fbottom, 0.0f);

	// Get the tip.
	vector3 center_cap = vector3(0.0f, fheight, 0.0f);

	// Create collection.
	std::vector<vector3> vertices_base = std::vector<vector3>();
	uint uicount_base = 0;

	// Find points uniformly distributed on circle of provided radius.
	for (uint i = 0; i < a_nSubdivisions; i++) {

		// Calculate current angle.
		float theta = 2.0f * PI * ((float)i / (float)a_nSubdivisions); // Gets angle for the current point, in relation to the center.
		
		// Use polar coordinates to get x and z.
		float x = a_fRadius * cosf(theta);
		float z = a_fRadius * sinf(theta);

		// Push new vertex onto collection.
		vertices_base.push_back(vector3(x, fbottom, z));
		uicount_base++;
	}

	std::cout << "\n \tGenerating triangles for <Cylinder> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	// Create the base.
	for (uint i = 0; i < uicount_base; i++) {

		// If we haven't reached the last vertex in the collection:
		if ((i + 1) < uicount_base) {
			vector3 bl = vertices_base[i]; // Bottom Left.
			vector3 br = vertices_base[i + 1]; // Bottom Right.
			vector3 tl = center_base; // Top Left.

			// std::cout << "Cylinder: <Base> [L|R|T] [" << glm::to_string(bl) << "|" << glm::to_string(br) << "|" << glm::to_string(tl) << "|" << " (" << i << " out of " << uicount_base << " vertices).\n";
			AddTri(bl, br, tl); // Add CCW triangle.
			uicount_triangles++;
		}
		// Once you've reached the last vertex in the collection.
		else {
			vector3 bl = vertices_base[i]; // Bottom Left.
			vector3 br = vertices_base[0]; // Bottom Right.
			vector3 tl = center_base; // Top Left.

			// std::cout << "Cylinder: <Base> [L|R|T] [" << glm::to_string(bl) << "|" << glm::to_string(br) << "|" << glm::to_string(tl) << "|" << " (" << i << " out of " << uicount_base << " vertices).\n";
			AddTri(bl, br, tl); // Add CCW triangle.
			uicount_triangles++;
		}

	}

	std::cout << "\n \tGenerating triangles for <Cylinder> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";
	
	// Create the cap. (Draw it reverse of the base).
	for (uint i = 0; i < uicount_base; i++) {

		// If we haven't reached the last vertex in the collection:
		if ((i + 1) < uicount_base) {
			vector3 bl = vector3(vertices_base[i + 1].x, fheight, vertices_base[i + 1].z); // Bottom Left.
			vector3 br = vector3(vertices_base[i].x, fheight, vertices_base[i].z); // Bottom Right.
			vector3 tl = center_cap; // Top Left.

			// std::cout << "Cylinder: <Cap> [L|R|T] [" << glm::to_string(bl) << "|" << glm::to_string(br) << "|" << glm::to_string(tl) << "|" << " (" << i << " out of " << uicount_base << " vertices).\n";
			AddTri(bl, br, tl); // Add CCW triangle.
			uicount_triangles++;
		}
		// Once you've reached the last vertex in the collection.
		else {
			vector3 bl = vector3(vertices_base[0].x, fheight, vertices_base[0].z); // Bottom Left.
			vector3 br = vector3(vertices_base[i].x, fheight, vertices_base[i].z); // Bottom Right.
			vector3 tl = center_cap; // Top Left.

			// std::cout << "Cylinder: <Cap> [L|R|T] [" << glm::to_string(bl) << "|" << glm::to_string(br) << "|" << glm::to_string(tl) << "|" << " (" << i << " out of " << uicount_base << " vertices).\n";
			AddTri(bl, br, tl); // Add CCW triangle.
			uicount_triangles++;
		}

	}

	std::cout << "\n \tGenerating triangles for <Cylinder> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	// Create the side quads.
	for (uint i = 0; i < uicount_base; i++) {
		
		// If we haven't reached the last vertex in the collection:
		if ((i + 1) < uicount_base) {
			// Create a quad.
			vector3 bl = vertices_base[i + 1];
			vector3 br = vertices_base[i];
			vector3 tl = vector3(vertices_base[i + 1].x, fheight, vertices_base[i + 1].z);
			vector3 tr = vector3(vertices_base[i].x, fheight, vertices_base[i].z);

			// Add quad.
			AddQuad(bl, br, tl, tr);
			uicount_triangles++;
			uicount_triangles++;
		}
		// Once you've reached the last vertex in the collection.
		else {
			// Create a quad.
			vector3 bl = vertices_base[0];
			vector3 br = vertices_base[i];
			vector3 tl = vector3(vertices_base[0].x, fheight, vertices_base[0].z);
			vector3 tr = vector3(vertices_base[i].x, fheight, vertices_base[i].z);

			// Add quad.
			AddQuad(bl, br, tl, tr);
			uicount_triangles++;
			uicount_triangles++;
		}

	}

	std::cout << "\n \tGenerating triangles for <Cylinder> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	std::cout << "\n\tGenerated a <Cylinder> with {Total " << std::to_string(uicount_triangles) << " triangles.}";

	std::cout << "\n\tCompiling a <Cylinder> with {Total " << std::to_string(uicount_triangles) << " triangles.}";

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();

	std::cout << "\n\tCompiled a <Cylinder> with {Total " << std::to_string(uicount_triangles) << " triangles.}\n";
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// GenerateTube(a_fOuterRadius, a_fInnerRadius, a_fHeight, a_nSubdivisions, a_v3Color);
	// -------------------------------
	
	uint uicount_triangles = 0;

	// Height and bottom values.
	float fheight = (a_fHeight / 2.0f);
	float fbottom = -(fheight);
	float fradius_i = a_fInnerRadius;
	float fradius_o = a_fOuterRadius;

	// Get the center point.
	// vector3 center_base = vector3(0.0f, fbottom, 0.0f);

	// Get the tip.
	// vector3 center_cap = vector3(0.0f, fheight, 0.0f);

	// Unit circle collection.
	std::vector<vector3> vertices_unit = std::vector<vector3>();
	uint uicount_unit = 0;
	
	// Find points uniformly distributed on circle of outer radius.
	for (uint i = 0; i < a_nSubdivisions; i++) {
		
		// Calculate current angle.
		float theta = 2.0f * PI * ((float)i / (float)a_nSubdivisions);  // Gets angle for the current point, in relation to the center.
							
		// Calculate a unit circle for the indicated subdivisions.
		float x = cosf(theta); 
		float z = sinf(theta);

		// Push unit vertex onto collection.
		vertices_unit.push_back(vector3(x, fbottom, z));
		uicount_unit++;
	}

	std::cout << "\n \tGenerating triangles for <Tube> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	// Create base.
	for (uint i = 0; i < uicount_unit; i++) {

		// If we haven't reached the last vertex in the collection:
		if ((i + 1) < uicount_unit) {
			// Get the points for a quad.
			vector3 bl = vector3(vertices_unit[i + 1].x * fradius_i, fbottom, vertices_unit[i + 1].z * fradius_i);
			vector3 br = vector3(vertices_unit[i].x * fradius_i, fbottom, vertices_unit[i].z * fradius_i);
			vector3 tl = vector3(vertices_unit[i + 1].x * fradius_o, fbottom, vertices_unit[i + 1].z * fradius_o);
			vector3 tr = vector3(vertices_unit[i].x * fradius_o, fbottom, vertices_unit[i].z * fradius_o);

			// Build a quad.
			AddQuad(bl, br, tl, tr);
			uicount_triangles++;
			uicount_triangles++;
		}
		// Once you've reached the last vertex in the collection.
		else 
		{
			// Get the points for a quad.
			vector3 bl = vector3(vertices_unit[0].x * fradius_i, fbottom, vertices_unit[0].z * fradius_i);
			vector3 br = vector3(vertices_unit[i].x * fradius_i, fbottom, vertices_unit[i].z * fradius_i);
			vector3 tl = vector3(vertices_unit[0].x * fradius_o, fbottom, vertices_unit[0].z * fradius_o);
			vector3 tr = vector3(vertices_unit[i].x * fradius_o, fbottom, vertices_unit[i].z * fradius_o);

			// Build a quad.
			AddQuad(bl, br, tl, tr);
			uicount_triangles++;
			uicount_triangles++;
		}
	}

	std::cout << "\n \tGenerating triangles for <Tube> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	// Create cap. (Reverse of base).
	for (uint i = 0; i < uicount_unit; i++) {

		// If we haven't reached the last vertex in the collection:
		if ((i + 1) < uicount_unit) {
			// Get the points for a quad.
			vector3 bl = vector3(vertices_unit[i].x * fradius_i, fheight, vertices_unit[i].z * fradius_i);
			vector3 br = vector3(vertices_unit[i + 1].x * fradius_i, fheight, vertices_unit[i + 1].z * fradius_i);
			vector3 tl = vector3(vertices_unit[i].x * fradius_o, fheight, vertices_unit[i].z * fradius_o);
			vector3 tr = vector3(vertices_unit[i + 1].x * fradius_o, fheight, vertices_unit[i + 1].z * fradius_o);

			// Build a quad.
			AddQuad(bl, br, tl, tr);
			uicount_triangles++;
			uicount_triangles++;
		}
		// Once you've reached the last vertex in the collection.
		else
		{
			// Get the points for a quad.
			vector3 bl = vector3(vertices_unit[i].x * fradius_i, fheight, vertices_unit[i].z * fradius_i);
			vector3 br = vector3(vertices_unit[0].x * fradius_i, fheight, vertices_unit[0].z * fradius_i);
			vector3 tl = vector3(vertices_unit[i].x * fradius_o, fheight, vertices_unit[i].z * fradius_o);
			vector3 tr = vector3(vertices_unit[0].x * fradius_o, fheight, vertices_unit[0].z * fradius_o);

			// Build a quad.
			AddQuad(bl, br, tl, tr);
			uicount_triangles++;
			uicount_triangles++;
		}
	}

	std::cout << "\n \tGenerating triangles for <Tube> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	// Create outside quads.
	for (uint i = 0; i < uicount_unit; i++) {
	
		// If we haven't reached the last vertex in the collection:
		if ((i + 1) < uicount_unit) {
			// Get the points for a quad.
			vector3 bl = vector3(vertices_unit[i + 1].x * fradius_o, fbottom, vertices_unit[i + 1].z * fradius_o);
			vector3 br = vector3(vertices_unit[i].x * fradius_o, fbottom, vertices_unit[i].z * fradius_o);
			vector3 tl = vector3(vertices_unit[i + 1].x * fradius_o, fheight, vertices_unit[i + 1].z * fradius_o);
			vector3 tr = vector3(vertices_unit[i].x * fradius_o, fheight, vertices_unit[i].z * fradius_o);

			// Build a quad.
			AddQuad(bl, br, tl, tr);
			uicount_triangles++;
			uicount_triangles++;
		}
		// Once you've reached the last vertex in the collection.
		else
		{
			// Get the points for a quad.
			vector3 bl = vector3(vertices_unit[0].x * fradius_o, fbottom, vertices_unit[0].z * fradius_o);
			vector3 br = vector3(vertices_unit[i].x * fradius_o, fbottom, vertices_unit[i].z * fradius_o);
			vector3 tl = vector3(vertices_unit[0].x * fradius_o, fheight, vertices_unit[0].z * fradius_o);
			vector3 tr = vector3(vertices_unit[i].x * fradius_o, fheight, vertices_unit[i].z * fradius_o);

			// Build a quad.
			AddQuad(bl, br, tl, tr);
			uicount_triangles++;
			uicount_triangles++;
		}

	}

	std::cout << "\n \tGenerating triangles for <Tube> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	// Create inside quads. (Reverse of outside).
	for (uint i = 0; i < uicount_unit; i++) {

		// If we haven't reached the last vertex in the collection:
		if ((i + 1) < uicount_unit) {
			// Get the points for a quad.
			vector3 bl = vector3(vertices_unit[i].x * fradius_i, fbottom, vertices_unit[i].z * fradius_i);
			vector3 br = vector3(vertices_unit[i + 1].x * fradius_i, fbottom, vertices_unit[i + 1].z * fradius_i);
			vector3 tl = vector3(vertices_unit[i].x * fradius_i, fheight, vertices_unit[i].z * fradius_i);
			vector3 tr = vector3(vertices_unit[i + 1].x * fradius_i, fheight, vertices_unit[i + 1].z * fradius_i);

			// Build a quad.
			AddQuad(bl, br, tl, tr);
			uicount_triangles++;
			uicount_triangles++;
		}
		// Once you've reached the last vertex in the collection.
		else
		{
			// Get the points for a quad.
			vector3 bl = vector3(vertices_unit[i].x * fradius_i, fbottom, vertices_unit[i].z * fradius_i);
			vector3 br = vector3(vertices_unit[0].x * fradius_i, fbottom, vertices_unit[0].z * fradius_i);
			vector3 tl = vector3(vertices_unit[i].x * fradius_i, fheight, vertices_unit[i].z * fradius_i);
			vector3 tr = vector3(vertices_unit[0].x * fradius_i, fheight, vertices_unit[0].z * fradius_i);

			// Build a quad.
			AddQuad(bl, br, tl, tr);
			uicount_triangles++;
			uicount_triangles++;
		}
	}

	std::cout << "\n \tGenerating triangles for <Tube> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	std::cout << "\n\tGenerated a <Tube> with {Total " << std::to_string(uicount_triangles) << " triangles.}";
	
	std::cout << "\n\tCompiling a <Tube> with {Total " << std::to_string(uicount_triangles) << " triangles.}";

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();

	std::cout << "\n\tCompiled a <Tube> with {Total " << std::to_string(uicount_triangles) << " triangles.}\n";
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	// GenerateTorus(a_fOuterRadius, a_fInnerRadius, a_nSubdivisionsA, a_nSubdivisionsB, a_v3Color);
	// -------------------------------

	uint uicount_triangles = 0;

	// Create outside quads.
	/* for (uint i = 0; i < uicount_unit; i++) {

	// If we haven't reached the last vertex in the collection:
	if ((i + 1) < uicount_unit) {

	}
	// Once you've reached the last vertex in the collection.
	else
	{

	}

	} */

	std::cout << "\n\tGenerating a <Torus> with {Total " << std::to_string(uicount_triangles) << " triangles.}";

	std::cout << "\n\tCompiling a <Torus> with {Total " << std::to_string(uicount_triangles) << " triangles.}";

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();

	std::cout << "\n\tCompiled a <Torus> with {Total " << std::to_string(uicount_triangles) << " triangles.}";
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// Replace this with your code
	// GenerateSphere(a_fRadius, a_nSubdivisions, a_v3Color);
	// -------------------------------
	uint uicount_triangles = 0;

	// Create collection for vertices and indices.
	std::vector<vector3> vertices = std::vector<vector3>();
	std::vector<uint> indices = std::vector<uint>();

	// Add the indices 
	uint indices_count = 0;
	uint steps = 3;

	std::cout << "\n \tAdding vertices for a <Sphere> with {Total " << std::to_string(indices.size()) << " vertices.}";

#pragma region "Add indices for the vertices of the icosahedron."
	
	/*
	Face #1: { 0,4,1 }
	Face #2: { 0,9,4 }
	Face #3: { 9,5,4 }
	Face #4: { 4,5,8 }
	Face #5: { 4,8,1 }
	*/

	// Face #1
	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(1);
	indices_count += steps;

	// Face #2
	indices.push_back(0);
	indices.push_back(9);
	indices.push_back(4);
	indices_count += steps;

	// Face #3
	indices.push_back(9);
	indices.push_back(5);
	indices.push_back(4);
	indices_count += steps;

	// Face #4
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(8);
	indices_count += steps;

	// Face #5
	indices.push_back(4);
	indices.push_back(8);
	indices.push_back(1);
	indices_count += steps;

	/*
	Face #6: { 8,10,1 }
	Face #7: { 8,3,10 }
	Face #8: { 5,3,8 }
	Face #9: { 5,2,3 }
	Face #10: { 2,7,3 }
	*/

	// Face #6
	indices.push_back(8);
	indices.push_back(10);
	indices.push_back(1);
	indices_count += steps;

	// Face #7
	indices.push_back(8);
	indices.push_back(3);
	indices.push_back(10);
	indices_count += steps;

	// Face #8
	indices.push_back(5);
	indices.push_back(3);
	indices.push_back(8);
	indices_count += steps;

	// Face #9
	indices.push_back(5);
	indices.push_back(2);
	indices.push_back(3);
	indices_count += steps;

	// Face #10
	indices.push_back(2);
	indices.push_back(7);
	indices.push_back(3);
	indices_count += steps;

	/*
	Face #11: { 7,10,3 }
	Face #12: { 7,6,10 }
	Face #13: { 7,11,6 }
	Face #14: { 11,0,6 }
	Face #15: { 0,1,6 }
	*/

	// Face #11
	indices.push_back(7);
	indices.push_back(10);
	indices.push_back(3);
	indices_count += steps;

	// Face #12
	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(10);
	indices_count += steps;

	// Face #13
	indices.push_back(7);
	indices.push_back(11);
	indices.push_back(6);
	indices_count += steps;

	// Face #14
	indices.push_back(11);
	indices.push_back(0);
	indices.push_back(6);
	indices_count += steps;

	// Face #15
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(6);
	indices_count += steps;

	/*
	Face #16: { 6,1,10 }
	Face #17: { 9,0,11 }
	Face #18: { 9,11,2 }
	Face #19: { 9,2,5 }
	Face #20: { 7,2,11 }
	*/

	// Face #16
	indices.push_back(6);
	indices.push_back(1);
	indices.push_back(10);
	indices_count += steps;

	// Face #17
	indices.push_back(9);
	indices.push_back(0);
	indices.push_back(11);
	indices_count += steps;

	// Face #18
	indices.push_back(9);
	indices.push_back(11);
	indices.push_back(2);
	indices_count += steps;

	// Face #19
	indices.push_back(9);
	indices.push_back(2);
	indices.push_back(5);
	indices_count += steps;

	// Face #20
	indices.push_back(7);
	indices.push_back(2);
	indices.push_back(11);
	indices_count += steps;

#pragma endregion

	std::cout << "\n \tAdded vertices for a <Sphere> with {Total " << std::to_string(indices.size()) << " vertices.}";

	// Add the vertices.
	const float X = 0.525731112119133606f;
	const float Z = 0.850650808352039932f;
	const float N = 0.0f;
	uint vertices_count = 0;


#pragma region "Add vertices for the icosahedron."

	// Add the unit icohedron vertices.
	vertices.push_back(vector3(-X, N, Z));
	vertices.push_back(vector3(X, N, Z));
	vertices.push_back(vector3(-X, N, -Z));
	vertices.push_back(vector3(X, N, -Z));

	vertices.push_back(vector3(N, Z, X));
	vertices.push_back(vector3(N, Z, -X));
	vertices.push_back(vector3(N, -Z, X));
	vertices.push_back(vector3(N, -Z, -X));

	vertices.push_back(vector3(Z, X, N));
	vertices.push_back(vector3(-Z, X, N));
	vertices.push_back(vector3(Z, -X, N));
	vertices.push_back(vector3(-Z, -X, N));
	vertices_count += 12;

#pragma endregion

	std::cout << "\n \tSubdividing the vertices for a <Sphere> with {Total " << std::to_string(indices.size()) << " vertices.}";

	// Subdivide the vectors and indices.
	for (uint j = 0; j < a_nSubdivisions; j++) {

#pragma region "Subdivide existing indices as many times as necessary."

		// Create map/hashtable/dictionary for midpoints.
		std::map<std::string, uint> midpoints = std::map<std::string, uint>();
		std::vector<uint> indices_sub = std::vector<uint>(indices.size() * 4);
		
		// Find midpoints from existing indices.
		for (uint i = 0; i < indices_count - 2; i += steps) {
			uint vertex_i0 = indices[i];
			uint vertex_i1 = indices[i + 1];
			uint vertex_i2 = indices[i + 2];

			// Find midpoint indices.
			uint midpoint_i0 = MidpointIndex(midpoints, vertices, vertex_i0, vertex_i1);
			uint midpoint_i1 = MidpointIndex(midpoints, vertices, vertex_i1, vertex_i2);
			uint midpoint_i2 = MidpointIndex(midpoints, vertices, vertex_i2, vertex_i0);
			
			// vertex_i0, midpoint_i0, midpoint_i2
			indices_sub.push_back(vertex_i0);
			indices_sub.push_back(midpoint_i0);
			indices_sub.push_back(midpoint_i2);

			// vertex_i1, midpoint_i1, midpoint_i0
			indices_sub.push_back(vertex_i1);
			indices_sub.push_back(midpoint_i1);
			indices_sub.push_back(midpoint_i0);

			// vertex_i2, midpoint_i2, midpoint_i1
			indices_sub.push_back(vertex_i2);
			indices_sub.push_back(midpoint_i2);
			indices_sub.push_back(midpoint_i1);

			// midpoint_i2, midpoint_i0, midpoint_i1
			indices_sub.push_back(midpoint_i2);
			indices_sub.push_back(midpoint_i0);
			indices_sub.push_back(midpoint_i1);
		}

		indices.clear();
		indices = std::vector<uint>(indices_sub.size());

		for (uint p = 0; p < indices_sub.size(); p++) {
			indices[p] = indices_sub[p];
		}

		indices_count = indices_sub.size();

#pragma endregion
	
	}

	std::cout << "\n \tSubdividing the vertices for a <Sphere> with {Total " << std::to_string(indices.size()) << " vertices.}";

	std::cout << "\n \tNormalizing the vertices for a <Sphere> with {Total " << std::to_string(indices.size()) << " vertices.}";

	// Normalize all the vectors.
	for (uint k = 0; k < vertices.size(); k++) {
		vertices[k] = glm::normalize(vertices[k]);
		vertices[k] = vertices[k] * a_fRadius; // Scale the normalized vectors.
	}

	std::cout << "\n \tNormalized the vertices for a <Sphere> with {Total " << std::to_string(indices.size()) << " vertices.}";

	std::cout << "\n \tGenerating triangles for <Sphere> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	// Add triangles.
	for (uint i = 0; i < indices_count - 2; i += steps) {
		AddTri(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
		uicount_triangles++;

		// Print every a message every 1000 triangles.
		if ((uicount_triangles % 5000) == 0) {
			std::cout << "\n \tGenerating triangles for <Sphere> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";
		}
	}

	std::cout << "\n \tGenerating triangles for <Sphere> of [" << std::to_string(a_nSubdivisions) << "] subdivisions. {Completed " << std::to_string(uicount_triangles) << " triangles.}";

	std::cout << "\n\tGenerated a <Sphere> with {Total " << std::to_string(uicount_triangles) << " triangles.}";

	std::cout << "\n\tCompiling a <Sphere> with {Total " << std::to_string(uicount_triangles) << " triangles.}";

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();

	std::cout << "\n\tCompiled a <Sphere> with {Total " << std::to_string(uicount_triangles) << " triangles.}\n";
}
float MyMesh::Magnitude(float a_fx, float a_fy, float a_fz) {

	// Get magnitude.
	float r_fmag = sqrtf((a_fx * a_fx) + (a_fy * a_fy) + (a_fz * a_fz));
	return r_fmag;

}
float MyMesh::Magnitude(vector3& a_v3vertex) {
	
	// Return the magnitude.
	return Magnitude(a_v3vertex.x, a_v3vertex.y, a_v3vertex.z);

}
int MyMesh::MidpointIndex(std::map<std::string, uint> &a_midpoints, std::vector<vector3> &a_vertices, uint vertex_ind1, uint vertex_ind2) {

	std::string token1 = std::to_string(std::max(vertex_ind1, vertex_ind2));
	std::string token2 = std::to_string(std::min(vertex_ind1, vertex_ind2));
	std::string key = token1 + "_" + token2;

	int r_midpointIndex = -1;

	// If the midpoint key doesn't already exist.
	if (a_midpoints.find(key) == a_midpoints.end()) {
		vector3 vertex1 = a_vertices[vertex_ind1];
		vector3 vertex2 = a_vertices[vertex_ind2];

		vector3 midpoint = (vertex1 + vertex2) / 2.0f;

		// If the midpoint is contained within it.
		if (std::find(a_vertices.begin(), a_vertices.end(), midpoint) != a_vertices.end()) {
			r_midpointIndex = std::distance(a_vertices.begin(), std::find(a_vertices.begin(), a_vertices.end(), midpoint));
		}
		else 
		{
			r_midpointIndex = a_vertices.size();
			a_vertices.push_back(midpoint);
			a_midpoints[key] = r_midpointIndex;
		}
	}
	else {
		r_midpointIndex = a_midpoints[key];
	}
	
	return r_midpointIndex;
}