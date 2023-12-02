//
//  Model.cpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "phongshader.h"
#include <list>

Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{

}
Model::Model(const char* ModelFile, bool FitSize) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
	bool ret = load(ModelFile, FitSize);
	if (!ret)
		throw std::exception();
}
Model::~Model()
{
	deleteNodes(&RootNode);
}

void Model::deleteNodes(Node* pNode)
{
	if (!pNode)
		return;
	for (unsigned int i = 0; i < pNode->ChildCount; ++i)
		deleteNodes(&(pNode->Children[i]));
	if (pNode->ChildCount > 0)
		delete[] pNode->Children;
	if (pNode->MeshCount > 0)
		delete[] pNode->Meshes;
}

bool Model::load(const char* ModelFile, bool FitSize)
{
	const aiScene* pScene = aiImportFile(ModelFile, aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords);

	if (pScene == NULL || pScene->mNumMeshes <= 0)
		return false;

	Filepath = ModelFile;
	Path = Filepath;
	size_t pos = Filepath.rfind('/');
	if (pos == std::string::npos)
		pos = Filepath.rfind('\\');
	if (pos != std::string::npos)
		Path.resize(pos + 1);

	loadMeshes(pScene, FitSize);
	loadMaterials(pScene);
	loadNodes(pScene);

	return true;
}

void Model::loadMeshes(const aiScene* pScene, bool FitSize)
{
	// Quelle: https://smunix.github.io/ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

	// MeshCount setzen
	MeshCount = pScene->mNumMeshes;
	// MeshArray initialisieren
	pMeshes = new Mesh[MeshCount];
	// BoundingBox berechnen und setzen
	calcBoundingBox(pScene, BoundingBox);
	float factor = 1;

	// FitSize Faktor berechnen
	if (FitSize) {
		Vector size = BoundingBox.size();
		if (size.X > size.Y && size.X > size.Z) {
			factor = BoundingBox.boundingX / size.X;
		}
		else if (size.Y > size.X && size.Y > size.Z) {
			factor = BoundingBox.boundingY / size.Y;
		}
		else if (size.Z > size.X && size.Z > size.Y) {
			factor = BoundingBox.boundingZ / size.Z;
		}
	}

	// Über die einzelnen Meshes in pScene->mMeshes iterieren
	for (int i = 0; i < MeshCount; i++) {
		const aiMesh* pMesh = pScene->mMeshes[i];
		// Neuen Mesh struct anlegen
		Mesh& m = pMeshes[i]; // ??
		// Material-Index setzen
		m.MaterialIdx = pMesh->mMaterialIndex;

		// aiVector3D definieren
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
		const aiVector3D* pPos;
		const aiVector3D* pNormal;
		const aiVector3D* pTexCoord;

		m.VB.begin();
		// pMesh -> nach aiVector3D konvertiert
		for (int k = 0; k < pMesh->mNumVertices; k++) {
			if (pMesh->HasPositions()) {
				pPos = &(pMesh->mVertices[k]);
			}
			else {
				pPos = &Zero3D;
			}

			if (pMesh->HasNormals()) {
				pNormal = &(pMesh->mNormals[k]);
			}
			else {
				pNormal = &Zero3D;
			}

			if (pMesh->HasTextureCoords(0)) {
				pTexCoord = &(pMesh->mTextureCoords[0][k]);
				// Frage: Wozu das Multidimensionale Array wenn sich die erste Dimension eh nie verändert?
			}
			else {
				pTexCoord = &Zero3D;
			}

			// VertexBuffer befüllen
			m.VB.addNormal(Vector(pNormal->x, pNormal->y, pNormal->z));
			m.VB.addTexcoord0(pTexCoord->x, -(pTexCoord->y)); // ??
			m.VB.addVertex(Vector(pPos->x * factor, pPos->y * factor, pPos->z * factor));
		}
		m.VB.end();

		m.IB.begin();

		// Über die einzelnen Seiten eines jeden Mesh aus pMesh iterieren
		for (int l = 0; l < pMesh->mNumFaces; l++) {
			const aiFace& Face = pMesh->mFaces[l];
			// IndexBuffer befüllen, davor prüfen auf == 3, wegen Dreieck
			if (Face.mNumIndices == 3) {
				m.IB.addIndex(Face.mIndices[0]);
				m.IB.addIndex(Face.mIndices[1]);
				m.IB.addIndex(Face.mIndices[2]);
			}
		}
		m.IB.end();

		// Initialisierten Mesh struct m in pMeshes packen
		// pMeshes[i] = m;
	}

}

