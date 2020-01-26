#include "BufferObject.h"
#include <iostream>

/**
* �o�b�t�@�I�u�W�F�N�g���쐬����.
*
* @param target �o�b�t�@�I�u�W�F�N�g�̎��.
* @param size   ���_�f�[�^�̃T�C�Y.
* @param data   ���_�f�[�^�ւ̃|�C���^.
* @param usage  �o�b�t�@�I�u�W�F�N�g�̃A�N�Z�X�^�C�v.
*
* @retval true  �쐬����.
* @retval false �쐬���s.
*/
bool BufferObject::Create(
	GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
	 {
	Destroy();
	glGenBuffers(1, &id);
	glBindBuffer(target, id);
	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
	this->target = target;
	this->size = size;
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[�G���[] " << __func__ << ": �o�b�t�@�̍쐬�Ɏ��s.\n";
		
	}
	return error == GL_NO_ERROR;
}


/**
* �o�b�t�@�Ƀf�[�^��]������.
*
* @param offset �]���J�n�ʒu(�o�C�g�P��).
* @param size   �]������o�C�g��.
* @param data   �]������f�[�^�ւ̃|�C���^.
*
* @retval true  �]������.
* @retval false �]�����s.
*/
bool BufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data)
 {
	if (offset + size >= this->size) {
		std::cerr << "[�x��] " << __func__ << ": �]����̈悪�o�b�t�@�T�C�Y���z���Ă��܂�.\n"
			 << "  buffer size:" << this->size << " offset:" << offset << " size:" << size << "\n";
		if (offset >= this->size) {
			return false;
			
		}
		    // �\�Ȕ͈͂����]�����s��.
			size = this->size - offset;
		
	}
	glBindBuffer(target, id);
	glBufferSubData(target, offset, size, data);
	glBindBuffer(target, 0);
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[�G���[] " << __func__ << ": �f�[�^�̓]���Ɏ��s.\n";
		
	}
	return error == GL_NO_ERROR;
}


/**
* Buffer Object��j������.
*/
void BufferObject::Destroy()
 {
	if (id) {
		glDeleteBuffers(1, &id);
		id = 0;
		
	}
}


/**
* VAO���쐬����.
*
* @param vbo  ���_�o�b�t�@�I�u�W�F�N�g��ID.
* @param ibo  �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g��ID.
*
* @retval true  �쐬����.
* @retval false �쐬���s.
*/
bool VertexArrayObject::Create(GLuint vbo, GLuint ibo)
 {
	Destroy();
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	vboId = vbo;
	iboId = ibo;
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[�G���[] " << __func__ << ": VAO�̍쐬�Ɏ��s.\n";
		
	}
	return error == GL_NO_ERROR;
}


/**
* VAO��j������.
*/
void VertexArrayObject::Destroy()
 {
	if (id) {
		glDeleteVertexArrays(1, &id);
		id = 0;
		vboId = 0;
		iboId = 0;
		
	}
}

/**
* VAO��OpenGL�R���e�L�X�g�Ƀo�C���h����.
*
* �`��AVertexAttribPointer()�AResetVertexAttribPointer()���Ăяo���O�ɁA
* ���̊֐������s����VAO���o�C���h����K�v������.
*
* @sa Unbind(), VertexAttribPointer(), ResetVertexAttribPointer()
*/
void VertexArrayObject::Bind() const
 {
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
}

/**
* OpenGL�R���e�L�X�g�ւ̃o�C���h����������.
*
* �`��AVertexAttribPointer()�AResetVertexAttribPointer()���I�������A
* ���̊֐������s���ăo�C���h���������Ȃ���΂Ȃ�Ȃ�.
*
* @sa Bind(), VertexAttribPointer(), ResetVertexAttribPointer()
*/
void VertexArrayObject::Unbind() const
 {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/**
* ���_�A�g���r���[�g��ݒ肷��.
*
* @param index      ���_�A�g���r���[�g�̃C���f�b�N�X.
* @param size       ���_�A�g���r���[�g�̗v�f��.
* @param type       ���_�A�g���r���[�g�̌^.
* @param normalized GL_TRUE=�v�f�𐳋K������. GL_FALSE=���K�����Ȃ�.
* @param stride     ���̒��_�f�[�^�܂ł̃o�C�g��.
* @param offset     ���_�f�[�^�擪����̃o�C�g�I�t�Z�b�g.
*
* Bind()���Ă���Unbind()����܂ł̊ԂŌĂяo������.
*
* @sa Bind(), Unbind(), ResetVertexAttribPointer()
*/
void VertexArrayObject::VertexAttribPointer(GLuint index, GLint size,
	GLenum type, GLboolean normalized, GLsizei stride, size_t offset) const
	 {
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride,
		reinterpret_cast<GLvoid*>(offset));
}


/**
* �S�Ă̒��_�A�g���r���[�g�𖳌�������.
*
* @sa Bind(), Unbind(), VertexAttribPointer()
*/
void VertexArrayObject::ResetVertexAttribPointer() const
 {
	GLint maxAttr;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttr);
	for (int i = 0; i < maxAttr; ++i) {
		glDisableVertexAttribArray(i);
		
	}
}
