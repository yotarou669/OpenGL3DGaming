/**
* @file Font.cpp
*/
#define  _CRT_SECURE_NO_WARNINGS
#include "Font.h"
#include <memory>
#include <iostream>
#include <stdio.h>


/**
* �t�H���g�`��I�u�W�F�N�g������������.
*
* @param maxCharacter  �ő�`�敶����.
*
* @retval true  ����������.
* @retval false ���������s.
*/
bool FontRenderer::Init(size_t maxCharacter)
 {
return spriteRenderer.Init(maxCharacter, "Res/Sprite.vert", "Res/Sprite.frag");
}


/**
* �t�H���g�t�@�C����ǂݍ���.
*
* @param filename �t�H���g�t�@�C����.
*
* @retval true  �ǂݍ��ݐ���.
* @retval false �ǂݍ��ݎ��s.
*/
bool FontRenderer::LoadFromFile(const char* filename)
{
	// �t�@�C�����J��.
	std::unique_ptr<FILE, decltype(&fclose)> fp(fopen(filename, "r"), &fclose);
	if (!fp) {
		std::cerr << "[�G���[] " << __func__ << ": " << filename << "���J���܂���.\n";
		return false;
	}

	// info�s��ǂݍ���.
	int line = 1; // �ǂݍ��ލs�ԍ�(�G���[�\���p).
	int spacing[2]; // �P�s�ڂ̓ǂݍ��݃`�G�b�N�p.
	int ret = fscanf(fp.get(),
		"info face=\"%*[^\"]\" size=%*d bold=%*d italic=%*d charset=%*s unicode=%*d"
		" stretchH=%*d smooth=%*d aa=%*d padding=%*d,%*d,%*d,%*d spacing=%d,%d%*[^\n]",
		&spacing[0], &spacing[1]);
	if (ret < 2) {
		std::cerr << "[�G���[]" << __func__ << ": " << filename << "�̓ǂݍ��݂Ɏ��s(" <<
			line << "�s��).\n";
		return false;

	}
	++line;


	// common�s��ǂݍ���.
	float scaleH;
	ret = fscanf(fp.get(),
		" common lineHeight=%f base=%f scaleW=%*d scaleH=%f pages=%*d packed=%*d%*[^\n]",
		&lineHeight, &base, &scaleH);
	if (ret < 3) {
		std::cerr << "[�G���[]" << __func__ << ": " << filename << "�̓ǂݍ��݂Ɏ��s(" <<
			line << "�s��).\n";
		return false;

	}
	++line;


	// page�s��ǂݍ���.
	std::vector<std::string> texNameList;
	texNameList.reserve(16);
	for (;;) {
		int id;
		char tex[256];
		ret = fscanf(fp.get(), " page id=%d file=\"%255[^\"]\"", &id, tex);
		if (ret < 2) {
			break;

		}
		tex[sizeof(tex) / sizeof(tex[0]) - 1] = '\0'; // 0�I�[��ۏ؂���.
		if (texNameList.size() <= static_cast<size_t>(id)) {
			texNameList.resize(id + 1);

		}
		texNameList[id] = std::string("Res/") + tex;
		++line;

	}
	if (texNameList.empty()) {
		std::cerr << "[�G���[]" << __func__ << ": " << filename << "�̓ǂݍ��݂Ɏ��s(" <<
			line << "�s��).\n";
		return false;

	}
	return true;
}


/**
* ������̒ǉ����J�n����.
*/
void FontRenderer::BeginUpdate()
 {
spriteRenderer.BeginUpdate();
}


/**
* �������ǉ�����.
*
* @param position �\���J�n���W(Y���W�̓t�H���g�̃x�[�X���C��).
* @param str      �ǉ�����UTF-16������.
*
* @retval true  �ǉ�����.
* @retval false �ǉ����s.
*/
bool FontRenderer::AddString(const glm::vec2& position, const wchar_t* str)
{
	glm::vec2 pos = position;
	for (const wchar_t* itr = str; *itr; ++itr) {
		const CharacterInfo& info = characterInfoList[*itr];
		if (info.id >= 0 && info.size.x && info.size.y) {
			// �X�v���C�g�̍��W�͉摜�̒��S���w�肷�邪�A�t�H���g�͍�����w�肷��.
			 // �����ŁA���̍���ł��������߂̕␳�l���v�Z����.
			const float baseX = info.size.x * 0.5f + info.offset.x;
			const float baseY = base - info.size.y * 0.5f - info.offset.y;
			const glm::vec3 spritePos = glm::vec3(pos + glm::vec2(baseX, baseY), 0);

			Sprite sprite(textures[info.page]);
			sprite.Position(spritePos);
			sprite.Rectangle({ info.uv, info.size });
			if (!spriteRenderer.AddVertices(sprite)) {
				return false;

			}

		}
		pos.x += info.xadvance; // ���̕\���ʒu�ֈړ�.

	}
	return true;
}


/**
* ������̒ǉ����I������.
*/
void FontRenderer::EndUpdate()
{
	spriteRenderer.EndUpdate();
}


/**
* �t�H���g��`�悷��.
*
* @param screenSize ��ʃT�C�Y.
*/
void FontRenderer::Draw(const glm::vec2& screenSize) const
{
	spriteRenderer.Draw(screenSize);
}


/**
* �s�̍������擾����.
*
* @return �s�̍���(�s�N�Z����).
*/
float FontRenderer::LineHeight() const
{
	return lineHeight;
}