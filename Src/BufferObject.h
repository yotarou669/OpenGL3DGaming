#ifndef BUFFEROBJECT_H_INCLUDED
#define BUFFEROBJECT_H_INCLUDED
#include <GL/glew.h>

/**
* �ėp�o�b�t�@�I�u�W�F�N�g(VBO, IBO�Ȃ�).
*/
class BufferObject
{
public:
	BufferObject() = default;
	~BufferObject() { Destroy(); }
	BufferObject(const BufferObject&) = delete;
	BufferObject & operator=(const BufferObject&) = delete;

	bool Create(GLenum target, GLsizeiptr size, const GLvoid* data = nullptr,
		GLenum usage = GL_STATIC_DRAW);
	bool BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data);
	void Destroy();
	GLuint Id() const { return id; }
	GLsizeiptr Size() const { return size; }
private:
	GLenum target = 0; ///< �o�b�t�@�̎��.
	GLuint id = 0; ///< �Ǘ�ID.
	GLsizeiptr size = 0; ///< �o�b�t�@�̃o�C�g�T�C�Y.
};


/**
* VAO�p.
*/
class VertexArrayObject
 {
public:
	VertexArrayObject() = default;
	~VertexArrayObject() { Destroy(); }
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject & operator=(const VertexArrayObject&) = delete;
	
		bool Create(GLuint vbo, GLuint ibo);
	void Destroy();
	void ResetVertexAttribPointer() const;
	void Bind() const;
	void Unbind() const;
	void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized,
		GLsizei stride, size_t offset) const;
	GLuint Id() const { return id; }
	GLuint Vbo() const { return vboId; }
	GLuint Ibo() const { return iboId; }
	
private:
	GLuint id = 0; ///< �Ǘ�ID
	GLuint vboId = 0; ///< VBO�̊Ǘ�ID
	GLuint iboId = 0; ///< IBO�̊Ǘ�ID
};

#endif // BUFFEROBJECT_H_INCLUDED