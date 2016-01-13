#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../Dependencies/glew/glew.h"
#include "Utilities.h"

class Mesh
{
public:

	std::vector<Vertex> Vertices, vertexData;
	std::vector <Texcoord> Texcoords, texcoordData;
	std::vector <Normal> Normals, normalData;

	std::vector<GLuint> vertexIdx, texcoordIdx, normalIdx;

	bool TexcoordsLoaded, NormalsLoaded;
	
	explicit Mesh(std::string&& filename);

	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFace(std::stringstream& sin);
	void parseLine(std::stringstream&& sin);
	void loadMeshData(std::string& filename);
	void processMeshData();
	void freeMeshData();
};
