#include "include.h"
#include "shader.h"
#include "fbo_render_texture.h"
#include "background.h"
using namespace std;
using namespace glm;

class RenderText
{
public:
	RenderText() {

	}
	~RenderText() {text_shader.TakeDown();}

	bool Initialize(const string & font =  "FreeSans.ttf"){
		if(FT_Init_FreeType(&ft)) {
			fprintf(stderr, "Could not init freetype library\n");
			return false;
		}

		if(FT_New_Face(ft, font.c_str(), 0, &face)) {
			fprintf(stderr, "Could not open font\n");
			return false;
		}


		text_shader.Initialize("text.vert", "text.frag");

		attribute_coord = text_shader.GetUniformLocation("coord");
		uniform_tex = text_shader.GetUniformLocation("tex");
		uniform_color = text_shader.GetUniformLocation("color");

		if(attribute_coord == -1 || uniform_tex == -1 || uniform_color == -1){
			return false;
		}

		glGenBuffers(1, &vbo);

		return true;
	}
	void RenderString(const char *text, float x, float y, float sx, float sy){
		const char *p;
		FT_GlyphSlot g = face->glyph;
		/* Create a texture that will be used to hold one "glyph" */
		GLuint tex;
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(uniform_tex, 0);
		/* We require 1 byte alignment when uploading texture data */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(attribute_coord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
		/* Loop through all characters */
		for (p = text; *p; p++) {
			/* Try to load and render the character */
			if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
				continue;
			/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
			/* Calculate the vertex and texture coordinates */
			float x2 = x + g->bitmap_left * sx;
			float y2 = -y - g->bitmap_top * sy;
			float w = g->bitmap.width * sx;
			float h = g->bitmap.rows * sy;
			point box[4] = {
				{x2, -y2, 0, 0},
				{x2 + w, -y2, 1, 0},
				{x2, -y2 - h, 0, 1},
				{x2 + w, -y2 - h, 1, 1},
			};
			/* Draw the character on the screen */
			glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			/* Advance the cursor to the start of the next character */
			x += (g->advance.x >> 6) * sx;
			y += (g->advance.y >> 6) * sy;
		}
		glDisableVertexAttribArray(attribute_coord);
		glDeleteTextures(1, &tex);

	}

	FT_Library ft;
	FT_Face face;


	GLuint vbo;
	GLint attribute_coord;
	GLint uniform_tex;
	GLint uniform_color;

	Shader text_shader;

	struct point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	};
};