void Model::loadMaterials(const aiScene* pScene)
{
	// Quelle: https://cpp.hotexamples.com/de/examples/-/-/aiGetMaterialColor/cpp-aigetmaterialcolor-function-examples.html
	// https://assimp.sourceforge.net/lib_html/materials.html
	// https://www.geeksforgeeks.org/convert-string-char-array-cpp/

	// MaterialCount setzen
	MaterialCount = pScene->mNumMaterials;
	// MaterialArray pMaterials initialisieren
	pMaterials = new Material[MaterialCount];

	// Über die einzelnen Materials aus pScene->mMaterials iterieren
	for (int i = 0; i < MaterialCount; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];
		// Neuen Material struct anlegen
		Material m;

		// aiColor4D definieren, da aiGetMaterialColor() aiColor4D Objekte referenziert
		aiColor4D aiColorDiffuse = aiColor4D();
		aiColor4D aiColorSpecular = aiColor4D();
		aiColor4D aiColorAmbient = aiColor4D();
		float shininess;
		aiString aiString;

		// aiColor4D Objekte befüllen mittels aiGetMaterialColor() // aiGetMaterialFloat() // aiGetMaterialString()
		aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &aiColorDiffuse);
		aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_SPECULAR, &aiColorSpecular);
		aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_AMBIENT, &aiColorAmbient);
		aiGetMaterialFloat(pMaterial, AI_MATKEY_SHININESS, &shininess);
		pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiString);

		// Material struct m befüllen mit herausgefundenen Werten
		m.DiffColor = Color(aiColorDiffuse.r, aiColorDiffuse.g, aiColorDiffuse.b);
		m.SpecColor = Color(aiColorSpecular.r, aiColorSpecular.g, aiColorSpecular.b);
		m.AmbColor = Color(aiColorAmbient.r, aiColorAmbient.g, aiColorAmbient.b);
		m.SpecExp = shininess;

		std::string tmpFilename = Path + aiString.data;
		char* filename = new char[tmpFilename.length() + 1];
		strcpy(filename, tmpFilename.c_str());
		m.DiffTex = Texture::LoadShared(filename);
		delete[]filename;
		// Material struct m in MaterialArray schreiben
		pMaterials[i] = m;
	}
}

void Model::calcBoundingBox(const aiScene* pScene, AABB& Box)
{
	// Variablendeklaration und Initialisierung
	bool lowestXSet = false, highestXSet = false;
	bool lowestYSet = false, highestYSet = false;
	bool lowestZSet = false, highestZSet = false;
	float lowestX = 0, highestX = 0;
	float lowestY = 0, highestY = 0;
	float lowestZ = 0, highestZ = 0;

	// Über die einzelnen Meshes in pScene->mMeshes iterieren
	for (int i = 0; i < MeshCount; i++) {
		const aiMesh* pMesh = pScene->mMeshes[i];
		const aiVector3D* pPos = &aiVector3D(0.0f, 0.0f, 0.0f);

		// Über die Vertices jedes einzelnen Meshes iterieren
		for (int k = 0; k < pMesh->mNumVertices; k++) {
			if (pMesh->HasPositions()) {
				pPos = &(pMesh->mVertices[k]);
			}
			else {
				continue;
			}

			// Suchen nach dem kleinsten und größten X, Y, Z
			if (pPos->x < lowestX || lowestXSet == false) {
				lowestX = pPos->x;
				lowestXSet = true;
			}
			else if (pPos->x > highestX || highestXSet == false) {
				highestX = pPos->x;
				highestXSet = true;
			}
			else if (pPos->y < lowestY || lowestYSet == false) {
				lowestY = pPos->y;
				lowestYSet = true;
			}
			else if (pPos->y > highestY || highestYSet == false) {
				highestY = pPos->y;
				highestYSet = true;
			}
			else if (pPos->z < lowestZ || lowestZSet == false) {
				lowestZ = pPos->z;
				lowestZSet = true;
			}
			else if (pPos->z > highestZ || highestZSet == false) {
				highestZ = pPos->z;
				highestZSet = true;
			}
		}
	}
	// der übergebenen BoundingBox Box die herausgefundenen Werte zuweisen
	Box = AABB(lowestX, lowestY, lowestZ, highestX, highestY, highestZ);
}

