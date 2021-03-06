/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#include <gk/gl/Texture.hpp>
#include <gk/resource/ResourceHandler.hpp>

#include "Color.hpp"
#include "Text.hpp"

Text::Text() : m_texture(gk::ResourceHandler::getInstance().get<gk::Texture>("texture-font")) {
	m_background.setFillColor(gk::Color::Transparent);

	updateCharWidth();
}

void Text::setText(const std::string &text) {
	if (m_text != text) {
		m_text = text;
		updateTextSprites();
	}
}

void Text::setColor(const gk::Color &color) {
	if (m_color != color) {
		m_color = color;
		updateTextSprites();
	}
}

void Text::draw(gk::RenderTarget &target, gk::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_background, states);

	states.transform.translate(m_padding.x, m_padding.y);

	for(const gk::Sprite &sprite : m_textSprites) {
		target.draw(sprite, states);
	}
}

// FIXME: USE A VBO INSTEAD
void Text::updateTextSprites() {
	m_textSprites.clear();

	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int maxX = 0;
	gk::Color color = gk::Color{70, 70, 70, 255};
	for(char c : m_text) {
		if (c == '\n' || (m_maxLineLength && x + m_charWidth[(u8)c] >= m_maxLineLength)) {
			y += 9;
			x = 0;
			continue;
		}

		gk::Sprite sprite{"texture-font", 8, 8};
		sprite.setCurrentFrame(c);
		sprite.setPosition(x + 1, y + 1, 0);
		sprite.setColor(color);
		m_textSprites.emplace_back(std::move(sprite));
		x += m_charWidth[(u8)c];
	}
	x = 0;
	y = 0;
	color = m_color;
	for(char c : m_text) {
		if (c == '\n' || (m_maxLineLength && x + m_charWidth[(u8)c] >= m_maxLineLength)) {
			maxX = std::max(x, maxX);
			y += 9;
			x = 0;
			continue;
		}

		gk::Sprite sprite{"texture-font", 8, 8};
		sprite.setCurrentFrame(c);
		sprite.setPosition(x, y, 0);
		if (c == '[')
			color = Color::Blue;
		sprite.setColor(color);
		m_textSprites.emplace_back(std::move(sprite));
		x += m_charWidth[(u8)c];
	}

	m_size.x = std::max(x, maxX);
	m_size.y = y + 9;

	unsigned int backgroundX = std::max<int>(m_background.getSize().x, m_size.x + m_padding.x);
	unsigned int backgroundY = std::max<int>(m_background.getSize().y, m_size.y + m_padding.y);

	m_background.setSize(backgroundX, backgroundY);
}

// FIXME: Since I use the font from Minecraft assets, I needed to use
//        this piece of code to make it look good
//        I'll remove it later anyway
void Text::updateCharWidth() {
	const int width = m_texture.getSize().x;
	const int height = m_texture.getSize().y;
	unsigned int *data = new unsigned int[width * height];

	gk::Texture::bind(&m_texture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data);
	gk::Texture::bind(nullptr);

	const int charMaxHeight = height / 16;
	const int charMaxWidth = width / 16;

	for (int i = 0 ; i < 256 ; ++i) {
		if (i == ' ') {
			m_charWidth[i] = 4;
			continue;
		}

		int charX = i % 16;
		int charY = i / 16;

		if (i == 32)
			m_charWidth[i] = 4;

		int l1;
		for (l1 = charMaxWidth - 1 ; l1 >= 0 ; --l1) {
			int i2 = charX * charMaxWidth + l1;
			bool flag1 = true;

			for (int j2 = 0 ; j2 < charMaxHeight && flag1 ; ++j2) {
				int k2 = (charY * charMaxWidth + j2) * width;

				if ((data[i2 + k2] & 255) != 0)
					flag1 = false;
			}

			if (!flag1) break;
		}

		++l1;
		m_charWidth[i] = 0.5f + l1 * (8.0f / charMaxWidth) + 1;
	}

	delete[] data;
}

