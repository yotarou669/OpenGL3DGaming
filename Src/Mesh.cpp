/**
* @file Mesh.cpp
*/
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

/**
* メッシュに関する機能を格納する名前空間.
*/
namespace Mesh
{

	/**
	* メッシュバッファを初期化する.
	*
	* @param vboSize VBOのバイトサイズ.
	* @param iboSize IBOのバイトサイズ.
	*
	* @retval true  初期化成功.
	* @retval false 初期化失敗.
	*/
	bool Buffer::Init(GLsizeiptr vboSize, GLsizeiptr iboSize)
	{
		if (!vbo.Create(GL_ARRAY_BUFFER, vboSize)) {
			return false;

		}
		if (!ibo.Create(GL_ELEMENT_ARRAY_BUFFER, iboSize)) {
			return false;
		}
		progStaticMesh = Shader::Program::Create("Res/StaticMesh.vert", "Res/StaticMesh.frag");
		if (progStaticMesh->IsNull()) {
			return false;
		}

		vboEnd = 0;
		iboEnd = 0;
		files.reserve(100);

		return true;
	}


	/**
	* 頂点データを追加する.
	*
	* @param data 追加するデータのポインタ.
	* @param size 追加するデータのバイト数.
	*
	* @return データを追加した位置.
	*         CreatePrimitiveのvOffsetパラメータとして使用する.
	*/
	GLintptr Buffer::AddVertexData(const void* data, size_t size)
	{
		vbo.BufferSubData(vboEnd, size, data);
		const GLintptr tmp = vboEnd;
		vboEnd += ((size + 3) / 4) * 4;
		return tmp;
	}


	/**
	* インデックスデータを追加する.
	*
	* @param data 追加するデータのポインタ.
	* @param size 追加するデータのバイト数.
	*
	* @return データを追加した位置.
	*         プリミティブのindicesパラメータとして使うことができる.
	*/
	GLintptr Buffer::AddIndexData(const void* data, size_t size)
	{
		ibo.BufferSubData(iboEnd, size, data);
		const GLintptr tmp = iboEnd;
		iboEnd += ((size + 3) / 4) * 4;
		return tmp;
	}


	/**
	* プリミティブを作成する.
	*
	* @param count    プリミティブのインデックスデータの数.
	* @param type     インデックスデータの型
	*                 (GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INTのいずれか).
	* @param iOffset  IBO内のインデックスデータの開始位置.
	* @param vOffset  VBO内の頂点データの開始位置.
	*
	* @return 作成したPrimitive構造体.
	*/
	Primitive Buffer::CreatePrimitive(
		size_t count, GLenum type, size_t iOffset, size_t vOffset) const
	{
		// プリミティブ用のVAOを作成.
		std::shared_ptr<VertexArrayObject> vao = std::make_shared<VertexArrayObject>();
		vao->Create(vbo.Id(), ibo.Id());
		vao->Bind();
		vao->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			vOffset + offsetof(Vertex, position));
		vao->VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			vOffset + offsetof(Vertex, texCoord));
		vao->VertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			vOffset + offsetof(Vertex, normal));
		vao->Unbind();

		// プリミティブのメンバ変数を設定.
		Primitive p;
		p.mode = GL_TRIANGLES;
		p.count = static_cast<GLsizei>(count);
		p.type = type;
		p.indices = reinterpret_cast<const GLvoid*>(iOffset);
		p.baseVertex = 0;
		p.vao = vao;
		p.material = 0; // マテリアルは0番で固定.

		return p;
	}


	/**
	* マテリアルを作成する.
	*
	* @param color   マテリアルの基本色.
	* @param texture マテリアルのテクスチャ.
	*
	* @return 作成したMaterial構造体.
	*/
	Material Buffer::CreateMaterial(
		const glm::vec4& color, Texture::Image2DPtr texture) const
	{
		Material m;
		m.baseColor = color;
		m.texture = texture;
		m.program = progStaticMesh;
		return m;
	}

	/**
	* メッシュを追加する.
	*
	* @param name      メッシュ及びファイルの名前.
	* @param primitive メッシュとして追加するプリミティブ.
	* @param material  プリミティブ用のマテリアル.
	*
	* @retval true  追加成功.
	* @retval false 追加失敗(同名のメッシュが登録済み).
	*/
	bool Buffer::AddMesh(
		const char* name, const Primitive& primitive, const Material& material)
	{
		if (files.find(name) != files.end()) {
			std::cerr << "[警告]" << __func__ << ": " << name <<
				"という名前は既に追加されています.\n";
			return false;

		}

		FilePtr p = std::make_shared<File>();
		p->name = name;
		p->materials.push_back(material);
		p->meshes.resize(1);
		p->meshes[0].name = name;
		p->meshes[0].primitives.push_back(primitive);

		files.insert(std::make_pair(p->name, p));
		std::cout << "[情報]" << __func__ << ": メッシュ'" << name << "'を追加.\n";
		return true;
	}

	
	/**
	* ファイルを取得する.
	*
	* @param name 取得したいファイルの名前.
	*
	* @return nameと同じ名前を持つメッシュファイル.
	*/
	FilePtr Buffer::GetFile(const char* name) const
	 {
	const auto itr = files.find(name);
	if (itr == files.end()) {
	std::cerr << "[警告]" << __func__ << ": " << name <<
	"という名前のメッシュは追加されていません.\n";
	static FilePtr empty = std::make_shared<File>();
	return empty;
	}
	return itr->second;	
	}

	/*
		立方体を追加する
		@param name　立方体のメッシュ名
	*/
	void Buffer::AddCube(const char* name)
	{
		//    6---7      +Y -Z
		//   /|  /|       |/
		//  / 5-/-4  -X --*-- +X
		// 3---2 /       /|
		// |/  |/     +Z -Y
		// 0---1
		const glm::vec3 basePositions[] = {
		{-1,-1, 1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1},
		{ 1,-1,-1}, {-1,-1,-1}, {-1, 1,-1}, { 1, 1,-1},
		};
		const glm::vec2 baseTexCoords[] = { { 0, 1}, { 0, 0}, { 1, 0}, { 1, 1} };
		const glm::vec3 normals[] = {
		{ 0, 0, 1}, { 1, 0, 0}, { 0, 0,-1}, {-1, 0, 0},
		{ 0, -1, 0}, { 0, 1, 0} };
		const int planes[6][4] = {
		{ 0, 1, 2, 3}, { 1, 4, 7, 2}, { 4, 5, 6, 7}, { 5, 0, 3, 6},
		{ 5, 4, 1, 0}, { 3, 2, 7, 6} };
		const GLubyte baseIndices[] = { 0, 1, 2, 2, 3, 0 };

		std::vector<Vertex> vertices;
		vertices.reserve(4 * 6);
		std::vector<GLubyte> indices;
		indices.reserve(6 * 6);


		// 頂点データとインデックスデータを作成.
		for (size_t plane = 0; plane < 6; ++plane) {
			for (size_t i = 0; i < 4; ++i) {
				Vertex v;
				v.position = basePositions[planes[plane][i]];
				v.texCoord = baseTexCoords[i];
				v.normal = normals[plane];
				vertices.push_back(v);

			}
			for (size_t i = 0; i < 6; ++i) {
				indices.push_back(static_cast<GLubyte>(baseIndices[i] + (plane * 4)));

			}

		}
	}
	
} // namespace Mesh
