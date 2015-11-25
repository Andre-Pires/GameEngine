#include "Mesh.h"

/////////////////////////////////////////////////////////////////////// MESH

Mesh::Mesh(std::string& filename)
{
	createMesh(filename);
}

void Mesh::parseVertex(std::stringstream& sin)
{
	Vertex v;
	sin >> v.XYZW[0] >> v.XYZW[1] >> v.XYZW[2];
	v.XYZW[3] = 1.0f;
	vertexData.push_back(v);
}

void Mesh::parseTexcoord(std::stringstream& sin)
{
	Texcoord t;
	sin >> t.UV[0] >> t.UV[1];
	texcoordData.push_back(t);
}

void Mesh::parseNormal(std::stringstream& sin)
{
	Normal n;
	sin >> n.NXNYNZ[0] >> n.NXNYNZ[1] >> n.NXNYNZ[2];
	normalData.push_back(n);
}

void Mesh::parseFace(std::stringstream& sin)
{
	std::string token;
	for (int i = 0; i < 3; i++) {
		std::getline(sin, token, '/');
		if (token.size() > 0)	vertexIdx.push_back(std::stoi(token));
		std::getline(sin, token, '/');
		if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
		std::getline(sin, token, ' ');
		if (token.size() > 0) normalIdx.push_back(std::stoi(token));
	}
}

void Mesh::parseLine(std::stringstream& sin)
{
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("f") == 0) parseFace(sin);
}

void Mesh::loadMeshData(std::string& filename)
{
	std::ifstream ifile(filename);
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		parseLine(std::stringstream(line));
	}
	TexcoordsLoaded = (texcoordIdx.size() > 0);
	NormalsLoaded = (normalIdx.size() > 0);
}

void Mesh::processMeshData()
{
	for (unsigned int i = 0; i < vertexIdx.size(); i++) {
		unsigned int vi = vertexIdx[i];
		Vertex v = vertexData[vi - 1];
		Vertices.push_back(v);
		if (TexcoordsLoaded) {
			unsigned int ti = texcoordIdx[i];
			Texcoord t = texcoordData[ti - 1];
			Texcoords.push_back(t);
		}
		if (NormalsLoaded) {
			unsigned int ni = normalIdx[i];
			Normal n = normalData[ni - 1];
			Normals.push_back(n);
		}
	}
}

void Mesh::freeMeshData()
{
	vertexData.clear();
	texcoordData.clear();
	normalData.clear();
	vertexIdx.clear();
	texcoordIdx.clear();
	normalIdx.clear();
}

const void Mesh::createMesh(std::string& filename)
{
	loadMeshData(filename);
	processMeshData();
	freeMeshData();
}