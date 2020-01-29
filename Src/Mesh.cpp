/**
* @file Mesh.cpp
*/
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

/**
* ���b�V���Ɋւ���@�\���i�[���閼�O���.
*/
namespace Mesh
{

	/**
	* ���b�V���o�b�t�@������������.
	*
	* @param vboSize VBO�̃o�C�g�T�C�Y.
	* @param iboSize IBO�̃o�C�g�T�C�Y.
	*
	* @retval true  ����������.
	* @retval false ���������s.
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
	* ���_�f�[�^��ǉ�����.
	*
	* @param data �ǉ�����f�[�^�̃|�C���^.
	* @param size �ǉ�����f�[�^�̃o�C�g��.
	*
	* @return �f�[�^��ǉ������ʒu.
	*         CreatePrimitive��vOffset�p�����[�^�Ƃ��Ďg�p����.
	*/
	GLintptr Buffer::AddVertexData(const void* data, size_t size)
	{
		vbo.BufferSubData(vboEnd, size, data);
		const GLintptr tmp = vboEnd;
		vboEnd += ((size + 3) / 4) * 4;
		return tmp;
	}


	/**
	* �C���f�b�N�X�f�[�^��ǉ�����.
	*
	* @param data �ǉ�����f�[�^�̃|�C���^.
	* @param size �ǉ�����f�[�^�̃o�C�g��.
	*
	* @return �f�[�^��ǉ������ʒu.
	*         �v���~�e�B�u��indices�p�����[�^�Ƃ��Ďg�����Ƃ��ł���.
	*/
	GLintptr Buffer::AddIndexData(const void* data, size_t size)
	{
		ibo.BufferSubData(iboEnd, size, data);
		const GLintptr tmp = iboEnd;
		iboEnd += ((size + 3) / 4) * 4;
		return tmp;
	}


	/**
	* �v���~�e�B�u���쐬����.
	*
	* @param count    �v���~�e�B�u�̃C���f�b�N�X�f�[�^�̐�.
	* @param type     �C���f�b�N�X�f�[�^�̌^
	*                 (GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT�̂����ꂩ).
	* @param iOffset  IBO���̃C���f�b�N�X�f�[�^�̊J�n�ʒu.
	* @param vOffset  VBO���̒��_�f�[�^�̊J�n�ʒu.
	*
	* @return �쐬����Primitive�\����.
	*/
	Primitive Buffer::CreatePrimitive(
		size_t count, GLenum type, size_t iOffset, size_t vOffset) const
	{
		// �v���~�e�B�u�p��VAO���쐬.
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

		// �v���~�e�B�u�̃����o�ϐ���ݒ�.
		Primitive p;
		p.mode = GL_TRIANGLES;
		p.count = static_cast<GLsizei>(count);
		p.type = type;
		p.indices = reinterpret_cast<const GLvoid*>(iOffset);
		p.baseVertex = 0;
		p.vao = vao;
		p.material = 0; // �}�e���A����0�ԂŌŒ�.

		return p;
	}


	/**
	* �}�e���A�����쐬����.
	*
	* @param color   �}�e���A���̊�{�F.
	* @param texture �}�e���A���̃e�N�X�`��.
	*
	* @return �쐬����Material�\����.
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
	* ���b�V����ǉ�����.
	*
	* @param name      ���b�V���y�уt�@�C���̖��O.
	* @param primitive ���b�V���Ƃ��Ēǉ�����v���~�e�B�u.
	* @param material  �v���~�e�B�u�p�̃}�e���A��.
	*
	* @retval true  �ǉ�����.
	* @retval false �ǉ����s(�����̃��b�V�����o�^�ς�).
	*/
	bool Buffer::AddMesh(
		const char* name, const Primitive& primitive, const Material& material)
	{
		if (files.find(name) != files.end()) {
			std::cerr << "[�x��]" << __func__ << ": " << name <<
				"�Ƃ������O�͊��ɒǉ�����Ă��܂�.\n";
			return false;

		}

		FilePtr p = std::make_shared<File>();
		p->name = name;
		p->materials.push_back(material);
		p->meshes.resize(1);
		p->meshes[0].name = name;
		p->meshes[0].primitives.push_back(primitive);

		files.insert(std::make_pair(p->name, p));
		std::cout << "[���]" << __func__ << ": ���b�V��'" << name << "'��ǉ�.\n";
		return true;
	}

	
	/**
	* �t�@�C�����擾����.
	*
	* @param name �擾�������t�@�C���̖��O.
	*
	* @return name�Ɠ������O�������b�V���t�@�C��.
	*/
	FilePtr Buffer::GetFile(const char* name) const
	 {
	const auto itr = files.find(name);
	if (itr == files.end()) {
	std::cerr << "[�x��]" << __func__ << ": " << name <<
	"�Ƃ������O�̃��b�V���͒ǉ�����Ă��܂���.\n";
	static FilePtr empty = std::make_shared<File>();
	return empty;
	}
	return itr->second;	
	}

	/*
		�����̂�ǉ�����
		@param name�@�����̂̃��b�V����
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


		// ���_�f�[�^�ƃC���f�b�N�X�f�[�^���쐬.
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