void Model::loadNodes(const aiScene* pScene)
{
	deleteNodes(&RootNode);
	copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode)
{
	pNode->Name = paiNode->mName.C_Str();
	pNode->Trans = convert(paiNode->mTransformation);

	if (paiNode->mNumMeshes > 0)
	{
		pNode->MeshCount = paiNode->mNumMeshes;
		pNode->Meshes = new int[pNode->MeshCount];
		for (unsigned int i = 0; i < pNode->MeshCount; ++i)
			pNode->Meshes[i] = (int)paiNode->mMeshes[i];
	}

	if (paiNode->mNumChildren <= 0)
		return;

	pNode->ChildCount = paiNode->mNumChildren;
	pNode->Children = new Node[pNode->ChildCount];
	for (unsigned int i = 0; i < paiNode->mNumChildren; ++i)
	{
		copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
		pNode->Children[i].Parent = pNode;
	}
}

void Model::applyMaterial(unsigned int index)
{
	if (index >= MaterialCount)
		return;

	PhongShader* pPhong = dynamic_cast<PhongShader*>(shader());
	if (!pPhong) {
		std::cout << "Model::applyMaterial(): WARNING Invalid shader-type. Please apply PhongShader for rendering models.\n";
		return;
	}

	Material* pMat = &pMaterials[index];
	pPhong->ambientColor(pMat->AmbColor);
	pPhong->diffuseColor(pMat->DiffColor);
	pPhong->specularExp(pMat->SpecExp);
	pPhong->specularColor(pMat->SpecColor);
	pPhong->diffuseTexture(pMat->DiffTex);
}

void Model::draw(const BaseCamera& Cam)
{
	if (!pShader) {
		std::cout << "BaseModel::draw() no shader found" << std::endl;
		return;
	}
	pShader->modelTransform(transform());

	std::list<Node*> DrawNodes;
	DrawNodes.push_back(&RootNode);

	while (!DrawNodes.empty())
	{
		Node* pNode = DrawNodes.front();
		Matrix GlobalTransform;

		if (pNode->Parent != NULL)
			pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
		else
			pNode->GlobalTrans = transform() * pNode->Trans;

		pShader->modelTransform(pNode->GlobalTrans);

		for (unsigned int i = 0; i < pNode->MeshCount; ++i)
		{
			Mesh& mesh = pMeshes[pNode->Meshes[i]];
			mesh.VB.activate();
			mesh.IB.activate();
			applyMaterial(mesh.MaterialIdx);
			pShader->activate(Cam);
			glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
			mesh.IB.deactivate();
			mesh.VB.deactivate();
		}
		for (unsigned int i = 0; i < pNode->ChildCount; ++i)
			DrawNodes.push_back(&(pNode->Children[i]));

		DrawNodes.pop_front();
	}
}

Matrix Model::convert(const aiMatrix4x4& m)
{
	return Matrix(m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4);
}



